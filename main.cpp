/*
    Wave function collapse, procedural generation of images and such.
*/

#include "./src/lodepng.h"
#include "./src/structure.h"
#include "./src/utilities.h"

#define boardWidth 5
#define boardHeight 5

int main() {
    board world(boardWidth, boardHeight);

    world.readConfig("./images/config.ini");

    world.generateImage(time(NULL), 1, 2, 2);
    world.writeImageBuffer();
    world.exportBoard("Board");

    return 0;
}