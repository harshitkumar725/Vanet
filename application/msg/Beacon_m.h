//
// Generated file, do not edit! Created by nedtool 5.5 from application/msg/Beacon.msg.
//

#ifndef __VEINS_BEACON_M_H
#define __VEINS_BEACON_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// cplusplus {{
#include "veins/modules/messages/BaseFrame1609_4_m.h"
// }}


namespace veins {

/**
 * Class generated from <tt>application/msg/Beacon.msg:12</tt> by nedtool.
 * <pre>
 * packet Beacon extends BaseFrame1609_4
 * {
 *     LAddress::L2Type destAddr = -1;   // destination address
 *     LAddress::L2Type srcAddr = -1;    // source address
 *     string NodeName;
 *     double positionX;
 *     double positionY;
 *     double speed;
 *     int myResult = 0;
 *     bool NeedCRL;
 *     int CRL_Piece_No;
 *     int rangeS;
 *     int rangeE;
 *     int CRLchannel;
 * }
 * </pre>
 */
class Beacon : public ::veins::BaseFrame1609_4
{
  protected:
    LAddress::L2Type destAddr;
    LAddress::L2Type srcAddr;
    ::omnetpp::opp_string NodeName;
    double positionX;
    double positionY;
    double speed;
    int myResult;
    bool NeedCRL;
    int CRL_Piece_No;
    int rangeS;
    int rangeE;
    int CRLchannel;

  private:
    void copy(const Beacon& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Beacon&);

  public:
    Beacon(const char *name=nullptr, short kind=0);
    Beacon(const Beacon& other);
    virtual ~Beacon();
    Beacon& operator=(const Beacon& other);
    virtual Beacon *dup() const override {return new Beacon(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual LAddress::L2Type& getDestAddr();
    virtual const LAddress::L2Type& getDestAddr() const {return const_cast<Beacon*>(this)->getDestAddr();}
    virtual void setDestAddr(const LAddress::L2Type& destAddr);
    virtual LAddress::L2Type& getSrcAddr();
    virtual const LAddress::L2Type& getSrcAddr() const {return const_cast<Beacon*>(this)->getSrcAddr();}
    virtual void setSrcAddr(const LAddress::L2Type& srcAddr);
    virtual const char * getNodeName() const;
    virtual void setNodeName(const char * NodeName);
    virtual double getPositionX() const;
    virtual void setPositionX(double positionX);
    virtual double getPositionY() const;
    virtual void setPositionY(double positionY);
    virtual double getSpeed() const;
    virtual void setSpeed(double speed);
    virtual int getMyResult() const;
    virtual void setMyResult(int myResult);
    virtual bool getNeedCRL() const;
    virtual void setNeedCRL(bool NeedCRL);
    virtual int getCRL_Piece_No() const;
    virtual void setCRL_Piece_No(int CRL_Piece_No);
    virtual int getRangeS() const;
    virtual void setRangeS(int rangeS);
    virtual int getRangeE() const;
    virtual void setRangeE(int rangeE);
    virtual int getCRLchannel() const;
    virtual void setCRLchannel(int CRLchannel);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Beacon& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Beacon& obj) {obj.parsimUnpack(b);}

} // namespace veins

#endif // ifndef __VEINS_BEACON_M_H

