/*
    Wave function collapse, procedural generation of images and such.
*/

#include "./src/lodepng.h"
#include "./src/structure.h"
#include "./src/utilities.h"

#define boardWidth 2
#define boardHeight 2

int main() {
    board world(boardWidth, boardHeight);

    world.content[0][0] = tile("./images/Up.png");
    world.content[0][1] = tile("./images/Down.png");
    world.content[1][0] = tile("./images/Right.png");
    world.content[1][1] = tile("./images/Left.png");

    world.writeImageBuffer();
    world.exportBoard("board");

    return 0;
}