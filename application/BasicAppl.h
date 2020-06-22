/*
 * BasicAppl.h
 *
 *  Created on: May 3, 2020
 *      Author: thimus
 */

#ifndef APPLICATION_BASICAPPL_H_
#define APPLICATION_BASICAPPL_H_

#include <map>
#include <omnetpp.h>
#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mac/ieee80211p/DemoBaseApplLayerToMac1609_4Interface.h"
#include "veins/base/connectionManager/ChannelAccess.h"
#include "veins/base/utils/FindModule.h"
#include "ExtraClasses.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/utility/Consts80211p.h"




//message classes
#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "msg/Beacon_m.h"
#include "msg/CRL_Piece_m.h"
#include "msg/SimpleMsg_m.h"
#include "msg/Piece_list_m.h"


using namespace omnetpp;
using veins::BaseApplLayer;
using veins::DemoBaseApplLayerToMac1609_4Interface;
using veins::FindModule;
using veins::BaseFrame1609_4;
using veins::Beacon;
using veins::CRL_Piece;
using veins::SimpleMsg;
using veins::Piece_list;

using veins::AnnotationManager;
using veins::AnnotationManagerAccess;
using veins::TraCICommandInterface;
using veins::TraCIMobility;
using veins::TraCIMobilityAccess;


class BasicAppl : public BaseApplLayer {

public:
    BasicAppl();
    ~BasicAppl();
    void initialize(int stage) override;
    void finish() override;

    enum MessageType
    {
        // Self Messages
        Timer_Initial_Wait_CA,
        Timer_Wait_Tx,
        Timer_CRL_Interval_RSU,
        Timer_Beacon_RSU,
        Timer_Beacon_V,
        Timer_Wait_Beacon_V,
        Timer_Wait_CRL_Over_V,
        Timer_Wait_CRL_Over_V_2,
        Timer_Magic_Req,
        Timer_Statistics_SnapShot,
        Timer_Wait_PList,
        Timer_Sending_Compromised_Msg,
        Timer_Wait_Verify,

        // Messages

        Msg_CRL_CA,
        Msg_CRL_RSU,
        Msg_CRL_V,
        Msg_Compromised_V,
        Msg_Pieces_list,
        Msg_Beacon_V,
        Msg_Beacon_RSU,
    };

protected:
    cModule *nodePtr;
    int nodeId;
    DemoBaseApplLayerToMac1609_4Interface* mac;
    const char *applType;
    int headerLength;

    //members used by other nodes
    static int NoSegments;
    static double Pseudonym_lifeTime;
    static int totalPieces;
};

#endif /* APPLICATION_BASICAPPL_H_ */
