/*
 * AppV.cc
 *
 *  Created on: Jun 12, 2020
 *      Author: thimus
 */

#include "AppV.h"
using namespace omnetpp;
Define_Module(AppV);

void AppV::initialize(int stage)
{
    AppBase::initialize(stage);
    if (stage == 0) {
        // Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
    }
    else if (stage == 1) {
        // Initializing members that require initialized other modules goes here
        Timer1 = new cMessage("Timer1",1);
        scheduleAt(simTime() + 1, Timer1);
    }
}

void AppV::handleSelfMsg(cMessage* msg)
{
    if(msg->getKind()==1)
    {

        Demo_Piece_list* bsm = new Demo_Piece_list();
        bsm->setRecipientAddress(-1);
        bsm->setChannelNumber(static_cast<int>(Channel::cch));
        bsm->setSerial(123);
        bsm->setList("My list goes here");
        bsm->setBitLength(headerLength);
        bsm->setSenderAddress(mac->getMACAddress());
        sendDown(bsm);
    }
    delete msg;
}
void AppV::handleLowerMsg(cMessage* msg)
{

    BaseFrame1609_4* wsm = dynamic_cast<BaseFrame1609_4*>(msg);
    ASSERT(wsm);

    if (Demo_Piece_list* bsm = dynamic_cast<Demo_Piece_list*>(wsm)) {
        EV<<"Piece List message is recieved\n";
        EV<<"Serial: "<<bsm->getSerial()<<endl;
        EV<<"List: "<<bsm->getList()<<endl;
        std::cout<<FindModule<>::findHost(this)->getId()<<" mac Address "<< mac->getMACAddress()<<" recieves a message from "<< bsm->getSenderAddress()<<endl;


        Demo_Piece_list* bsm1 = new Demo_Piece_list();
        bsm1->setRecipientAddress(bsm->getSenderAddress());
        bsm1->setChannelNumber(static_cast<int>(Channel::cch));
        bsm1->setSerial(123);
        bsm1->setList("My list goes here");
        bsm1->setBitLength(headerLength);
        bsm1->setSenderAddress(mac->getMACAddress());
        sendDown(bsm1);
        std::cout<<FindModule<>::findHost(this)->getId()<<" mac Address "<< mac->getMACAddress()<<" is sending  a message to "<< bsm->getSenderAddress()<<endl;

    }
    else
    EV<<"Wrong message is recieved";

    delete (msg);
}

void AppV::finish()
{
    AppBase::finish();
    // statistics recording goes here
}

