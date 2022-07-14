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
    imageWidth = -1;
    imageHeight = -1;
    numTypes = 5;
    filePath = "";
    filled = false;
    content.clear();
    entropy.clear();
}

board::board(int width, int height) {
    boardWidth = width;
    boardHeight = height;
    imageWidth = -1;
    imageHeight = -1;
    numTypes = 5;
    filePath = "";
    filled = false;
    content = vector<vector<tile>>(height, vector<tile>(width));
    entropy = vector<vector<int>>(height, vector<int>(width, -2));
}

void board::writeImageBuffer() {
    image result;
    bool first = true;

    if (!filled) {
        return;
    }

    for (auto row : content) {
        image rowImage;
        for (int i = 0; i < row.size(); i++) {
            if (i == 0) {
                rowImage = row[i].data;
            } else {
                rowImage = rowImage + row[i].data;
            }
        }

        if (first) {
            result = rowImage;
            first = false;
        } else {
            result = result | rowImage;
        }
    }

    imageBuffer = result.imageBuffer;
    imageWidth = content[0][0].data.width * content[0].size();
    imageHeight = content[0][0].data.width * content.size();
}

void board::writePixelBuffer() {
    pixelBuffer = imageBuffer2PixelBuffer(imageBuffer, imageWidth, imageHeight);
}

void board::exportBoard(string fileName, int width, int height) {
    if (!filled) {
        cout << "Board not completed filled!";
        return;
    }
    bool originalSize = width == height == -1;
    fileName = fileName + ".png";

    if (imageBuffer.empty()) {
        writeImageBuffer();
    }

    if (!originalSize) {
    }

    lodepng::encode(fileName, imageBuffer, imageWidth, imageHeight);
}

void board::resizeBoard(int width, int height) {
    boardWidth = width;
    boardHeight = height;
    content = vector<vector<tile>>(height, vector<tile>(width));
    entropy = vector<vector<int>>(height, vector<int>(width, numTypes));
}

direction board::getTileConnection(int type) {
    // e = 0, d = 1, clockwise
    vector<vector<int>> dirDic = {
        {1, 1, 0, 1},
        {1, 1, 1, 0},
        {0, 1, 1, 1},
        {1, 0, 1, 1},
        {0, 0, 0, 0}};

    direction connect;
    connect.clear();
    connect.n = dirDic[type][0];
    connect.e = dirDic[type][1];
    connect.s = dirDic[type][2];
    connect.w = dirDic[type][3];

    return connect;
}

string board::getTilePath(int type) {
    vector<string> dic = {
        "./images/Up.png",
        "./images/Right.png",
        "./images/Down.png",
        "./images/Left.png",
        "./images/Empty.png"};

    return dic[type];
}

tile board::getNewTile(int type) {
    if (type != -1) {
        tile newTile(getTilePath(type));
        newTile.connection = getTileConnection(type);

        return newTile;
    }

    srand(randSeed);

    int randType = rand() % numTypes;
    tile newTile(getTilePath(randType));
    newTile.connection = getTileConnection(randType);

    return newTile;
}

