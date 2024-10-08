
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

network GarbageCollectionSystem
{
    @figure[upper_street1](type=rectangle; pos=100,100; size=600,3; fillColor=black; fillOpacity=1);
    @figure[upper_street2](type=rectangle; pos=200,200; size=500,3; fillColor=black; fillOpacity=1);
    @figure[lower_street1](type=rectangle; pos=200,350; size=500,3; fillColor=black; fillOpacity=1);
    @figure[lower_street2](type=rectangle; pos=100,450; size=600,3; fillColor=black; fillOpacity=1);

    @figure[vertical_street1](type=rectangle; pos=100,100; size=3,350; fillColor=black; fillOpacity=1);
    @figure[vertical_street2](type=rectangle; pos=200,200; size=3,150; fillColor=black; fillOpacity=1);

    @display("bgb=2000,600");
    submodules:
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

