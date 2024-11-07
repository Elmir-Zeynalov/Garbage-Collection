#ifndef NETWORK_CANVAS_UTILS_H
#define NETWORK_CANVAS_UTILS_H

#include <omnetpp.h>
#include <vector>
#include <string>

using namespace omnetpp;

class NetworkCanvasUtils {
public:
    struct FigureInfo {
            std::string name;
            std::string text;
            int value;
            int posX;
            int posY;
        };

    static const int g4 = 50; // smartphone and cloud in cloud solution
    static const int wifi = 25; // smartphone and cans
    static const int lan = 5; // cans and cloud in fog solution

    // Public access to figureDefinitions
    static std::vector<FigureInfo> delayMetrics;


    // Method to set texts figures to the canvas
    static void addTextFiguresToCanvas();

    // Method to update value for a specific figure index
    static void updateTextFigure(const int index, const int increaseBy);
};

#endif // NETWORK_CANVAS_UTILS_H
