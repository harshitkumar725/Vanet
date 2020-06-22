/*
 * SCRLE.h
 *
 *  Created on: Jun 16, 2020
 *      Author: thimus
 */

#ifndef APPLICATION_SCRLE_H_
#define APPLICATION_SCRLE_H_
#include "BasicAppl.h"
#include <boost/tokenizer.hpp>
class SCRLE : public BasicAppl{

public:
    SCRLE();
    virtual ~SCRLE();
    virtual void initialize(int);
       virtual int numInitStages() const
       {
           return 2;
       }
       virtual void handleMessage(cMessage *);
       virtual void finish();
       // defined public to be used by VAppl_C2C_Epidemic_Ideal
       void recieveCRL(cMessage *);
       std::vector<CRL_Piece *> PiecesCRLcollected;

     protected:

       //for recording CRL pieces with respect to time
       cOutVector No_CRL_Piece;

       // parameters
       int nodeID;
       double beacon_Interval;
       double V2V_Wait_Beacon;
       double V2V_Wait_CRL;
       double plist_Interval;
       double beacon_Listen_Interval;
       // variables
       int CRL_Piece_No;
       int CRLChannel;

       cMessage *Timer1;   // for sending beacon messages
       cMessage *Timer2;
       double message_Lost;


       int *broadcastMask;   // an array of int
       Beacon* selectedBeacon;

       //parameters for statistics
       uint32_t dup_msg;
       uint32_t org_msg;
       uint32_t plist_msg;
       simtime_t all_pieces;

        // is used in sendCRLs method
       int totFrames;
       unsigned int forCounter;

       enum VAppl_States
       {
           // states of VAppl
           STATE_IDLE,  // 0
           STATE_BROADCAST_BEACON_SENDING,
           STATE_BROADCAST_BEACON_SEND,
           STATE_BEACON_RECIEVING,
           STATE_PLIST_iNTERVAL,
           // states for V2V algorithms
           STATE_UNICAST_BEACON_SENDING,
           STATE_UNICAST_BEACON_SEND,
           STATE_WAIT_FOR_UNICAST_BEACON,
           STATE_WAITING_CRL,
           STATE_WAITING_CRL_2,
           STATE_WAIT_FOR_MORE_BEACONS,
           STATE_SENDING_CRLS,
           STATE_CRL_LIST_SEND
       };

       VAppl_States state;  // state of the vehicle node

      TraCIMobility* traci;
   //    static const simsignalwrap_t mobilityStateChangedSignal;

       void Send_CRL_List_Msg(int,veins::LAddress::L2Type);
       void sendBeacon(veins::LAddress::L2Type, bool);
       void recievePlist(cMessage* msg);

     private:
       // parameters
       int ourSerial;//for tracking serial no of our plist
       double speed;
       int getStatus();
       void recieveBeacon(cMessage *);
       void updateText();
       double round(double);
       void CRLreconstruction();
       void CRLreconstruction_NoErasure();
       void sendCRLsMask(veins::LAddress::L2Type);
       void UpdateMask(Piece_list *);
       void record();
};

#endif /* APPLICATION_SCRLE_H_ */
