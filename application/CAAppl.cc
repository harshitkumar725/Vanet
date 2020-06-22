//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "CAAppl.h"
Define_Module(CAAppl);
CAAppl::CAAppl() {

}



void CAAppl::initialize(int stage)
{
    BasicAppl::initialize(stage);

    if(stage == 0)
    {
        dataToRSU = findGate("dataToRSU");

        InitialWait = par("InitialWait");
        if(InitialWait < 0)
            error("value for InitialWait is incorrect !!");

        CRLsize = par("CRLsize");
        if(CRLsize <= 0)
            error("value for CRLsize is incorrect !!");

        EnableShuffle = par("EnableShuffle").boolValue();

        Pseudonym_lifeTime = par("Pseudonym_lifeTime").doubleValue();
        if(Pseudonym_lifeTime <= 0)
            error("value for Pseudonym_lifeTime is incorrect !!");


        NoSegments = par("NoSegments");

        if(NoSegments <= 0)
                error("Value of NoSegments is incorrect! Check configuration file.");

        totalPieces = NoSegments;


        Timer1 = new cMessage( "Timer_Initial_Wait_CA", Timer_Initial_Wait_CA );
        scheduleAt(simTime() + InitialWait, Timer1);

        Timer2 = new cMessage( "Timer_Wait_Tx", Timer_Wait_Tx );
    }
}


void CAAppl::handleSelfMsg(cMessage *msg){
    if(msg->getKind()==Timer_Initial_Wait_CA)
    {
       createCRL();
    }
    else if(msg->getKind()==Timer_Wait_Tx)
    {
       sendBuffer();
    }
}




void CAAppl::createCRL()
{
    // CRL consists of number of certificates
    std::vector<Certificate *> CRL;

    // CRLsize is a parameter
    for(int i = 0; i< CRLsize; i++)
    {
        char buf[25];
        sprintf(buf, "Node%d_Certificate", i+1);

        Certificate *cert = new Certificate();

        cert->CerName = buf;
        cert->CAname = "CA1";
        cert->CAid = 12;
        cert->NodeName = "V[x]";
        cert->NodeID = i+1;

        CRL.push_back(cert);
    }

    EV << "*** " << nodePtr->getFullName() << " created a CRL with size " << CRLsize << endl;



    // do some processing on CRL vector!
    processCRL(CRL);
}

void CAAppl::processCRL(std::vector<Certificate *> CRL)
{
    // Step 1: change CRL message into bytes and save it in CRLbytes stream.
    // CRLbytes is defined globally to be reachable in Noerasure() and erasure() methods.
    {
        boost::archive::text_oarchive oa(CRLbytes);
        oa << CRL;
    }


    // todo:
    char fileName [30];
    sprintf( fileName, "matrix.txt");
    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");



    unsigned long len = CRLbytes.str().size();

    EV << endl << "CRL is treated as raw data (sequence of " << len << " bytes):" << endl;

    for(unsigned long i=0; i<len; i++)
    {
        EV << (unsigned int)CRLbytes.str().at(i) << " " ;
        fprintf (pFile, "%d ", (unsigned int)CRLbytes.str().at(i) );
    }

    fprintf (pFile, "\n");
    fclose(pFile);

    EV << endl << endl;

    // Step 2: apply the erasure code if its enable.
    std::vector<std::string> EncodedCRL;


        EncodedCRL = NOerasure();

    // Step 3: adding header to each of the entries in EncodedCRL.
    // PiecesCRL is defined in header file to be accessible from the whole class.
    PiecesCRL = addHeader(EncodedCRL);

    // now we have all CRL pieces in PiecesCRL vector
    fillBuffer(PiecesCRL);

    // we filled the buffer, and now send it
    sendBuffer();
}


std::vector<std::string> CAAppl::NOerasure()
{
    EV << "*** " << nodePtr->getFullName() << ": erasure code is disabled!" << endl;
    EV << "Dividing CRL message into "<< NoSegments << " segments." << endl;

    unsigned long len = CRLbytes.str().size();
    int n;   // number of bytes in each pieces

    if(len % NoSegments == 0)
        n = len / NoSegments;
    else
        n = (len / NoSegments) + 1;

    EV << "This means that each segment has maximum of " << n << " bytes." << endl;

    // now we save the bytes of each piece to the tmp vector.
    std::vector<std::string> tmp;
    std::ostringstream oss;

    for(unsigned long i=0; i<len; i++)
    {
        EV << (unsigned int)CRLbytes.str().at(i) << " ";
        oss << CRLbytes.str().at(i);

        if(i != 0 && (i+1)%n == 0)
        {
            EV << "  -   ";
            tmp.push_back(oss.str());
            oss.str("");
        }
    }

    if(len % NoSegments != 0)
        tmp.push_back(oss.str());

    EV << endl << endl;

    return tmp;
}

