#include "./structure.h"

void pixel::clear() {
    r = -1;
    g = -1;
    b = -1;
    a = -1;
}

pixel::pixel() {
    clear();
}

pixel::pixel(uint8_t ri, uint8_t gi, uint8_t bi, uint8_t ai) {
    clear();
    this->r = ri;
    this->g = gi;
    this->b = bi;
    this->a = ai;
}

void image::clear() {
    height = -1;
    width = -1;
    filePath = "";
    pixelBuffer = {};
}

image::image() {
    clear();
}

image::image(string path) {
    clear();
    filePath = path;
    read();
}

vector<vector<pixel>> imageBuffer2PixelBuffer(vector<uint8_t> imageBuffer, int width, int height) {
    auto tempPixelBuffer = vector<vector<pixel>>(height, vector<pixel>(width));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = imageBuffer[4 * (y * width + x)];
            uint8_t g = imageBuffer[4 * (y * width + x) + 1];
            uint8_t b = imageBuffer[4 * (y * width + x) + 2];
            uint8_t a = imageBuffer[4 * (y * width + x) + 3];
            pixel temp(r, g, b, a);
            tempPixelBuffer[y][x] = temp;
        }
    }

    return tempPixelBuffer;
}

void image::read() {
    unsigned error = lodepng::decode(imageBuffer, width, height, filePath);
    pixelBuffer = imageBuffer2PixelBuffer(imageBuffer, width, height);
}

void direction::clear() {
    e = -1;
    n = -1;
    s = -1;
    w = -1;
}

tile::tile() {
    data.clear();
    connection.clear();
}

tile::tile(string filePath) {
    filePath = filePath;
    data = image(filePath);
    connection.clear();
}

tile::tile(string filePath, direction connect) {
    filePath = filePath;
    data = image(filePath);
    connection = connect;
}

board::board() {
    boardWidth = -1;
    boardHeight = -1;
    content.clear();
}

board::board(int width, int height) {
    boardWidth = width;
    boardHeight = height;
    content = vector<vector<tile>>(height, vector<tile>(width));
}

void board::writeImageBuffer() {
    image result;
    bool first = true;

    for (auto row : content) {
        image rowImage;
        for (int i = 0; i < row.size(); i++) {
            if (i == 0) {
                rowImage = row[i].data;
            } else {
                rowImage = rowImage + row[i].data;
            }
        }

        if(first){
            result = rowImage;
            first = false;
        } else {
            result = result | rowImage;
        }
    }

    imageBuffer = result.imageBuffer;
    imageWidth = content[0][0].data.width * content[0].size() ;
    imageHeight = content[0][0].data.width * content.size() ;
}

void board::writePixelBuffer() {
    pixelBuffer = imageBuffer2PixelBuffer(imageBuffer, imageWidth, imageHeight);
}

void board::exportBoard(string fileName, int width, int height){
    bool originalSize = width == height == -1;
    fileName = fileName + ".png";

    if(imageBuffer.empty()){
        writeImageBuffer();
    }

    if(!originalSize){

    }

    lodepng::encode(fileName, imageBuffer, imageWidth, imageHeight);
}