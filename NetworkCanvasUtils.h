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
            int posX;
            int posY;
        };

    // Public access to figureDefinitions
    static std::vector<FigureInfo> delayMetrics;


    // Method to add initial figures to the canvas
    static void addTextFiguresToCanvas();

    // Method to update text for a specific figure
    static void updateTextFigure(const std::string &figureName, const std::string &newText);
};

#endif // NETWORK_CANVAS_UTILS_H
