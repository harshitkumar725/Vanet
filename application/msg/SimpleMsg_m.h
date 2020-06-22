//
// Generated file, do not edit! Created by nedtool 5.5 from application/msg/SimpleMsg.msg.
//

#ifndef __VEINS_SIMPLEMSG_M_H
#define __VEINS_SIMPLEMSG_M_H

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
 * Class generated from <tt>application/msg/SimpleMsg.msg:9</tt> by nedtool.
 * <pre>
 * packet SimpleMsg extends BaseFrame1609_4
 * {
 *     string payload;
 *     string signature;
 *     double certLTfrom;
 *     double certLTto;
 * }
 * </pre>
 */
class SimpleMsg : public ::veins::BaseFrame1609_4
{
  protected:
    ::omnetpp::opp_string payload;
    ::omnetpp::opp_string signature;
    double certLTfrom;
    double certLTto;

  private:
    void copy(const SimpleMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SimpleMsg&);

  public:
    SimpleMsg(const char *name=nullptr, short kind=0);
    SimpleMsg(const SimpleMsg& other);
    virtual ~SimpleMsg();
    SimpleMsg& operator=(const SimpleMsg& other);
    virtual SimpleMsg *dup() const override {return new SimpleMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getPayload() const;
    virtual void setPayload(const char * payload);
    virtual const char * getSignature() const;
    virtual void setSignature(const char * signature);
    virtual double getCertLTfrom() const;
    virtual void setCertLTfrom(double certLTfrom);
    virtual double getCertLTto() const;
    virtual void setCertLTto(double certLTto);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const SimpleMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, SimpleMsg& obj) {obj.parsimUnpack(b);}

} // namespace veins

#endif // ifndef __VEINS_SIMPLEMSG_M_H