std::vector<CRL_Piece *> CAAppl::addHeader(std::vector<std::string> vec)
{
    EV << "*** " << nodePtr->getFullName() << " is adding header to each entry." << endl;

    // generate a random initial Sequence number
    // int ISeqNo = 1 + intrand(1000);
    int ISeqNo = 0;

    // a vector for storing the CRL pieces
    std::vector<CRL_Piece *> vecResult;

    for(unsigned int i=0; i< vec.size(); i++)
    {
        // create the packet for transmitting a certificate
        CRL_Piece *pkt = new CRL_Piece(nodePtr->getFullName(), Msg_CRL_CA);

        pkt->setCRLversion(1);
        pkt->setTimestamp(0);
        pkt->setSeqNo(ISeqNo++);
        pkt->setCAid(23);

            pkt->setPad(-1);

        pkt->setPayload(vec[i].c_str());

        pkt->setBitLength(headerLength);   // size of each msg is 2300 Bytes.

        // pkt is ready. we add it to the vector
        vecResult.push_back(pkt);
    }

    EV << "*** " << " CRL pieces are ready: ";

    for(unsigned int i=0; i<vecResult.size(); i++)
        EV << vecResult[i]->getSeqNo() << "  ";

    EV << endl << endl;

    return vecResult;
}


std::vector<CRL_Piece *> CAAppl::shuffle(std::vector<CRL_Piece *> vec)
{
    int n = 0;

    for (int i = vec.size() - 1; i >= 0; i--)
    {
        n = intrand( vec.size() );

        if(i != n)
            std::swap(vec[i], vec[n]);
    }

    return vec;
}


// send the CRL pieces to all RSUs
void CAAppl::fillBuffer(std::vector<CRL_Piece *> PiecesCRL)
{
    if ( getEnvir()->isGUI() )  nodePtr->bubble("Sending CRL Msg");

    std::vector<bufferEntry *> TempBuffer;

    // get number of RSUs
    int RSUNo = nodePtr->getParentModule()->par("numRSU");

    for(int i=0; i<RSUNo; i++)
    {
        // shuffling is not enabled
        if(!EnableShuffle)
        {
            for(unsigned int j=0; j < PiecesCRL.size(); j++)
            {
                bufferEntry *entry = new bufferEntry();
                entry->piece = PiecesCRL[j];
                entry->RSU = i;
                entry->sent = false;

                TempBuffer.push_back(entry);
            }
        }
        // send a shuffled version of array to RSU i
        else
        {
            std::vector<CRL_Piece *> PiecesCRL_shuffled;
            PiecesCRL_shuffled = shuffle(PiecesCRL);

            for(unsigned int j=0; j < PiecesCRL_shuffled.size(); j++)
            {
                bufferEntry *entry = new bufferEntry();
                entry->piece = PiecesCRL_shuffled[j];
                entry->RSU = i;
                entry->sent = false;

                TempBuffer.push_back(entry);
            }
        }
    }

    int index;

    // re-arrange the order of buffer to increase efficiency
    for(int i=0; i<totalPieces; i++)
    {
        for(int j=0; j<RSUNo; j++)
        {
            index = (totalPieces * j) + i;

            bufferEntry *entry = new bufferEntry;
            entry->RSU = TempBuffer[index]->RSU;
            entry->piece = TempBuffer[index]->piece;
            entry->sent = TempBuffer[index]->sent;

            buffer.push_back(entry);
        }
    }
}


// send buffer to RSUs
void CAAppl::sendBuffer()
{
    for(unsigned int k=0; k < buffer.size(); k++)
    {
        bufferEntry *entry = buffer[k];

        // if this entry has not been sent
        if(!entry->sent)
        {
            // now check if the channel is free
            cChannel *txChannel = gate("dataToRSU", entry->RSU)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();

            // channel free; send out the packet immediately
            if (txFinishTime <= simTime())
            {
                EV << "*** " << nodePtr->getFullName() << " is sending piece ";
                EV << entry->piece->getSeqNo() << " to RSU " << entry->RSU;
                EV << " at " << simTime() << endl;

                send(entry->piece->dup(), "dataToRSU", entry->RSU);

                entry->sent = true;
            }
            else
            {
                scheduleAt(txFinishTime, Timer2);

                // return from this method
                return;
            }
        }
    }
}


void CAAppl::finish()
{


}


CAAppl::~CAAppl()
{
    cancelAndDelete(Timer1);
    cancelAndDelete(Timer2);

    for(unsigned int i = 0; i < PiecesCRL.size(); i++)
    {
        delete PiecesCRL[i];
    }
}


