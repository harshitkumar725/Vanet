/*
 * AppRSU.h
 *
 *  Created on: Jun 12, 2020
 *      Author: thimus
 */

#ifndef DEMO_APPRSU_H_
#define DEMO_APPRSU_H_

#include "AppBase.h"
using namespace veins;
using namespace omnetpp;

class AppRSU : public AppBase{
public:
    void initialize(int stage) override;
    void finish() override;

protected:

    void handleSelfMsg(cMessage* msg) override;
    void handleLowerMsg(cMessage* msg)override;
};

#endif /* DEMO_APPRSU_H_ */
