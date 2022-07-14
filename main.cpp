/*
    Wave function collapse, procedural generation of images and such.
*/

#include "./src/lodepng.h"
#include "./src/structure.h"
#include "./src/utilities.h"

#define boardWidth 3
#define boardHeight 3

int main() {
    board world(boardWidth, boardHeight);

    world.generateImage();
    world.writeImageBuffer();
    world.exportBoard("Board");

    return 0;
}