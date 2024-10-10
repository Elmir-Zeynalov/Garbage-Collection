#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class TrashCan : public cSimpleModule
{
  private:
    int numLostMsgs = 0;

  public:
    virtual ~TrashCan();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TrashCan);

TrashCan::~TrashCan()
{
}

void TrashCan::initialize()
{
    /*
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

    EV << "Sending initial hello message\n";
    message = new cMessage("1-Hello");
    sendCopyOf(message);*/
}

void TrashCan::handleMessage(cMessage *msg)
{
    if (strcmp("1-Is the can full?", msg->getName()) == 0)
    {
        numLostMsgs++;
        if (numLostMsgs > 3) {
            // it is time to acknowledge the truck
            send(new cMessage("3 – YES"), "truckOut");
        } else {
            EV << "\"Losing\" message.\n";
            bubble("message lost");
        }
    }
    else if (strcmp("4-Is the can full?", msg->getName()) == 0)
    {
        numLostMsgs++;
        if (numLostMsgs > 3) {
            // it is time to acknowledge the truck
            send(new cMessage("6 - YES"), "truckOut");
        } else {
            EV << "\"Losing\" message.\n";
            bubble("message lost");
        }
    }

    delete msg;
}
