
#include "VAppl.h"
Define_Module(VAppl);


bool VAppl::VehicleTypeSet = false;
bool VAppl::runCompromised = false;


// constructor
VAppl::VAppl()
{


}


void VAppl::initialize(int stage)
{
    BasicAppl::initialize(stage);

    if(stage == 0)
    {
        // get the value of parameters
        nodeID = nodePtr->getId();
        speed = par("speed");
        if(speed < 0)
            error("value for speed is incorrect !!");

        beacon_Interval = par("beacon_Interval").doubleValue();
        if(beacon_Interval <= 0)
            error("value for beacon_Interval is incorrect !!");

        V2V_tho = par("V2V_tho").doubleValue();
        if(V2V_tho < 0)     // V2V_tho can be 0.
            error("value for V2V_tho is incorrect !!");

        V2V_Wait_Beacon = par("V2V_Wait_Beacon").doubleValue();
        if(V2V_Wait_Beacon <= 0)
            error("value for V2V_Wait_Beacon is incorrect !!");

        V2V_Wait_CRL = par("V2V_Wait_CRL").doubleValue();
        if(V2V_Wait_CRL <= 0)
            error("value for V2V_Wait_CRL is incorrect !!");

        CRLrecons = par("CRLrecons").boolValue();

        CompromisedEnable = par("CompromisedEnable").boolValue();

        CompromisedMsgSendTime = par("CompromisedMsgSendTime").doubleValue();
        if(CompromisedMsgSendTime < 0)
            error("value for CompromisedMsgSendTime is incorrect !!");

        if(CompromisedEnable)
            Timer_Verify  = new cMessage( "Timer_Wait_VerifyPS", Timer_Wait_Verify );

        // set variables
        CRL_Piece_No = 0;
        CRLChannel = -1;
        state = STATE_IDLE;
        EncounteredRSU_old = "";

        totFrames = 0;
        forCounter = 0;

        // we assume, all vehicles are private at the start!
        // we will select special vehicles later (in stage == 1).
        vType = privateV;

        // initially none of the vehicles are compromised
        IsCompromised = false;

        receiveCompromisedMsg = false;

        // rangeStart, rangeEnd and unionRange are for ICE
        // are used in sendBeacon method
        rangeStart = 0;
        rangeEnd = totalPieces-1;


        // define and start Timer1 (send broadcast beacon)
        Timer1 = new cMessage( "Timer_Beacon_V", Timer_Beacon_V );
        scheduleAt(simTime() + dblrand() * beacon_Interval, Timer1);
    }
    // check if we are in second stage (stage == 1) of initialization!
    else if(stage == 1)
    {

            traci = TraCIMobilityAccess().get(getParentModule());

        // select a random node in the whole network as compromised
        if(CompromisedEnable && !runCompromised)
        {
            setCompromised();
            runCompromised = true;
        }
    }
    // check if we are in third stage (stage == 2) of initialization.
    else if(stage == 2)
    {

        // if this vehicle is compromised, then send the msg at TimeMsgSend
        if(IsCompromised == true)
        {
            Timer3 = new cMessage( "Timer_Sending_Compromised_Msg", Timer_Sending_Compromised_Msg );
            scheduleAt(simTime() + CompromisedMsgSendTime, Timer3);
        }
    }
}


void VAppl::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        if( msg->getKind() == Timer_Beacon_V )
        {
             state = STATE_BROADCAST_BEACON_SENDING;
            sendBeacon(-1, true);    // -1: broadcast beacon
        }

        else if( msg->getKind() == Timer_Sending_Compromised_Msg )
        {
            sendMsg();
        }
        else
            delete msg;

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
        EV<<"Message is recieved from lower layer\n";
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
            recieveCRL(msg);
        }
        // Receive a CRL message from vehicle
        else if(msg->getKind() == Msg_CRL_V)
        {
            recieveCRL(msg);
        }
        // Receive a compromised msg from vehicle
        else if(msg->getKind() == Msg_Compromised_V)
        {
            //scheduleAt(simTime() + 0.1, Timer_VerifyPS);

            // check validity first.
            bool result = IsValid(msg);

            // if ok, then call receiveMsg(msg);
            if(result)
            {
                receiveMsg(msg);
            }
        }
    }
}

