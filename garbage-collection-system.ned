
import inet.mobility.contract.IMobility;
import inet.mobility.single.TurtleMobility;
import inet.visualizer.common.IntegratedVisualizer;

module MobileHost
{
    parameters:
        string configTitle;
        @display("t=host-car");
    gates:
        input cloudIn;
        input canIn;
        input can2In;
        output cloudOut;
        output canOut;
        output can2Out;

    submodules:
        mobility: TurtleMobility;
        host: GarbageTruck {
            parameters:
                configTitle = parent.configTitle;
                @display("p=200,200");
        }

    connections allowunconnected:
        cloudIn --> host.cloudIn;
        canIn --> host.canIn;
        can2In --> host.can2In;

        host.cloudOut --> cloudOut;
        host.canOut --> canOut;
        host.can2Out --> can2Out;
}



simple GarbageTruck
{
    parameters:
        @display("i=device/pc");
        string configTitle; //NEEDED TO UPDATE TITLE. A LITTLE HACKY ATM BUT WORKS FOR NOW
        @display("t=host-ca");
    gates:
        input cloudIn;
        input canIn;
        input can2In;
        output cloudOut;
        output canOut;
        output can2Out;
}

simple TrashCan
{
    parameters:
        @display("i=device/pc");
        string configTitle; //NEEDED TO UPDATE TITLE. A LITTLE HACKY ATM BUT WORKS FOR NOW
        int canId;
    gates:
        input cloudIn;
        input truckIn;
        output cloudOut;
        output truckOut;
}

simple CloudNode
{
    parameters:
        @display("i=device/server");
        string configTitle;
    gates:
        input truckIn;
        input canIn;
        input can2In;
        output truckOut;
        output canOut;
        output can2Out;
}



network GarbageCollectionSystem
{
    parameters:
        string configTitle;
        int numHosts = 1;
        @figure[upper_street1](type=rectangle; pos=160,200; size=880,3; fillColor=black; fillOpacity=1);
        @figure[upper_street2](type=rectangle; pos=340,350; size=700,3; fillColor=black; fillOpacity=1);
        @figure[lower_street1](type=rectangle; pos=340,590; size=700,3; fillColor=black; fillOpacity=1);
        @figure[lower_street2](type=rectangle; pos=160,750; size=880,3; fillColor=black; fillOpacity=1);

        @figure[vertical_street1](type=rectangle; pos=160,200; size=3,550; fillColor=black; fillOpacity=1);
        @figure[vertical_street2](type=rectangle; pos=340,350; size=3,240; fillColor=black; fillOpacity=1);

        @figure[Title](type=text; pos=1500,15; text="Initial Title");

        @display("bgb=2500,870");

    submodules:
        can: TrashCan {
            parameters:
                canId = 1;
                @display("p=380,120;i=block/bucket;r=200");
        }
        anotherCan: TrashCan {
            parameters:
                canId = 2;
                @display("p=450,510;i=block/bucket;r=200");
        }
        host[numHosts]: MobileHost {
            parameters:
                configTitle = parent.configTitle;
                @display("p=910,900;i=block/wheelbarrow;r=90;t=-");
                @display("t=host-car");
        }
        cloud: CloudNode {
            @display("p=1200,460;i=block/encap;r=850");
        }
        visualizer: IntegratedVisualizer {
            @display("p=1100,30");
        }
    connections:
        can.truckOut --> {  delay = 100ms; } --> host[0].canIn;
        can.cloudOut --> {  delay = 100ms; } --> cloud.canIn;
        can.truckIn <-- {  delay = 100ms; } <-- host[0].canOut;
        can.cloudIn <-- {  delay = 100ms; } <-- cloud.canOut;

        anotherCan.truckOut --> {  delay = 100ms; } --> host[0].can2In;
        anotherCan.cloudOut --> {  delay = 100ms; } --> cloud.can2In;
        anotherCan.truckIn <-- {  delay = 100ms; } <-- host[0].can2Out;
        anotherCan.cloudIn <-- {  delay = 100ms; } <-- cloud.can2Out;

        host[0].cloudOut --> {  delay = 100ms; } --> cloud.truckIn;
        host[0].cloudIn <-- {  delay = 100ms; } <-- cloud.truckOut;
}

