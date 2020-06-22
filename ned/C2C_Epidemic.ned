package ned;

simple C2C_Epidemic extends BasicAppl
{    
    parameters:
        @class(C2C_Epidemic);
   	  double speed @unit(mps);        
        double beacon_Interval @unit(s);
        double plist_Interval @unit(s);
        double message_Lost;
        
    gates:       

}