void board::calculateBoardEntropy(vector<vector<vector<string>>> &options) {
    vector<pair<vector<int>, string>> connectionConditions = {
        {{1, 0, 1, 1}, "Up"},
        {{0, 1, 1, 1}, "Down"},
        {{1, 1, 1, 0}, "Left"},
        {{1, 1, 0, 1}, "Right"},
        {{0, 0, 0, 0}, "Empty"}};
    vector<pair<int, int>> directions = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}};
    for (int rowIndex = 0; rowIndex < boardHeight; rowIndex++) {
        for (int locIndex = 0; locIndex < boardWidth; locIndex++) {
            // numbering same as "directions"
            vector<int> available(4, -1);
            if (entropy[rowIndex][locIndex] == -1) {
                continue;
            }
            for (int dirIndex = 0; dirIndex < directions.size(); dirIndex++) {
                auto dir = directions[dirIndex];
                auto newRow = rowIndex + dir.first;
                auto newLoc = locIndex + dir.second;

                if (locWithinBoard(newLoc, newRow, boardWidth, boardHeight)) {
                    if (entropy[newRow][newLoc] == -1) {  // if the tile is not empty
                        auto thisTile = content[newRow][newLoc];
                        auto connectionType = -1;

                        switch (dirIndex) {
                            case 0:
                                connectionType = (thisTile.connection.s);
                                break;
                            case 1:
                                connectionType = (thisTile.connection.n);
                                break;
                            case 2:
                                connectionType = (thisTile.connection.e);
                                break;
                            case 3:
                                connectionType = (thisTile.connection.w);
                                break;
                            default:
                                break;
                        }

                        available[dirIndex] = connectionType;
                    }
                }
            }

            vector<string> newEntropyArr;
            for (int optionIndex = 0; optionIndex < available.size(); optionIndex++) {
                if (available[optionIndex] == -1) {
                    continue;
                }
                // int opposite = getOpposite(optionIndex);
                for (auto possibleTile : connectionConditions) {
                    if (possibleTile.first[optionIndex] == available[optionIndex]) {
                        newEntropyArr.push_back(possibleTile.second);
                    }
                }
            }

            sort(newEntropyArr.begin(), newEntropyArr.end());
            newEntropyArr.erase(unique(newEntropyArr.begin(), newEntropyArr.end()), newEntropyArr.end());

            options[rowIndex][locIndex] = newEntropyArr;

            if (newEntropyArr.size() == 0 && entropy[rowIndex][rowIndex] != -1) {
                entropy[rowIndex][locIndex] = -2;
            } else {
                entropy[rowIndex][locIndex] = newEntropyArr.size();
            }
        }
    }
}

pair<int, int> board::getLowestEntropyLoc() {
    int lowest = numTypes;
    pair<int, int> result;
    for (int rowIndex = 0; rowIndex < entropy.size(); rowIndex++) {
        for (int locIndex = 0; locIndex < entropy[0].size(); locIndex++) {
            if (entropy[rowIndex][locIndex] < lowest && entropy[rowIndex][locIndex] > 0) {
                lowest = entropy[rowIndex][locIndex];
                result = {rowIndex, locIndex};
            }
        }
    }

    return result;
}

void board::placeTile(int sx, int sy) {
    vector<pair<int, int>> directions = {
        {1, 0},
        {-1, 0},
        {0, -1},
        {0, 1}};

    cout << "Place Tile At: " << sy << " " << sx << endl;
    entropy[sy][sx] = -1;

    for (int dirIndex = 0; dirIndex < directions.size(); dirIndex++) {
        auto dir = directions[dirIndex];
        auto newRow = sy + dir.first;
        auto newLoc = sx + dir.second;

        if (locWithinBoard(newLoc, newRow, boardWidth, boardHeight)) {
            if (entropy[newRow][newLoc] != -1) {
                entropy[newRow][newLoc] = numTypes;
            }
        }
    }
}

void board::generateImage(size_t seed, int sy, int sx) {
    if (seed == -1) {
        seed = time(NULL);
    }
    if (sy == -1 && sx == -1) {
        auto newLoc = getNewLocFrom2DVec(entropy);
        sy = newLoc.first;
        sx = newLoc.second;
    }

    srand(seed);
    randSeed = seed;

    entropy[sy][sx] = -1;  // -1 represents tile is taken

    content[sy][sx] = getNewTile();

    // cout << sy << " " << sx << endl;

    // place first tile
    placeTile(sx, sy);
    cout << content[sy][sx].data.filePath << endl;

    while (!filled) {
        vector<vector<vector<string>>> options(boardHeight, vector<vector<string>>(boardWidth));
        calculateBoardEntropy(options);
        auto newLoc = getLowestEntropyLoc();
        int selected = -1;

        if (options[newLoc.first][newLoc.second].size() == 0) {
            selected = 4;
        } else {
            int randType = rand() % options[newLoc.first][newLoc.second].size();
            auto name = options[newLoc.first][newLoc.second][randType];

            if (name == "Up") {
                selected = 0;
            } else if (name == "Down") {
                selected = 2;
            } else if (name == "Left") {
                selected = 1;
            } else if (name == "Right") {
                selected = 3;
            } else {
                selected = 4;
            }
        }

        content[newLoc.first][newLoc.second] = getNewTile(selected);
        placeTile(newLoc.second, newLoc.first);
        cout << "  Type: " << selected << endl;

        calculateBoardEntropy(options);
        setBoardFilled();
    }
}

void board::setBoardFilled() {
    for (auto row : entropy) {
        for (auto loc : row) {
            if (loc != -1) {
                filled = false;
                return;
            }
        }
    }

    filled = true;
}