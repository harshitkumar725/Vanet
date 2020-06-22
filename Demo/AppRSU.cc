/*
 * AppRSU.cc
 *
 *  Created on: Jun 12, 2020
 *      Author: thimus
 */

#include "AppRSU.h"
using namespace veins;

Define_Module(AppRSU);

void AppRSU::initialize(int stage)
{
    AppBase::initialize(stage);
    if (stage == 0) {
        // Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
    }
    else if (stage == 1) {
        // Initializing members that require initialized other modules goes here
    }
}

void AppRSU::handleSelfMsg(cMessage* msg)
{
    switch (msg->getKind()) {
    case SEND_BEACON_EVT: {
        EV<<"Generating Piece List on SEND_BEACON_EVENT Self message\n";
        Demo_Piece_list* bsm = new Demo_Piece_list();
        bsm->setRecipientAddress(-1);
        bsm->setChannelNumber(static_cast<int>(Channel::cch));
        bsm->setSerial(1567);
        bsm->setList("My list goes here");
        bsm->setBitLength(headerLength);
        bsm->setSenderAddress(mac->getMACAddress());
        sendDown(bsm);
        EV<<"Sending Piece List\n";
        //scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        break;
    }
    case SEND_WSA_EVT: {
//        DemoServiceAdvertisment* wsa = new DemoServiceAdvertisment();
//        populateWSM(wsa);
//        sendDown(wsa);
//        scheduleAt(simTime() + wsaInterval, sendWSAEvt);
        break;
    }
    default: {
        if (msg) EV_WARN << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
        break;
    }
    }
}
void AppRSU::handleLowerMsg(cMessage* msg)
{

    BaseFrame1609_4* wsm = dynamic_cast<BaseFrame1609_4*>(msg);
    ASSERT(wsm);

    if (Demo_Piece_list* bsm = dynamic_cast<Demo_Piece_list*>(wsm)) {
        EV<<"Piece List message is recieved\n";
        EV<<"Serial: "<<bsm->getSerial()<<endl;
        EV<<"List: "<<bsm->getList()<<endl;
    }
    else
    EV<<"Wrong message is recieved";

    delete (msg);
}

void AppRSU::finish()
{
    AppBase::finish();
    // statistics recording goes here
}
