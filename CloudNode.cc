#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class CloudNode : public cSimpleModule
{
    private:
        const char* config;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    void acknowledge(cMessage *message, std::string outpoint);
    virtual void initialize() override;
    virtual void handleFogBasedSolution(cMessage *msg);
    virtual void handleCloudBasedSolution(cMessage *msg);
    virtual void handleNoGarbageSolution(cMessage *msg);
  public:
      virtual ~CloudNode();
};

Define_Module(CloudNode);

CloudNode::~CloudNode()
{
}

void CloudNode::initialize()
{
    // Retrieve the configTitle parameter from the ini file
    const char* configTitle = par("configTitle").stringValue();
    config = configTitle;
}




void CloudNode::handleMessage(cMessage *msg)
{
    EV << msg << " received, sending back an acknowledgment.\n";

    //Behaviour changes depending on the config
    if(strcmp(config, "Fog-based solution with fast messages") == 0){
        handleFogBasedSolution(msg);
    }
    else if(strcmp(config, "Cloud-based solution with slow messages") == 0){
        handleCloudBasedSolution(msg);
    }else if(strcmp(config, "No garbage solution") == 0){
        handleNoGarbageSolution(msg);
    }
    else{
        //We shouldnt be here. This is an erroneous state
        EV << "Unknown configuration, no specific handling\n";
    }
    delete msg;
}

void CloudNode::acknowledge(cMessage* message, std::string outpoint)
{
    send(message, outpoint.c_str());
}


void CloudNode::handleFogBasedSolution(cMessage *msg){
    if (strcmp("7 – Collect can garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("8 - OK"), "canOut");
    }
    else if (strcmp("9 – Collect can garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("10 - OK"), "can2Out");
    }
}

void CloudNode::handleCloudBasedSolution(cMessage *msg){
    if (strcmp("7-Collect garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("8 - OK"), "truckOut");
    }
    else if (strcmp("9-Collect garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("10 - OK"), "truckOut");
    }
}

void CloudNode::handleNoGarbageSolution(cMessage *msg){

}



