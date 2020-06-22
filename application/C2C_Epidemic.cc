/*
 * C2C_Epidemic.cc
 *
 *  Created on: Jun 16, 2020
 *      Author: thimus
 */

#include "C2C_Epidemic.h"
#include <boost/tokenizer.hpp>

using namespace boost;
Define_Module(C2C_Epidemic);

C2C_Epidemic::C2C_Epidemic() {
    // TODO Auto-generated constructor stub

}



void C2C_Epidemic::initialize(int stage)
{
    BasicAppl::initialize(stage);

    if(stage == 0)
    {
        ourSerial = -1;
        // get the value of parameters
        nodeID = nodePtr->getId();
        speed = par("speed");
        if(speed < 0)
            error("value for speed is incorrect !!");

        beacon_Interval = par("beacon_Interval");
        EV<<"beacon_Interval is "<< beacon_Interval<<endl;
        if(beacon_Interval <= 0)
            error("value for beacon_Interval is incorrect !!");

        plist_Interval = par("plist_Interval");
        if(beacon_Interval < plist_Interval)
             error("value for plist_Interval is incorrect !!");

        message_Lost = par("message_Lost");
        // set variables
        CRL_Piece_No = 0;
        CRLChannel = -1;
        state = STATE_IDLE;

        totFrames = 0;
        forCounter = 0;

        //initialize
        No_CRL_Piece.setName("No of CRL Pieces");

        selectedBeacon = NULL;

        //initializing statistics variables
        dup_msg = 0;
        org_msg = 0;
        all_pieces = 0;
        plist_msg = 0;

        // define and start Timer1 (send broadcast beacon)
        Timer1 = new cMessage( "Timer_Beacon_V", Timer_Beacon_V );
        scheduleAt(simTime() + dblrand() * beacon_Interval, Timer1);

        //define Timer 2 (start sending PList)
        Timer2 = new cMessage("Timer_Wait_Plist",Timer_Wait_PList);
    }
    // check if we are in second stage (stage == 1) of initialization!
    else if(stage == 1)
    {

            traci = TraCIMobilityAccess().get(getParentModule());
            broadcastMask = new int[totalPieces];
            for (int i=0; i<totalPieces; i++)
            broadcastMask[i] = 1;    // Initialize all elements to 1

    }
}


void C2C_Epidemic::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        if( msg->getKind() == Timer_Beacon_V )//Timer 1
        {
            firstTime = true;
            sendBeacon(-1, true);    // -1: broadcast beacon

        }
        if(msg->getKind()== Timer_Wait_PList)//Timer2
        {
            //We will send our own Plist to the selected beacon
            if(selectedBeacon)//checking if selected beacon is there or not
            {
                if(PiecesCRLcollected.size()!= totalPieces)//if we have all the CRL pieces we don't need to send the PList
                Send_CRL_List_Msg(-1,selectedBeacon->getSrcAddr());//-1 indicates we are first sending Plist message
            }
        }


    }
    else if(msg->getArrivalGateId()==lowerLayerOut)
    {

    }
    else if(msg->getArrivalGateId()==lowerControlOut)
    {

    }
    else if(msg->getArrivalGateId()==lowerControlIn)
    {

    }
    else if(msg->getArrivalGateId()==lowerLayerIn)
    {
        if(msg->getKind() == Msg_Beacon_V)
        {

            recieveBeacon(msg);
        }
        else if(msg->getKind() == Msg_Beacon_RSU)
        {

            recieveBeacon(msg);
        }
        // Receive a CRL message from RSU
        else if(msg->getKind() == Msg_CRL_RSU)
        {
            if(uniform(0,1) < message_Lost)
            {

                        nodePtr->bubble("Message Lost");
                        delete msg;

            }
            else
            recieveCRL(msg);
        }
        // Receive a CRL message from vehicle
        else if(msg->getKind() == Msg_CRL_V)
        {

            recieveCRL(msg);
        }
        else if(msg->getKind() == Msg_Pieces_list)
        {
            recievePlist(msg);
        }
        else
            delete msg;
    }
}
void C2C_Epidemic::recievePlist(cMessage* msg)
{
    plist_msg++;
    Piece_list *pkt = static_cast<Piece_list *>(msg);
    if(pkt == NULL)
      return;
    EV<<"PList is recieved "<<endl;
    EV<<"serial: "<<pkt->getSerial()<<endl;
    EV<<"message: "<<pkt->getList()<<endl;
    EV<<"Sender: "<<pkt->getSender()<<endl;
    int pieceNo = PiecesCRLcollected.size();
    if(ourSerial != pkt->getSerial())//we have not sent our Plist to this vehicle
    {
        if((pieceNo!=totalPieces)||(pieceNo!=0)) //checking that we have some CRL pieces to send
                                             //if we have all the pieces then we don't need to send Plist
        Send_CRL_List_Msg(pkt->getSerial(),pkt->getSender());
    }
    if(pieceNo!=0)//sending CRL if we have some pieces
    {
    //We have to send all the CRL pieces that are not in the Plist
    // Update the broadcastMask array
    UpdateMask(pkt);

    // send the incremental-CRL update to the other node
    //sendCRLsMask(pkt->getSender());

    //broadcasting the incremental CRL
    sendCRLsMask(1);

    // reset the broadcastMask
    for(int i=0; i<totalPieces;i++)
        broadcastMask[i] = 1;
    }
}

