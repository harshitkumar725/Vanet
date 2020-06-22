/*
 * AppV.h
 *
 *  Created on: Jun 12, 2020
 *      Author: thimus
 */

#ifndef DEMO_APPV_H_
#define DEMO_APPV_H_

#include "AppBase.h"
using namespace veins;
using namespace omnetpp;

class AppV : public AppBase{
public:
public:
    void initialize(int stage) override;
    void finish() override;

protected:
    cMessage* Timer1;
    void handleSelfMsg(cMessage* msg) override;
    void handleLowerMsg(cMessage* msg)override;
};

#endif /* DEMO_APPV_H_ */
