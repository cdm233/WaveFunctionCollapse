/*
    Wave function collapse, procedural generation of images and such.
*/

#include "./src/lodepng.h"
#include "./src/structure.h"
#include "./src/utilities.h"

int main() {
    singleSettingLoader sLoader("./settings.ini");
    int boardWidth = 0;
    int boardHeight = 0;
    boardWidth = atoi(sLoader.getSetting("width").c_str());
    boardHeight = atoi(sLoader.getSetting("height").c_str());

    board world(boardWidth, boardHeight);

    world.readConfig("./images/config.ini");


    world.generateImage();
    world.writeImageBuffer();
    world.exportBoard("Board");

    return 0;
}