void C2C_Epidemic::sendBeacon(veins::LAddress::L2Type RecID, bool periodic)
{
    // RecID is the receiver ID. it will be stored in the DestAddr field of the beacon.
    // RecID = -1 means application layer broadcast

    if(RecID == -1)
    {
        EV << "**** " << nodePtr->getFullName() << " is sending a broadcast beacon ..." << endl;


    }
    else
    {
        EV << nodePtr->getFullName() << " is sending a unicast beacon ..." << endl;


    }

    //if ( getEnvir()->isGUI() ) nodePtr->bubble("Sending beacon");

    Beacon *pkt = new Beacon(nodePtr->getFullName(), Msg_Beacon_V);

    pkt->setSrcAddr(mac->getMACAddress());
    pkt->setDestAddr(RecID);

    pkt->setNodeName(nodePtr->getFullName());

    // get and then set the current position of the node
    int xPos = std::atoi( nodePtr->getDisplayString().getTagArg("p",0) );  // x-coordinate of node
    int yPos = std::atoi( nodePtr->getDisplayString().getTagArg("p",1) );  // y-coordinate of node
    pkt->setPositionX( xPos );
    pkt->setPositionY( yPos );

    pkt->setSpeed(speed);

    // ####################################################
    // set the NeedCRL flag (used in C2C_Epidemic and ICE)
    // ####################################################

    if( (PiecesCRLcollected.size() == (unsigned int)totalPieces) )
        pkt->setNeedCRL(false);
    else
        pkt->setNeedCRL(true);

    // ############################################
    // set the CRL_Piece_No (used in MPB and ICEv2)
    // ############################################

    pkt->setCRL_Piece_No(CRL_Piece_No);

    // ###################################################
    // set the rangeStart and rangeEnd (used only by ICE)
    // ###################################################

    EV << "**** " << nodePtr->getFullName() << " has these pieces: ";

    for(unsigned int i=0; i<PiecesCRLcollected.size();i++)
        EV << PiecesCRLcollected[i]->getSeqNo() << ", ";

    EV << endl;

    // set the start and end index of range
    pkt->setRangeS(0);
    pkt->setRangeE(totalPieces-1);
    pkt->setMyResult(0);



    pkt->setChannelNumber(static_cast<int>(veins::Channel::cch));

    // ###################################################
    // ###################################################

    pkt->setBitLength(1000);
    pkt->setRecipientAddress(RecID);

    send(pkt, lowerLayerOut);

    if(periodic) scheduleAt(simTime() + beacon_Interval, Timer1);
    //schedule for sending PList message
    //start Timer2
    scheduleAt(simTime() +  plist_Interval, Timer2);
}


