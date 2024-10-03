
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
        input in;
        output out;
}


network FogNetwork
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
        computerNode.out --> {  delay = 100ms; } --> cloudNode.in;
        computerNode.in <-- {  delay = 100ms; } <-- cloudNode.out;
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
        computerNode.out --> {  delay = 100ms; } --> cloudNode.in;
        computerNode.in <-- {  delay = 100ms; } <-- cloudNode.out;
}