#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class CloudNode : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
    void acknowledge(cMessage *message, std::string outpoint);
  public:
      virtual ~CloudNode();
};

Define_Module(CloudNode);

CloudNode::~CloudNode()
{
}

void CloudNode::handleMessage(cMessage *msg)
{
    EV << msg << " received, sending back an acknowledgment.\n";

    if (strcmp("7-Collect garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("8 - OK"), "truckOut");
    }
    else if (strcmp("9-Collect garbage", msg->getName()) == 0)
    {
        acknowledge(new cMessage("10 - OK"), "truckOut");
    }
    delete msg;
}

void CloudNode::acknowledge(cMessage* message, std::string outpoint)
{
    send(message, outpoint.c_str());
}
