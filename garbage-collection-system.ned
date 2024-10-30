
import inet.mobility.contract.IMobility;
import inet.mobility.single.TurtleMobility;
import inet.visualizer.contract.IIntegratedVisualizer;


import inet.mobility.contract.IMobility;
import inet.mobility.single.TurtleMobility;

module MobileHost
{
    parameters:
        string configTitle;
    
    gates:
        input cloudIn;
        input canIn;
        input can2In;
        output cloudOut;
        output canOut;
        output can2Out;
    
    submodules:
        mobility: TurtleMobility {
            @display("p=100,100;is=s");
        }
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

        @figure[upper_street1](type=rectangle; pos=160,200; size=800,3; fillColor=black; fillOpacity=1);
        @figure[upper_street2](type=rectangle; pos=340,350; size=600,3; fillColor=black; fillOpacity=1);
        @figure[lower_street1](type=rectangle; pos=340,590; size=600,3; fillColor=black; fillOpacity=1);
        @figure[lower_street2](type=rectangle; pos=160,750; size=800,3; fillColor=black; fillOpacity=1);

        @figure[vertical_street1](type=rectangle; pos=160,200; size=3,550; fillColor=black; fillOpacity=1);
        @figure[vertical_street2](type=rectangle; pos=340,350; size=3,240; fillColor=black; fillOpacity=1);

        @figure[Title](type=text; pos=1500,5; text="Initial Title");


        //Calculations and statistics part
        @figure[slow_from_smartphone_to_others](type=text; pos=1500,70; text="Slow connection from the smartphone to others (time it takes) =");
        @figure[slow_others_to_smartphone](type=text; pos=1500,90; text="Slow connection from tohers to the smartphone (time it takes) =");
        @figure[fast_from_smartphone_to_others](type=text; pos=1500,110; text="Fast connection from the smartphone to others (time it takes) =");
        @figure[fast_others_to_smartphone](type=text; pos=1500,130; text="Fast connection from tohers to the smartphone (time it takes) =");

        @figure[from_can_to_others](type=text; pos=1500,170; text="Connection from the can to others (time it takes) =");
        @figure[from_others_to_can](type=text; pos=1500,190; text="Connection from others to the can (time it takes) =");

        @figure[from_anotherCan_to_others](type=text; pos=1500,230; text="Connection from the anotherCan to others (time it takes) =");
        @figure[from_others_to_anotherCan](type=text; pos=1500,250; text="Connection from others to the anotherCan (time it takes) =");

        @figure[slow_from_cloud_to_others](type=text; pos=1500,290; text="Slow connection from the Cloud to others (time it takes) =");
        @figure[slow_from_others_to_cloud](type=text; pos=1500,310; text="Slow connection from others to the Cloud (time it takes) =");
        @figure[fast_from_cloud_to_others](type=text; pos=1500,330; text="Fast connection from the Cloud to others (time it takes) =");
        @figure[fast_from_others_to_cloud](type=text; pos=1500,350; text="Fast connection from others to the Cloud (time it takes) =");

        @display("bgb=2200,870");

    submodules:
        can: TrashCan {
            @display("p=250,120;i=block/bucket;r=100");
        }
        anotherCan: TrashCan {
            @display("p=450,510;i=block/bucket;r=100");
        }
        host: MobileHost {
            parameters:
                configTitle = parent.configTitle;
            @display("p=610,135;i=block/wheelbarrow;r=70");
        }
        cloudNode: CloudNode {
            @display("p=1080,440;i=block/table;r=650");
        }
         visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=1100,30");
        }
    connections:
        can.truckOut --> {  delay = 100ms; } --> host.canIn;
        can.cloudOut --> {  delay = 100ms; } --> cloudNode.canIn;
        can.truckIn <-- {  delay = 100ms; } <-- host.canOut;
        can.cloudIn <-- {  delay = 100ms; } <-- cloudNode.canOut;

        anotherCan.truckOut --> {  delay = 100ms; } --> host.can2In;
        anotherCan.cloudOut --> {  delay = 100ms; } --> cloudNode.can2In;
        anotherCan.truckIn <-- {  delay = 100ms; } <-- host.can2Out;
        anotherCan.cloudIn <-- {  delay = 100ms; } <-- cloudNode.can2Out;

        host.cloudOut --> {  delay = 100ms; } --> cloudNode.truckIn;
        host.cloudIn <-- {  delay = 100ms; } <-- cloudNode.truckOut;
}

