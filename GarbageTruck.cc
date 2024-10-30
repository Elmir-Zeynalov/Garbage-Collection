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
    cMessage *delayMessage = nullptr;
    cMessage *secondDelayMessage = nullptr;

    std::string currentOut;
    const char* config;
    char buf[150];
    int sentHostFast=0 , rcvdHostFast=0, sentHostSlow=0, rcvdHostSlow=0;

  public:
    virtual ~GarbageTruck();

  protected:
    virtual void sendCopyOf(cMessage *msg, std::string out);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleFogBasedSolution(cMessage *msg);
    virtual void handleCloudBasedSolution(cMessage *msg);
    virtual void handleNoGarbageSolution(cMessage *msg);
    virtual void updateMessageStats(int sentHostFast, int rcvdHostFast, int sentHostSlow, int rcvdHostSlow);
};

Define_Module(GarbageTruck);

GarbageTruck::~GarbageTruck()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void GarbageTruck::initialize()
{
    char temp[100];
    updateMessageStats(0, 0, 0, 0);

    // Retrieve the configTitle parameter from the ini file
    // UPDATING TITLE
    const char* configTitle = par("configTitle").stringValue();
    config = configTitle;

    EV << "Setting configTitle to: " << configTitle << endl;
    cModule *network = getSystemModule(); // This directly accesses the root network module
    cCanvas* canvass = network->getCanvas();
    cFigure *figure = canvass->getFigure("Title");
    cLabelFigure *total_num_cloud= (cLabelFigure*)figure;

    sprintf(temp, "%s", configTitle);
    total_num_cloud->setText(temp);
    total_num_cloud->setFont(cFigure::Font("Arial", 12, cFigure::FONT_BOLD));
    EV << "Title updated to: " << configTitle << endl;


    //CHECKPOINT 1 > All of the above works
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    EV << "Sending msg to can 1\n";

    //message = new cMessage("1-Is the can full?");
    //currentOut = "canOut";
    //sendCopyOf(message, currentOut);
    //sentHostFast++;
    //scheduleAt(simTime()+timeout, timeoutEvent);

    delayMessage = new cMessage("initialMessageDelay");
    scheduleAt(simTime()+timeout + 11.0, delayMessage);
    updateMessageStats(sentHostFast, rcvdHostFast, sentHostSlow, rcvdHostSlow);
}

void GarbageTruck::sendCopyOf(cMessage *msg, std::string out)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, out.c_str());
}

void GarbageTruck::handleMessage(cMessage *msg)
{
    updateMessageStats(sentHostFast, rcvdHostFast, sentHostSlow, rcvdHostSlow);
    if(msg == delayMessage){
        // Send the initial message after the delay
                EV << "Sending initial message to can 1 after delay\n";
                message = new cMessage("1-Is the can full?");
                currentOut = "canOut";
                sendCopyOf(message, currentOut);
                sentHostFast++;

                // Schedule the next timeout if needed
                scheduleAt(simTime() + timeout, timeoutEvent);

                delete msg;  // Clean up the initialMessage after use
    }else if(msg == secondDelayMessage){

        message = new cMessage("4-Is the can full?");
        currentOut = "can2Out";
        sendCopyOf(message, currentOut);
        sentHostFast++;
        scheduleAt(simTime() + timeout, timeoutEvent);
    }
    else
    //On timeouts we always behave in the same manner
    if (msg == timeoutEvent)
    {
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Timeout expired, resending message and restarting timer\n";
        sendCopyOf(message, currentOut);
        scheduleAt(simTime()+timeout, timeoutEvent);
        sentHostFast++;
    }
    else
    {
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

    }

    // refresh statistics
    updateMessageStats(sentHostFast, rcvdHostFast, sentHostSlow, rcvdHostSlow);

}

void GarbageTruck::handleFogBasedSolution(cMessage *msg){
    // message arrived
       // Acknowledgment received!
       EV << "Received: " << msg->getName() << "\n";
       EV << "Timer cancelled.\n";
       cancelEvent(timeoutEvent);

       if (strcmp("3 – YES", msg->getName()) == 0)
       {
           //message = new cMessage("4-Is the can full?");
           //currentOut = "can2Out";
           //sendCopyOf(message, currentOut);
           rcvdHostFast++;
           //sentHostFast++;
           secondDelayMessage = new cMessage("SecondDelay");
           scheduleAt(simTime()+timeout + 7, secondDelayMessage);
       }

       else if (strcmp("6 - YES", msg->getName()) == 0)
       {
           EV << "WE GOT 6????: " << msg->getName() << "\n";
           // can 2 is full, needs to be cleaned
           rcvdHostFast++;

       }else {
           EV << "FOG:TRUCK -> " << msg->getName() << "\n";
       }
}

void GarbageTruck::handleCloudBasedSolution(cMessage *msg){
    // message arrived
    // Acknowledgment received!
    EV << "Received: " << msg->getName() << "\n";
    EV << "Timer cancelled.\n";
    cancelEvent(timeoutEvent);

    if (strcmp("3 – YES", msg->getName()) == 0)
    {
        rcvdHostFast++;
        // can 1 is full, needs to be cleaned
        sendCopyOf(new cMessage("7-Collect garbage"), "cloudOut");
        sentHostSlow++;
    }
    else if (strcmp("6 - YES", msg->getName()) == 0)
    {
        rcvdHostFast++;
        EV << "WE GOT 6????: " << msg->getName() << "\n";
        // can 2 is full, needs to be cleaned
        sendCopyOf(new cMessage("9-Collect garbage"), "cloudOut");
        sentHostSlow++;
    }else if (strcmp("8 - OK", msg->getName()) == 0)
    {
        message = new cMessage("4-Is the can full?");
        currentOut = "can2Out";
        sendCopyOf(message, currentOut);
        scheduleAt(simTime()+timeout, timeoutEvent);
        rcvdHostSlow++;
    }
    else if (strcmp("10 - OK", msg->getName()) == 0)
    {
        rcvdHostSlow++;
    }
}

void GarbageTruck::handleNoGarbageSolution(cMessage *msg){
    // message arrived
    // Acknowledgment received!
    EV << "Received: " << msg->getName() << "\n";
    EV << "Timer cancelled.\n";
    cancelEvent(timeoutEvent);

    if (strcmp("2 – NO", msg->getName()) == 0)
    {
        rcvdHostFast++;
        message = new cMessage("4-Is the can full?");
        currentOut = "can2Out";
        sendCopyOf(message, currentOut);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else if (strcmp("5 – NO", msg->getName()) == 0)
    {
        rcvdHostFast++;
        EV << "No garbage to collect:  " << msg->getName() << "\n";
    }
}


void GarbageTruck::updateMessageStats(int sentHostFast, int rcvdHostFast, int sentHostSlow, int rcvdHostSlow){
    cModule *parent = getParentModule();

    sprintf(buf, "sentHostFast: %d  rcvdHostFast: %d sentHostSlow: %d rcvdHostSlow: %d", sentHostFast, rcvdHostFast, sentHostSlow, rcvdHostSlow);
    parent->getDisplayString().setTagArg("t", 0, buf);
}