void VAppl::sendBeacon(veins::LAddress::L2Type RecID, bool periodic)
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

   // if ( getEnvir()->isGUI() ) nodePtr->bubble("Sending beacon");

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
    pkt->setRangeS(rangeStart);
    pkt->setRangeE(rangeEnd);

    EV << "**** " << nodePtr->getFullName() << " range start: " << rangeStart;
    EV << ", range end: " << rangeEnd << endl;


    pkt->setChannelNumber(static_cast<int>(veins::Channel::cch));

    // ###################################################
    // ###################################################

    pkt->setBitLength(1000);
    pkt->setRecipientAddress(RecID);

    send(pkt, lowerLayerOut);

    if(periodic) scheduleAt(simTime() + beacon_Interval, Timer1);
}


// receive a beacon from an RSU or a vehicle
void VAppl::recieveBeacon(cMessage *msg)
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
        EV << "number of pieces: " << m->getCRL_Piece_No() << endl;
        EV << "CRL channel: " << m->getCRLchannel()  << endl;
        EV << "Need CRL: " << m->getNeedCRL() << endl;
        EV << "Range Start: " << m->getRangeS() << endl;
        EV << "Range End: " << m->getRangeE() << endl;
    }
    nodePtr->bubble(applType);
    // if we are running a V2V algorithm.
    // each algorithm has its own Run_V2V_Algorithm method.
    if( strcmp (applType, "VAppl") != 0 )
    {

        Run_V2V_Algorithm(m);
    }

    delete msg;
}


// Receive a CRL from RSU or from another vehicle
void VAppl::recieveCRL(cMessage *msg)
{
    // if any pieces is received (new or duplicate) from RSU or nearby vehicle, we set this flag.
    // is used in MPB and ICE method.
    AnyPieceReceived = true;

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
//            nodePtr->bubble("duplicate copy !!");
            EV << " --> duplicate copy!!" << endl;
        }



        delete msg;  // delete the message
    }
    // no copy is found in PiecesCRLcollected
    else
    {
        if ( getEnvir()->isGUI() )
        {
//            nodePtr->bubble("received a new CRL piece");
            EV << " --> save in array." << endl;
        }

        PiecesCRLcollected.push_back(m);  // store the CRL internally

        CRL_Piece_No++;  // Increment the number of CRLs received so far

        if(getEnvir()->isGUI())
        {
            EV << "*** now we have " << CRL_Piece_No << " CRL pieces." << endl;
            updateText();  // update the text in top of the V nodes.
        }

        // update the rangeStart, rangeEnd variables (only used in ICE)
        updateRange();

        // check to see if we can re-construct the CRL message, with this CRL piece.
        CRLreconstruction();

        // send signal to statistics to report number of CRLs received so far.
//        simsignal_t Signal_NoCRLs_V = registerSignal("NoCRLs_V");
//        nodePtr->emit(Signal_NoCRLs_V, CRL_Piece_No);
    }
}


// compromised node creates and broadcasts a msg
void VAppl::sendMsg()
{
    SimpleMsg *pkt = new SimpleMsg(nodePtr->getFullName(), Msg_Compromised_V);

    pkt->setPayload("accident happened in x y");
    pkt->setSignature("signature");
    pkt->setCertLTfrom(0);
    pkt->setCertLTto(Pseudonym_lifeTime);

    pkt->setBitLength(1000);
    pkt->setChannelNumber(static_cast<int>(veins::Channel::cch));
    pkt->setRecipientAddress(-1);
    send(pkt, lowerLayerOut);

    EV << "*** " << nodePtr->getFullName() << " sent a false message." << endl;


}


