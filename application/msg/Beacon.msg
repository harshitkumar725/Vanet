cplusplus {{

#include "veins/modules/messages/BaseFrame1609_4_m.h"

}}

namespace veins;

class BaseFrame1609_4;
  class LAddress::L2Type extends void;
packet Beacon extends BaseFrame1609_4
{
    LAddress::L2Type destAddr = -1;   // destination address
    LAddress::L2Type srcAddr = -1;    // source address
    string NodeName;
	double positionX;
	double positionY;
	double speed;
	int myResult = 0;
	bool NeedCRL;	
	int CRL_Piece_No;
	int rangeS;
	int rangeE;
    int CRLchannel;
}




