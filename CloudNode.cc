#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>
#include "NetworkCanvasUtils.h"

using namespace omnetpp;

class CloudNode : public cSimpleModule
{
    private:
        char buf[150];
        const char* config;
        int sentCloudFast=0 , rcvdCloudFast=0, sentCloudSlow=0, rcvdCloudSlow=0;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    void acknowledge(cMessage *message, std::string outpoint);
    virtual void initialize() override;
    virtual void handleFogBasedSolution(cMessage *msg);
    virtual void handleCloudBasedSolution(cMessage *msg);
    virtual void handleNoGarbageSolution(cMessage *msg);
    virtual void updateMessageStats(int sentCloudFast, int rcvdCloudFast, int sentCloudSlow, int rcvdCloudSlow);
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
    if(strcmp(config, "No garbage solution") != 0) {
        updateMessageStats(sentCloudFast, rcvdCloudFast, sentCloudSlow, rcvdCloudSlow);
    }


    NetworkCanvasUtils::addTextFiguresToCanvas();
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
    updateMessageStats(sentCloudFast, rcvdCloudFast, sentCloudSlow, rcvdCloudSlow);
}

void CloudNode::acknowledge(cMessage* message, std::string outpoint)
{
    send(message, outpoint.c_str());
}


void CloudNode::handleFogBasedSolution(cMessage *msg){
    rcvdCloudFast++;
    if (strcmp("7 – Collect can garbage", msg->getName()) == 0)
    {
        // from can to others
        NetworkCanvasUtils::updateTextFigure(4, NetworkCanvasUtils::quickDelay);
        // slow from others to cloud
        NetworkCanvasUtils::updateTextFigure(11, NetworkCanvasUtils::quickDelay);
        acknowledge(new cMessage("8 - OK"), "canOut");
        sentCloudFast++;
    }
    else if (strcmp("9 – Collect can garbage", msg->getName()) == 0)
    {
        // from anotherCan to others
        NetworkCanvasUtils::updateTextFigure(6, NetworkCanvasUtils::quickDelay);
        // slow from others to cloud
        NetworkCanvasUtils::updateTextFigure(11, NetworkCanvasUtils::quickDelay);
        acknowledge(new cMessage("10 - OK"), "can2Out");
        sentCloudFast++;
    }
}

void CloudNode::handleCloudBasedSolution(cMessage *msg){
    rcvdCloudSlow++;
    if (strcmp("7-Collect garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("8 - OK"), "truckOut");
        sentCloudSlow++;
        NetworkCanvasUtils::updateTextFigure(0, NetworkCanvasUtils::slowDelay);
        NetworkCanvasUtils::updateTextFigure(9, NetworkCanvasUtils::slowDelay);
    }
    else if (strcmp("9-Collect garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("10 - OK"), "truckOut");
        sentCloudSlow++;
        NetworkCanvasUtils::updateTextFigure(0, NetworkCanvasUtils::slowDelay);
        NetworkCanvasUtils::updateTextFigure(9, NetworkCanvasUtils::slowDelay);
    }
}

void CloudNode::handleNoGarbageSolution(cMessage *msg){

}

void CloudNode::updateMessageStats(int sentCloudFast, int rcvdCloudFast, int sentCloudSlow, int rcvdCloudSlow){
    sprintf(buf, "sentCloudFast: %d  rcvdCloudFast: %d sentCloudSlow: %d rcvdCloudSlow: %d", sentCloudFast, rcvdCloudFast, sentCloudSlow, rcvdCloudSlow);
    getDisplayString().setTagArg("t", 0, buf);
}


