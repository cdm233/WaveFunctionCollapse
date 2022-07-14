#pragma once
#include "./lodepng.h"
#include "./utilities.h"

struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    pixel();
    pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();

    pixel operator+(pixel &rhs) const {
        int nr = rhs.r;
        nr += r;
        int ng = rhs.g;
        ng += g;
        int nb = rhs.b;
        nb += b;
        int na = rhs.a;
        na += a;

        return pixel(clip(nr), clip(ng), clip(nb), clip(na));
    }
};

vector<vector<pixel>> imageBuffer2PixelBuffer(vector<uint8_t> imageBuffer, int width, int height);

struct image {
    unsigned height;
    unsigned width;
    string filePath;
    vector<uint8_t> imageBuffer;
    vector<vector<pixel>> pixelBuffer;

    image();
    image(string path);
    void read();
    void clear();

    image operator+(image &rhs) const {
        if (height != rhs.height) {
            return rhs;
        }
        image result;

        result.width = rhs.width + width;
        result.height = height;

        result.filePath = "";

        int breakPoint = round(((float)result.width) / 2);

        for (int row = 0; row < result.height; row++) {
            for (int col = 0; col < result.width; col++) {
                int index = col;

                pixel tempPixel;
                if (index >= breakPoint) {
                    index -= breakPoint;
                    tempPixel = rhs.pixelBuffer[row][index];
                } else {
                    tempPixel = pixelBuffer[row][index];
                }

                result.imageBuffer.push_back(tempPixel.r);
                result.imageBuffer.push_back(tempPixel.g);
                result.imageBuffer.push_back(tempPixel.b);
                result.imageBuffer.push_back(tempPixel.a);
            }
        }

        result.pixelBuffer = imageBuffer2PixelBuffer(result.imageBuffer, result.width, result.height);

        return result;
    }

    image operator|(image &rhs) const {
        if (width != rhs.width) {
            return rhs;
        }

        image result;

        result.width = rhs.width;
        result.height = rhs.height + height;

        result.filePath = "";

        int breakPoint = round(((float)result.height) / 2);

        for (int row = 0; row < result.height; row++) {
            for (int col = 0; col < result.width; col++) {
                int index = row;

                pixel tempPixel;
                if (index >= breakPoint) {
                    index -= breakPoint;
                    tempPixel = rhs.pixelBuffer[index][col];
                } else {
                    tempPixel = pixelBuffer[index][col];
                }

                result.imageBuffer.push_back(tempPixel.r);
                result.imageBuffer.push_back(tempPixel.g);
                result.imageBuffer.push_back(tempPixel.b);
                result.imageBuffer.push_back(tempPixel.a);
            }
        }

        result.pixelBuffer = imageBuffer2PixelBuffer(result.imageBuffer, result.width, result.height);

        return result;
    }
};

struct direction {
    int e;
    int n;
    int s;
    int w;

    void clear();
};

class tile {
   public:
    image data;
    direction connection;

    tile();
    tile(string filePath);
    tile(string filePath, direction connect);
};

class board {
   public:
    int boardWidth;
    int boardHeight;
    unsigned imageWidth;
    unsigned imageHeight;
    string filePath;
    vector<vector<tile>> content;
    vector<uint8_t> imageBuffer;
    vector<vector<pixel>> pixelBuffer;

    board();
    board(int width, int height);

    void writeImageBuffer();
    void writePixelBuffer();

    void exportBoard(string fileName, int width = -1, int height = -1);
};
