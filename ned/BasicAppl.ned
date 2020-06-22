
package ned;
import org.car2x.veins.base.modules.IBaseApplLayer;

simple BasicAppl like IBaseApplLayer
{
      parameters: 
          int headerLength @unit("bit");
          string applType;
        @class(BasicAppl);  
    
      gates:
        input lowerLayerIn; // from mac layer
        output lowerLayerOut; // to mac layer
        input lowerControlIn;
        output lowerControlOut;
}