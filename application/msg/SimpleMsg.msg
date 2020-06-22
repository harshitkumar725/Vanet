cplusplus {{
#include "veins/modules/messages/BaseFrame1609_4_m.h"
}}

namespace veins;

class BaseFrame1609_4;
packet SimpleMsg extends BaseFrame1609_4
{
    string payload;
    string signature;
    double certLTfrom;
    double certLTto;
}
