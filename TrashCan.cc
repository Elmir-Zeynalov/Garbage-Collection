#include <string.h>
#include <cstring>
#include <omnetpp.h>
#include <stdio.h>
#include "NetworkCanvasUtils.h"

using namespace omnetpp;

class TrashCan : public cSimpleModule
{
  private:
    int numLostMsgs = 0;
    const char* config;
    char buf[150];
    int sentCanFast=0, rcvdCanFast=0, numberOfLostCanMsgs=0;

  public:
    virtual ~TrashCan();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void updateMessageStats(int sentCanFast, int rcvdCanFast, int numberOfLostCanMsgs);
};

Define_Module(TrashCan);

TrashCan::~TrashCan()
{
}

void TrashCan::initialize()
{
    // Retrieve the configTitle parameter from the ini file
    const char* configTitle = par("configTitle").stringValue();
    config = configTitle;
    //Update the message stats on init
    updateMessageStats(0,0,0);
}

void TrashCan::handleMessage(cMessage *msg)
{
    updateMessageStats(sentCanFast, rcvdCanFast, numberOfLostCanMsgs);
    if (strcmp("1-Is the can full?", msg->getName()) == 0)
    {
        // fast connection from host/smartphone to others
        NetworkCanvasUtils::updateTextFigure(2, NetworkCanvasUtils::wifi);

        numLostMsgs++;
        if (numLostMsgs > 3) {
            // it is time to acknowledge the truck
            rcvdCanFast++;
            if(strcmp(config, "No garbage solution") == 0){
                send(new cMessage("2 – NO"), "truckOut");
            }else{
                //always send 3 - YES
                send(new cMessage("3 – YES"), "truckOut");

                //only in the FOG solution do we send an answer to the cloud
                if(strcmp(config, "Fog-based solution with fast messages") == 0){
                    sentCanFast++;
                    send(new cMessage("7 – Collect can garbage"), "cloudOut");
                }
            }
            // others to can
            NetworkCanvasUtils::updateTextFigure(5, NetworkCanvasUtils::wifi);

            sentCanFast++;
        } else {
            EV << "\"Losing\" message.\n";
            numberOfLostCanMsgs++;
            bubble("message lost");
        }
    }
    else if (strcmp("4-Is the can full?", msg->getName()) == 0)
    {
        // fast connection from host/smartphone to others
        NetworkCanvasUtils::updateTextFigure(2, NetworkCanvasUtils::wifi);

        numLostMsgs++;
        if (numLostMsgs > 3) {
            // it is time to acknowledge the truck
            rcvdCanFast++;
            if(strcmp(config, "No garbage solution") == 0){
                send(new cMessage("5 – NO"), "truckOut");
            }else{
                //always send 3 - YES
                send(new cMessage("6 - YES"), "truckOut");
                //only in the FOG solution do we send an answer to the cloud
                if(strcmp(config, "Fog-based solution with fast messages") == 0){
                    sentCanFast++;
                    send(new cMessage("9 – Collect can garbage"), "cloudOut");
                }
            }
            // others to anotherCan
            NetworkCanvasUtils::updateTextFigure(7, NetworkCanvasUtils::wifi);
            sentCanFast++;

        } else {
            EV << "\"Losing\" message.\n";
            numberOfLostCanMsgs++;
            bubble("message lost");
        }
    }
    else if(strcmp("8 - OK", msg->getName()) == 0){
        rcvdCanFast++;
        // others to can
        NetworkCanvasUtils::updateTextFigure(5, NetworkCanvasUtils::lan);
        // slow from cloud to others
        NetworkCanvasUtils::updateTextFigure(10, NetworkCanvasUtils::lan);
    }else if(strcmp("10 - OK", msg->getName()) == 0){
        NetworkCanvasUtils::updateTextFigure(7, NetworkCanvasUtils::lan);
        NetworkCanvasUtils::updateTextFigure(10, NetworkCanvasUtils::lan);
        rcvdCanFast++;
    }

    updateMessageStats(sentCanFast, rcvdCanFast, numberOfLostCanMsgs);
    delete msg;
}

void TrashCan::updateMessageStats(int sentCanFast, int rcvdCanFast, int numberOfLostCanMsgs){
    const int canId = par("canId").intValue();

    sprintf(buf, "%s: %d  %s: %d numberOfLostCanMsgs: %d ", (canId == 1 ? "sentCanFast" : "sentAnotherCanFast"), sentCanFast,
            (canId == 1 ? "rcvdCanFast" : "rcvdAnotherCanFast"), rcvdCanFast, numberOfLostCanMsgs);
    getDisplayString().setTagArg("t", 0, buf);
}




