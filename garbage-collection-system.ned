
simple ComputerNode
{
    parameters:
        @display("i=device/pc");
    gates:
        input in;
        output out;
}

simple CloudNode
{
    parameters:
        @display("i=device/server");
    gates:
        input in[3];
        output out[3];
}

simple CanvasManager
{
    parameters:
        string configTitle;
    gates:
        // No gates required for this simple module
}


network GarbageCollectionSystem
{
    parameters:
        string configTitle;
  
    @figure[upper_street1](type=rectangle; pos=100,100; size=600,3; fillColor=black; fillOpacity=1);
    @figure[upper_street2](type=rectangle; pos=200,200; size=500,3; fillColor=black; fillOpacity=1);
    @figure[lower_street1](type=rectangle; pos=200,350; size=500,3; fillColor=black; fillOpacity=1);
    @figure[lower_street2](type=rectangle; pos=100,450; size=600,3; fillColor=black; fillOpacity=1);

    @figure[vertical_street1](type=rectangle; pos=100,100; size=3,350; fillColor=black; fillOpacity=1);
    @figure[vertical_street2](type=rectangle; pos=200,200; size=3,150; fillColor=black; fillOpacity=1);
    
    @figure[Title](type=text; pos=1500,30; text="Initial Title");

    
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
	
    @display("bgb=2000,600");

    submodules:
        canvasManager: CanvasManager {
            @display("p=100,100");  // The submodule will handle updating the title
        }
        
        can: ComputerNode {
            @display("p=210,70;i=block/bucket;r=100");
        }
        anotherCan: ComputerNode {
            @display("p=270,320;i=block/bucket;r=100");
        }
        host: ComputerNode {
            @display("p=610,135;i=block/wheelbarrow;r=70");
        }
        cloudNode: CloudNode {
            @display("p=837,275;i=block/table;r=380");
        }
    connections:
        can.out --> {  delay = 100ms; } --> cloudNode.in[0];
        can.in <-- {  delay = 100ms; } <-- cloudNode.out[0];
        anotherCan.out --> {  delay = 100ms; } --> cloudNode.in[1];
        anotherCan.in <-- {  delay = 100ms; } <-- cloudNode.out[1];
        host.out --> {  delay = 100ms; } --> cloudNode.in[2];
        host.in <-- {  delay = 100ms; } <-- cloudNode.out[2];
   
   

}

network CloudNetwork
{
    @display("bgb=550");
    submodules:
        computerNode: ComputerNode {
            @display("p=63,79");
        }
        cloudNode: CloudNode {
            @display("p=480,79");
        }
    connections:
        computerNode.out --> {  delay = 100ms; } --> cloudNode.in[0];
        computerNode.in <-- {  delay = 100ms; } <-- cloudNode.out[0];
}