// receive a beacon from an RSU or a vehicle
void C2C_Epidemic::recieveBeacon(cMessage *msg)
{
    EV<<"Beacon Message is recieved\n";
    Beacon *m = static_cast<Beacon *>(msg);
    if (m == NULL) return;

    if ( getEnvir()->isGUI() )
    {
        // print on output, which type of beacon we have received.
        if(m->getDestAddr() == -1)
        {
            EV << "*** " << nodePtr->getFullName() << " received a broadcast beacon from " << m->getNodeName() << endl;
       //     nodePtr->bubble("received a broadcast beacon");
        }
        else if(m->getDestAddr() == nodeID)
        {
            EV << "*** " << nodePtr->getFullName() << " received a unicast beacon from " << m->getNodeName() << endl;
        //    nodePtr->bubble("received a unicast beacon");
        }
        else
        {
            EV << "*** " << nodePtr->getFullName() << " received a unicast beacon (not-mine) from " << m->getNodeName() << endl;
         //   nodePtr->bubble("received a unicast beacon (not-mine)");
        }

        EV << "*** Extracting beacon information: " << endl;

        EV << "source Add: " << m->getSrcAddr() << endl;
        EV << "destination Add: " << m->getDestAddr() << endl;
        EV << "node name: " << m->getNodeName() << endl;
        EV << "position (x,y): " << m->getPositionX() << ", " << m->getPositionY() << endl;
        EV << "speed: " << m->getSpeed() << endl;
        EV << "CRL channel: " << m->getCRLchannel()  << endl;
        EV << "Need CRL: " << m->getNeedCRL() << endl;
        EV << "My Result" << m->getMyResult()<<endl;

    }

        selectedBeacon = m->dup();

        delete msg;
}


// Receive a CRL from RSU or from another vehicle
void C2C_Epidemic::recieveCRL(cMessage *msg)
{


    CRL_Piece *m = static_cast<CRL_Piece *>(msg);
    if (m == NULL) return;

    if ( getEnvir()->isGUI() )
    {
        EV << "*** " << nodePtr->getFullName() << " received a CRL piece:" << endl;

        EV << "Msg name: " << m->getName() << endl;
        EV << "CRL Version: " << m->getCRLversion() << endl;
        EV << "Timestamp: " << m->getTimestamp() << endl;
        EV << "Sequence Number: " << m->getSeqNo() << endl;
        EV << "CA ID: " << m->getCAid() << endl;
        EV << "Padding: " << m->getPad() << endl << endl;
    }

    // add it to PiecesCRLcollected, but first search it in PiecesCRLcollected to
    // see if we previously received it. We use SeqNo as the key
    bool found = false;

    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        if( m->getSeqNo() == PiecesCRLcollected[i]->getSeqNo() )
        {
            found = true;
            break;
        }
    }

    if(found)
    {
        if ( getEnvir()->isGUI() && PiecesCRLcollected.size()!=totalPieces)
        {
            dup_msg++;
            nodePtr->bubble("duplicate copy !!");
            EV << " --> duplicate copy!!" << endl;
        }

        delete msg;  // delete the message
    }
    // no copy is found in PiecesCRLcollected
    else
    {
        if ( getEnvir()->isGUI() )
        {
            nodePtr->bubble("received a new CRL piece");
            EV << " --> save in array." << endl;
        }
        org_msg++;
        PiecesCRLcollected.push_back(m);  // store the CRL internally

        CRL_Piece_No++;  // Increment the number of CRLs received so far
        No_CRL_Piece.record(CRL_Piece_No);
        if(CRL_Piece_No==totalPieces)
        {
            all_pieces=simTime();
            record();
        }
        if(getEnvir()->isGUI())
        {
            EV << "*** now we have " << CRL_Piece_No << " CRL pieces." << endl;
            updateText();  // update the text in top of the V nodes.
        }


        // check to see if we can re-construct the CRL message, with this CRL piece.
        CRLreconstruction();

        // send signal to statistics to report number of CRLs received so far.
//        simsignal_t Signal_NoCRLs_V = registerSignal("NoCRLs_V");
//        nodePtr->emit(Signal_NoCRLs_V, CRL_Piece_No);
    }
}




