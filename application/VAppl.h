

#ifndef APPLICATION_VAPPL_H_
#define APPLICATION_VAPPL_H_
#include "BasicAppl.h"

class VAppl : public BasicAppl{

  public:
    VAppl();
    virtual ~VAppl();
    virtual void finish();
    virtual void initialize(int);
    virtual int numInitStages() const
    {
        return 3;
    }
    virtual void handleMessage(cMessage *);

    // defined public to be used by VAppl_C2C_Epidemic_Ideal
    void recieveCRL(cMessage *);
    std::vector<CRL_Piece *> PiecesCRLcollected;

  protected:
    // parameters
    int nodeID;
    double beacon_Interval;
    double V2V_tho;
    double V2V_Wait_Beacon;
    bool V2V_EnableMultiChannel;
    double V2V_Wait_CRL;

    // variables
    int CRL_Piece_No;
    int CRLChannel;
    std::string EncounteredRSU_old;

    // for MPB method
    bool AnyPieceReceived;

    // for ICE
    int rangeStart;
    int rangeEnd;

    cMessage *Timer1;   // for sending beacon messages
    cMessage *Timer2;   // for sending Magic_Req signal
    cMessage *Timer3;   // for sending msg in compromised node
    cMessage *Timer_Verify;

     // is used in sendCRLs method
    int totFrames;
    unsigned int forCounter;

    enum VAppl_States
    {
        // states of VAppl
        STATE_IDLE,  // 0
        STATE_BROADCAST_BEACON_SENDING,
        STATE_BROADCAST_BEACON_SEND,

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


    void sendBeacon(veins::LAddress::L2Type, bool);
    virtual void Run_V2V_Algorithm(Beacon *) { };
    virtual void updateRange() { };  // implemented only in ICE

  private:
    // parameters
    double speed;
    int MagicCarsRatio;
    double MagicReqTime;
    bool CRLrecons;
    bool CompromisedEnable;
    double CompromisedMsgSendTime;

    enum VehicleType
    {
        privateV,
        public_transport,
        public_emergency,
        public_authority,
        public_army,
        vip,
        ignoring,
        passenger,
        hov,
        taxi,
        bus,
        delivery,
        transport,
        lightrail,
        cityrail,
        rail_slow,
        rail_fast,
        motorcycle,
        bicycle,
        pedestrian,
    };

    VehicleType vType;  // type of vehicle
    static bool VehicleTypeSet;

    bool IsCompromised;
    static bool runCompromised;
    bool receiveCompromisedMsg;

    void recieveBeacon(cMessage *);

    void sendMsg();
    bool IsValid(cMessage *);
    void receiveMsg(cMessage *);

    void updateText();

    void setCompromised();



    void CRLreconstruction();
    void CRLreconstruction_NoErasure(); // when erasure code is not used
    void CRLreconstruction_Erasure();  // when erasure code is used
    double round(double);
};


#endif /* APPLICATION_VAPPL_H_ */
