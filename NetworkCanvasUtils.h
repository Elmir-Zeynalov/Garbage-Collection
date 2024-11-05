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

    static const int slowDelay = 500;
    static const int quickDelay = 50;

    // Public access to figureDefinitions
    static std::vector<FigureInfo> delayMetrics;


    // Method to set texts figures to the canvas
    static void addTextFiguresToCanvas();

    // Method to update value for a specific figure index
    static void updateTextFigure(const int index, const int increaseBy);
};

#endif // NETWORK_CANVAS_UTILS_H
