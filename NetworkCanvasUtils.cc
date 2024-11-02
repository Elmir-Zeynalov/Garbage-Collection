#include "NetworkCanvasUtils.h"


// Define the vector with figure details
std::vector<NetworkCanvasUtils::FigureInfo> NetworkCanvasUtils::delayMetrics = {
    {"slow_from_smartphone_to_others", "Slow connection from the smartphone to others (time it takes) =", 1500, 90},
    {"slow_others_to_smartphone", "Slow connection from others to the smartphone (time it takes) =", 1500, 130},
    {"fast_from_smartphone_to_others", "Fast connection from the smartphone to others (time it takes) =", 1500, 170},
    {"fast_others_to_smartphone", "Fast connection from others to the smartphone (time it takes) =", 1500, 210},
    {"from_can_to_others", "Connection from the can to others (time it takes) =", 1500, 300},
    {"from_others_to_can", "Connection from others to the can (time it takes) =", 1500, 340},
    {"from_anotherCan_to_others", "Connection from the anotherCan to others (time it takes) =", 1500, 430},
    {"from_others_to_anotherCan", "Connection from others to the anotherCan (time it takes) =", 1500, 470},
    {"slow_from_cloud_to_others", "Slow connection from the Cloud to others (time it takes) =", 1500, 550},
    {"slow_from_others_to_cloud", "Slow connection from others to the Cloud (time it takes) =", 1500, 590},
    {"fast_from_cloud_to_others", "Fast connection from the Cloud to others (time it takes) =", 1500, 630},
    {"fast_from_others_to_cloud", "Fast connection from others to the Cloud (time it takes) =", 1500, 670}
};


void NetworkCanvasUtils::addTextFiguresToCanvas(){
    // Access the root network module
    cModule *network = getSimulation()->getSystemModule();
    if (network == nullptr) {
        EV << "Network module not found!" << endl;
        return;
    }

    // Access the canvas of the network module
    cCanvas *canvas = network->getCanvas();

    for (const auto& figureInfo : delayMetrics) {
        cTextFigure *textFigure = new cTextFigure(figureInfo.name.c_str());
        textFigure->setText(figureInfo.text.c_str());
        textFigure->setPosition(cFigure::Point(figureInfo.posX, figureInfo.posY));
        textFigure->setFont(cFigure::Font("Arial", 16, cFigure::FONT_NONE));
        textFigure->setColor(cFigure::BLACK);

        canvas->addFigure(textFigure);
    }
}

//TODO:: Need to add a way to alter/update the text on screen
//One way of doing it is to pass WHAT you want to update and the NEWTEXT.
//You can use the vector/dictionary to retrieve what is already in the canvas. the X and Y coordinates by using the figureName as the key
//and just replacing the value with your newText.
void NetworkCanvasUtils::updateTextFigure(const std::string &figureName, const std::string &newText) {

}
