

cplusplus {{
#include "veins/modules/messages/BaseFrame1609_4_m.h"
}}

namespace veins;

class BaseFrame1609_4;
 
class LAddress::L2Type extends void;

packet CRL_Piece extends BaseFrame1609_4
{
    LAddress::L2Type sender = -1;
    int CRLversion;
    int Timestamp;
    int SeqNo;
    int CAid;
    int pad;
    // cArray Array_CRL;
    string payload;
  // pType payload;
}
