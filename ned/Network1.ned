
package ned;

import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Scenario;

network Network1 extends Scenario
{
    parameters:
        int numCA;
        int numRSU;
        int numV;   // total number of vehicles in the network        
        int headerLength = default(88);
        string applType;
    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 100Mbps;
            delay = 10us;
            @display("ls=red");
        }
    submodules:
        CA[numCA]: CANode {
            parameters:
                @display("p=39,124;i=old/pc");
        }
        rsu[numRSU]: RSUNode {
            parameters:
                @display("p=86,192;i=device/antennatower");
        }
    connections allowunconnected:
        for i=0..(numRSU-1) {
            CA[0].dataToRSU++ --> C --> rsu[i].dataFromCA;
        }

}



