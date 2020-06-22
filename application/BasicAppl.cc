/*
 * BasicAppl.cpp
 *
 *  Created on: May 3, 2020
 *      Author: thimus
 */

#include "BasicAppl.h"


Define_Module(BasicAppl);



int BasicAppl::NoSegments;
double BasicAppl::Pseudonym_lifeTime;
int BasicAppl::totalPieces;
BasicAppl::BasicAppl() {
}

BasicAppl::~BasicAppl() {
}

void BasicAppl::initialize(int stage)
{
    if(stage == 0)
    {
        BaseApplLayer::initialize(stage);

        // get the ptr of the current module (NodeCA or NodeRSU or NodeV)
        nodePtr = FindModule<>::findHost(this);
        if(nodePtr == NULL)
            error("can not get a pointer to the module.");

        // get node id
        nodeId = nodePtr->getId();
        EV << "*** node id of " << nodePtr->getFullName() << " is: " << nodeId << endl;

        //pointer to mac
        mac = FindModule<DemoBaseApplLayerToMac1609_4Interface*>::findSubModule(getParentModule());
        ASSERT(mac);

        applType = par("applType");
        headerLength = par("headerLength");
    }
}
void BasicAppl::finish()
{

}