void C2C_Epidemic::updateText()
{
    char buf[15];
    sprintf(buf, "pieces: %d", CRL_Piece_No);

    nodePtr->getDisplayString().setTagArg("t", 0, buf);
}



void C2C_Epidemic::CRLreconstruction()
{
    // if ErasureCode is not enabled, we have to wait to receive all
    // CRL pieces to be able to reconstruct the original CRL message.
    if( CRL_Piece_No == NoSegments)
    {
        EV << endl;
        EV << "*** " << nodePtr->getFullName() << " received all " << NoSegments << " CRL pieces." << endl;
        EV << "Seq number of the received CRL pieces are: ";

        for(unsigned int i=0;i<PiecesCRLcollected.size();i++)
        {
            EV << PiecesCRLcollected[i]->getSeqNo() << "  ";
        }

        EV << endl;

        EV << "Original CRL message can be re-constructed." << endl << endl;

        // do the real CRL reconstruction
        if(false) //it is made false to save reconstruction timing
            CRLreconstruction_NoErasure();

    }
}


void C2C_Epidemic::CRLreconstruction_NoErasure()
{
    // we have CRL pieces in PiecesCRLcollected vector.
    // We should sort the pieces based on the SeqNo.
    std::vector<CRL_Piece *> vec (PiecesCRLcollected);

    for(unsigned int i = 0; i<vec.size()-1; i++)
    {
        for(unsigned int j = i+1; j<vec.size(); j++)
        {
            if( vec[i]->getSeqNo() > vec[j]->getSeqNo())
                std::swap(vec[i], vec[j]);
        }
    }

    // now we are able to re-construct the original CRL message.
    std::stringstream ostr;

    for(unsigned int i = 0; i<vec.size(); i++)
        ostr << vec[i]->getPayload();

    // use boost to de-serialize
    std::vector<Certificate *> recCRL;

    {
        boost::archive::text_iarchive ia(ostr);
        // read class state from archive
        ia >> recCRL;
    }

    EV << "Re-constructed CRL is:" << endl;

    for(unsigned int i = 0; i < recCRL.size(); i++)
    {
        EV << "Certificate name: " << recCRL[i]->CerName << ", ";
        EV << "CA name: " << recCRL[i]->CAname << ", ";
        EV << "CA ID: " << recCRL[i]->CAid << ", ";
        EV << "Node ID: " << recCRL[i]->NodeID << ", ";
        EV << "Node Name: " << recCRL[i]->NodeName << endl;
    }

    EV << endl;
}



double C2C_Epidemic::round(double r)
{
    double n = (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);

    return abs(n);
}

void C2C_Epidemic::Send_CRL_List_Msg(int rSerial,veins::LAddress::L2Type recId)
{
    bubble("Sending plist");
    std::ostringstream oss;
    oss.str("");

    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        oss << PiecesCRLcollected[i]->getSeqNo();
        oss << ", ";
    }

    Piece_list *pkt = new Piece_list(nodePtr->getFullName(), Msg_Pieces_list);
    int serial = 0;
    if(rSerial == -1)
    {
        // generate a random number from 0 to 32766
        serial = intrand(32767);
    }
    else
        serial = rSerial;
    ourSerial = serial;//storing our Serial to know that we have recieved our own Plist message
    pkt->setSerial(serial);
    pkt->setList(oss.str().c_str());
    pkt->setBitLength(18400);
    pkt->setChannelNumber(static_cast<int>(veins::Channel::cch));
    pkt->setRecipientAddress(recId);
    pkt->setSender(mac->getMACAddress());
    EV << "*** " << nodePtr->getFullName() << " is sending PList message: ";
    EV << "Serial: " << serial << ", List: " << oss.str() << endl;

    // send the pkt
    send(pkt, lowerLayerOut);
      std::cout<<"CRL List Sent by "<<nodePtr->getFullName()<<endl;
}
void C2C_Epidemic::UpdateMask(Piece_list *m)
{
     // tokenize the received string
     std::string text = m->getList();
     char_separator<char> sep(", ");
     tokenizer< char_separator<char> > tokens(text, sep);

     for(tokenizer< char_separator<char> >::iterator beg=tokens.begin(); beg!=tokens.end();++beg)
     {
         std::string str = *beg;
         int PieceNumber = std::atoi(str.c_str());
         broadcastMask[PieceNumber] = 0;
     }
}


