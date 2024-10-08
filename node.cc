#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;


class CanvasManager : public cSimpleModule
{
protected:
    virtual void initialize() override {
        // Retrieve the configTitle parameter from the ini file
        const char* configTitle = par("configTitle").stringValue();
        EV << "Setting configTitle to: " << configTitle << endl;

        cCanvas* canvas = this->getParentModule()->getCanvas();
        char temp[100];

        cLabelFigure *total_num_cloud= (cLabelFigure*)(canvas->getFigure("Title"));
        sprintf(temp, "%s", configTitle);
        total_num_cloud->setText(temp);
        total_num_cloud->setFont(cFigure::Font("Arial", 12, cFigure::FONT_BOLD));

        EV << "Title updated to: " << configTitle << endl;
    }
};

Define_Module(CanvasManager);


class ComputerNode : public cSimpleModule
{
  private:
    simtime_t timeout;
    cMessage *timeoutEvent = nullptr;  // holds pointer to the timeout self-message
    cMessage *message = nullptr;  // message that has to be re-sent on timeout

  public:
    virtual ~ComputerNode();

  protected:
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(ComputerNode);

ComputerNode::~ComputerNode()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void ComputerNode::initialize()
{
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    EV << "Sending initial hello message\n";
    message = new cMessage("1-Hello");
    sendCopyOf(message);
}

void ComputerNode::sendCopyOf(cMessage *msg)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, "out");
}

void ComputerNode::handleMessage(cMessage *msg)
{
    if (msg == timeoutEvent)
    {
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Timeout expired, resending message and restarting timer\n";
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else
    {
        // message arrived
        // Acknowledgment received!
        EV << "Received: " << msg->getName() << "\n";
        delete msg;

        // Also delete the stored message and cancel the timeout event.
        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);

        if (strcmp("3- Test message", message->getName()) == 0)
        {
            // message 3 has been delivered to cloud and we stop the simulation
            delete message;
            message = nullptr;
            return;
        }
        delete message;

        // Ready to send another one.
        message = new cMessage("3- Test message");
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

class CloudNode : public cSimpleModule
{
  private:
    int numLostMsgs = 0;
    cMessage *ackMsg = new cMessage("4 – ACK");
  protected:
    virtual void handleMessage(cMessage *msg) override;
    void acknowledgeFromCloudToComputer();
  public:
      virtual ~CloudNode();
};

Define_Module(CloudNode);

CloudNode::~CloudNode()
{
    delete ackMsg;
}

void CloudNode::handleMessage(cMessage *msg)
{
    if (strcmp("1-Hello", msg->getName()) == 0)
    {
        EV << msg << " received, sending back an acknowledgment.\n";
        acknowledgeFromCloudToComputer();
    }
    else if (strcmp("3- Test message", msg->getName()) == 0)
    {
        numLostMsgs++;
        if (numLostMsgs > 3) {
            // it is time to acknowledge the computer
            acknowledgeFromCloudToComputer();
        } else {
            EV << "\"Losing\" message.\n";
            bubble("message lost");
        }
    }
    delete msg;
}

void CloudNode::acknowledgeFromCloudToComputer()
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *)ackMsg->dup();
    send(copy, "out");
}



