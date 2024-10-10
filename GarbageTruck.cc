#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class GarbageTruck : public cSimpleModule
{
  private:
    simtime_t timeout;
    cMessage *timeoutEvent = nullptr;  // holds pointer to the timeout self-message
    cMessage *message = nullptr;  // message that has to be re-sent on timeout
    std::string currentOut;

  public:
    virtual ~GarbageTruck();

  protected:
    virtual void sendCopyOf(cMessage *msg, std::string out);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(GarbageTruck);

GarbageTruck::~GarbageTruck()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void GarbageTruck::initialize()
{

    // Retrieve the configTitle parameter from the ini file
    // UPDATING TITLE
    const char* configTitle = par("configTitle").stringValue();
    EV << "Setting configTitle to: " << configTitle << endl;
    cCanvas* canvas = this->getParentModule()->getCanvas();
    char temp[100];
    cLabelFigure *total_num_cloud= (cLabelFigure*)(canvas->getFigure("Title"));
    sprintf(temp, "%s", configTitle);
    total_num_cloud->setText(temp);
    total_num_cloud->setFont(cFigure::Font("Arial", 12, cFigure::FONT_BOLD));
    EV << "Title updated to: " << configTitle << endl;
    //

    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    EV << "Sending msg to can 1\n";
    message = new cMessage("1-Is the can full?");
    currentOut = "canOut";
    sendCopyOf(message, currentOut);
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void GarbageTruck::sendCopyOf(cMessage *msg, std::string out)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, out.c_str());
}

void GarbageTruck::handleMessage(cMessage *msg)
{
    if (msg == timeoutEvent)
    {
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Timeout expired, resending message and restarting timer\n";
        sendCopyOf(message, currentOut);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else
    {
        // message arrived
        // Acknowledgment received!
        EV << "Received: " << msg->getName() << "\n";
        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);

        if (strcmp("3 – YES", msg->getName()) == 0)
        {
            // can 1 is full, needs to be cleaned
            sendCopyOf(new cMessage("7-Collect garbage"), "cloudOut");
        }
        else if (strcmp("6 - YES", msg->getName()) == 0)
        {
            // can 2 is full, needs to be cleaned
            sendCopyOf(new cMessage("9-Collect garbage"), "cloudOut");
        }
        else if (strcmp("8 - OK", msg->getName()) == 0)
        {
            message = new cMessage("4-Is the can full?");
            currentOut = "can2Out";
            sendCopyOf(message, currentOut);
            scheduleAt(simTime()+timeout, timeoutEvent);
        }
    }
}