// broadcast CRL pieces of the vehicle with respect to broadcastMask[i] (used only in Epidemic and ICE).
void C2C_Epidemic::sendCRLsMask(veins::LAddress::L2Type recID)
{
    if ( getEnvir()->isGUI() )
    {
        nodePtr->bubble("Sending CRLs");
        EV << "**** " << nodePtr->getFullName();
        EV << ": sending pieces on channel "  << endl;

        EV << "broadcastMask is: ";

        for(int h=0; h<totalPieces; h++)
        {
            EV << broadcastMask[h] << ", ";
        }

        EV << endl;

        EV << "broadcast CRL pieces are: ";
    }

    double time = 0;
    int counter = 0;

    // iterate on all pieces in PiecesCRLcollected
    // note that the pieces are not sorted by SeqNo
    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        // if corresponding bit in Mask variable is zero, then skip sending this piece
        if(broadcastMask[PiecesCRLcollected[i]->getSeqNo()] == 0)
            continue;

        // create the packet for transmitting a certificate
        CRL_Piece *pkt = PiecesCRLcollected[i]->dup();

        pkt->setName(nodePtr->getFullName());
        pkt->setKind(Msg_CRL_V);
        pkt->setBitLength(18400);
        pkt->setChannelNumber(static_cast<int>(veins::Channel::cch));
        pkt->setRecipientAddress(recID);
        pkt->setSender(mac->getMACAddress());

        // we have time constrain (tho).
        // we have to calculate how many CRL pieces can we send in tho.
        // the calculation considers size of frame and data rate.
        // I2V_tho == 0 means we don't have any restriction.
//        if(V2V_tho != 0)
//        {
//            // transmission time for a single frame
//            // (32 bits header for network layer and 192 bits for data-link layer)
//            double TxTime = ( pkt->getBitLength() + 32 + 272 + 192) / bitrate;
//            time = time + TxTime;
//            if(time >= V2V_tho)
//            {
//                delete pkt;
//                break;
//            }
//        }

        counter++;
        EV << pkt->getSeqNo() << ", ";

        // send the pkt
        send(pkt, lowerLayerOut);
        std::cout<<nodePtr->getFullName()<<" is sending  CRL PIECE==========="<<endl;
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLcollected.size() << " are sent." << endl;
}


void C2C_Epidemic::record()
{

}

void C2C_Epidemic::finish()
{
    recordScalar("Duplicate Messages",dup_msg);
    recordScalar("Original Messages",org_msg);
    recordScalar("Plist Message",plist_msg);
    recordScalar("Full CRL Recieved Time",all_pieces);

}


C2C_Epidemic::~C2C_Epidemic()
{
    cancelAndDelete(Timer1);
       cancelAndDelete(Timer2);
       delete selectedBeacon;


        for(unsigned int i = 0; i < PiecesCRLcollected.size(); i++)
        {
            // only if the Msg does not come from CA magically!
            if(PiecesCRLcollected[i]->getKind() != Msg_CRL_CA)
            {
                delete PiecesCRLcollected[i];
            }
        }
        delete broadcastMask;
}


