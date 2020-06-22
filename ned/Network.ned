
package ned;

import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.base.modules.BaseWorldUtility;
import org.car2x.veins.modules.mobility.traci.TraCIScenarioManager*;
import org.car2x.veins.modules.obstacle.ObstacleControl;
import org.car2x.veins.modules.world.annotations.AnnotationManager;
import org.car2x.veins.nodes.Car;

network Network
{
    parameters:
        int numCA;
        int numRSU;
        int numV;   // total number of vehicles in the network        
        int headerLength = default(88);
        string applType;
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=346,204");

    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 100Mbps;
            delay = 10us;
            @display("ls=red");
        }
    submodules:
        obstacles: ObstacleControl {
            @display("p=272,50");
        }
        annotations: AnnotationManager {
            @display("p=201,24");
        }
        connectionManager: ConnectionManager {
            parameters:
                @display("p=128,39;i=abstract/multicast");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=53,24;i=misc/globe");
        }
        manager: TraCIScenarioManagerLaunchd {
            parameters:
                @display("p=512,128");
        }
        CA[numCA]: CANode {
            parameters:
                @display("p=66,106;i=old/pc");
        }
        RSU[numRSU]: RSUNode {
            parameters:
                @display("p=144,156;i=device/antennatower");
        }
        V[numV]: VNode {
            @display("p=251,156;i=veins/node/car_s");
        }
    connections allowunconnected:
        for i=0..(numRSU-1) {
            CA[0].dataToRSU++ --> C --> RSU[i].dataFromCA;
        }
}
