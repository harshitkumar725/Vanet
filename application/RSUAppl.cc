

#include "RSUAppl.h"
Define_Module(RSUAppl);
RSUAppl::RSUAppl() {
    // TODO Auto-generated constructor stub

}
void RSUAppl::initialize(int stage)
{
    BasicAppl::initialize(stage);

    if(stage == 0)
    {
        dataFromCA = findGate("dataFromCA");

        forCounter = 0;//for sending periodic CRL
        AnyoneNeedCRL = false;
        state = STATE_IDLE;

        // get the value of parameters
        CRL_Interval = par("CRL_Interval");
        beacon_Interval = par("beacon_Interval").doubleValue();
        I2V_tho = par("I2V_tho");

        if(CRL_Interval <= 0)
            error("value for RSU_ONLY is incorrect !!");

        if(beacon_Interval <= 0)
            error("value for beacon_Interval is incorrect !!");

        if(I2V_tho < 0)
            error("value for I2V_tho is incorrect !!");


//        // RSUs broadcast CRL pieces periodically
            //In RSU only and C2CEpidemic RSU sends periodic CRL
            Timer2 = new cMessage( "Timer_CRL_Interval_RSU", Timer_CRL_Interval_RSU );
            scheduleAt(simTime() + dblrand() * 10, Timer2);
    }
}


void RSUAppl::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        if(msg->getKind()==Timer_CRL_Interval_RSU)
        {
            sendPeriodicCRL();
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
        // receive a beacon from vehicle
        if(msg->getKind() == Msg_Beacon_V)
        {
            recieveBeacon(msg);
        }
        else delete msg;  // if receive a CRL from V, delete it
    }
    else if(msg->getArrivalGateId()==dataFromCA)
    {
        recieveCRL(msg);
    }
    else
    {
        EV << "Unknown message! -> delete, kind: " << msg->getKind() << endl;
        delete msg;
    }
}




// Receive CRL from the CA
void RSUAppl::recieveCRL(cMessage *msg)
{
    CRL_Piece *m = static_cast<CRL_Piece *>(msg);
    if (m == NULL) return;

    if ( getEnvir()->isGUI() )
    {
        nodePtr->bubble("received a piece from CA");

        EV << "*** " << nodePtr->getFullName() << " received a CRL piece from CA:" << endl;

        EV << "Msg name: " << m->getName() << endl;
        EV << "CRL Version: " << m->getCRLversion() << endl;
        EV << "Timestamp: " << m->getTimestamp() << endl;
        EV << "Sequence Number: " << m->getSeqNo() << endl;
        EV << "CA ID: " << m->getCAid() << endl;
        EV << "Padding: " << m->getPad() << endl << endl;
    }

    // save this CRL piece for future periodic CRL sending
    PiecesCRLfromCA.push_back( m );
}


void RSUAppl::sendPeriodicCRL()
{
    if ( getEnvir()->isGUI() )
    {
        nodePtr->bubble("Sending periodic CRL");
        EV << "**** " << nodePtr->getFullName() << " is sending periodic CRL." << endl;
    }

    sendCRLs();

    // reset to false
    // AnyoneNeedCRL = false;
    EV<<"sending periodic CRL\n";
    scheduleAt(simTime() + CRL_Interval, Timer2);
}


// broadcast CRL pieces of the RSU
void RSUAppl::sendCRLs()
{
    if ( getEnvir()->isGUI() )
    {
        EV << "**** " << nodePtr->getFullName();
        EV << ": sending pieces on channel " ;//<< nicClassPtr->getChannel() << " --> ";
    }

    double time = 0;
    int counter = 0;

    for(unsigned int i=0; i<PiecesCRLfromCA.size(); i++)
    {
        CRL_Piece *pkt = PiecesCRLfromCA[forCounter]->dup();

        pkt->setName(nodePtr->getFullName());
        pkt->setKind(Msg_CRL_RSU);
        pkt->setBitLength(18400);
        pkt->setChannelNumber(static_cast<int>(veins::Channel::cch));
        pkt->setSender(mac->getMACAddress());
        pkt->setRecipientAddress(-1);
        // we have time constrain (tho).
        // we have to calculate how many CRL pieces can we send in tho.
        // the calculation considers size of frame and data rate.
        // I2V_tho == 0 means we don't have any restriction.
//        if(I2V_tho != 0)
//        {
//            // transmission time for a single frame
//            // (32 bits header for network layer and 192 bits for data-link layer)
//            double TxTime = ( pkt->getBitLength() + 32 + 272 + 192) / bitrate;
//            time = time + TxTime;
//            if(time >= I2V_tho)
//            {
//                delete pkt;
//                break;
//            }
//        }

        counter++;
        EV << pkt->getSeqNo() << ", ";

        // send the pkt
        send(pkt, lowerLayerOut);

        forCounter++;
        if( forCounter == PiecesCRLfromCA.size() ) forCounter = 0;
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLfromCA.size() << " are sent." << endl;

    // if one or more pieces sent
    if(counter > 0)
    {
//        simsignal_t Signal_Broadcast_RSU = registerSignal("Broadcast_RSU");
//        nodePtr->emit(Signal_Broadcast_RSU, 1);
    }
}


void RSUAppl::recieveBeacon(cMessage *msg)
{
    Beacon *m = static_cast<Beacon *>(msg);
    if (m == NULL) return;

    if(getEnvir()->isGUI())
    {
        EV << "*** " << nodePtr->getFullName() << " received a beacon from " << m->getNodeName() << endl;
        nodePtr->bubble("received a beacon");

        EV << "*** Extracting beacon information: " << endl;

        EV << "source Add: " << m->getSrcAddr() << endl;
        EV << "destination Add: " << m->getDestAddr() << endl;
        EV << "node name: " << m->getNodeName() << endl;
        EV << "position (x,y): " << m->getPositionX() << ", " << m->getPositionY() << endl;
        EV << "speed: " << m->getSpeed() << endl;
        EV << "number of pieces: " << m->getCRL_Piece_No() << endl;
        EV << "CRL channel: " << m->getCRLchannel()  << endl;
        EV << "Need CRL: " << m->getNeedCRL() << endl;
        EV << "Range Start: " << m->getRangeS() << endl;
        EV << "Range End: " << m->getRangeE() << endl;
    }


    delete msg;
}




void RSUAppl::finish()
{


}


RSUAppl::~RSUAppl()
{
        cancelAndDelete(Timer2);

    for(unsigned int i=0; i<PiecesCRLfromCA.size(); i++)
    {
        delete PiecesCRLfromCA[i];
    }
}