bool VAppl::IsValid(cMessage *msg)
{
    // step 1: Pseudonym verify: we check the lifetime of certificate here!

    SimpleMsg *m = static_cast<SimpleMsg *>(msg);
    if (m == NULL)
        return false;

    if( simTime().dbl() >= m->getCertLTfrom() && simTime().dbl() <= m->getCertLTto() )
    {
        return true;
    }
    else
    {
        EV << "*** " << nodePtr->getFullName() << " discards a msg:" << endl;

        EV << "Msg name: " << m->getName() << endl;
        EV << "Payload: " << m->getPayload() << endl;
        EV << "Signature: " << m->getSignature() << endl;
        EV << "Certificate Life-time from: " << m->getCertLTfrom() << endl;
        EV << "Certificate Life-time to: " << m->getCertLTto() << endl;
        EV << endl;

        return false;
    }

    // step 2: msg verify
}


void VAppl::receiveMsg(cMessage *msg)
{
    SimpleMsg *m = static_cast<SimpleMsg *>(msg);
    if (m == NULL) return;

    if(IsCompromised || receiveCompromisedMsg)
        return;

    EV << "*** " << nodePtr->getFullName() << " received a msg:" << endl;

    EV << "Msg name: " << m->getName() << endl;
    EV << "Payload: " << m->getPayload() << endl;
    EV << "Signature: " << m->getSignature() << endl;
    EV << "Certificate Life-time from: " << m->getCertLTfrom() << endl;
    EV << "Certificate Life-time to: " << m->getCertLTto() << endl;
    EV << endl;

    if ( getEnvir()->isGUI() )
    {
//        nodePtr->bubble("msg received");
        nodePtr->getDisplayString().updateWith("i2=status/excl");
    }

    simsignal_t Signal_Msg_received_V = registerSignal("Msg_received_V");
    nodePtr->emit(Signal_Msg_received_V, 0);

    // re-broadcast this message
    send(msg, lowerLayerOut);

    receiveCompromisedMsg = true;
}


void VAppl::updateText()
{
    char buf[15];
    sprintf(buf, "pieces: %d", CRL_Piece_No);

    nodePtr->getDisplayString().setTagArg("t", 0, buf);
}




// select a random node and set it as compromised
void VAppl::setCompromised()
{
    // get number of vehicles.
    int numV = nodePtr->getParentModule()->par("numV");
    int rnd = intrand( numV );
    // get a pointer to this vehicle module.
    cModule *vehiclePtr = getSimulation()->getSystemModule()->getSubmodule("V", rnd);
    // get a pointer to the application sub-module
    cModule *appPtr = vehiclePtr->getSubmodule("appl");
    // get a pointer to the application class
    VAppl *classPtr = check_and_cast<VAppl *>(appPtr);
    classPtr->IsCompromised = true;
    // now change the icon
    if(getEnvir()->isGUI())
        vehiclePtr->getDisplayString().updateWith("i2=status/stop");
    EV << "*** " << vehiclePtr->getFullName() << " is compromised." << endl;
}



void VAppl::CRLreconstruction()
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

        if(getEnvir()->isGUI() && !IsCompromised)
            nodePtr->getDisplayString().updateWith("i2=status/checkmark,green");

        // do the real CRL reconstruction
        if(CRLrecons)
            CRLreconstruction_NoErasure();
    }
}


void VAppl::CRLreconstruction_NoErasure()
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



double VAppl::round(double r)
{
    double n = (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);

    return abs(n);
}


void VAppl::finish()
{


}


VAppl::~VAppl()
{
    cancelAndDelete(Timer1);

     for(unsigned int i = 0; i < PiecesCRLcollected.size(); i++)
     {

             delete PiecesCRLcollected[i];

     }

     if(runCompromised==true)
     {
         cancelAndDelete(Timer3);
     }
}
