#include "utilities.h"

uint8_t clip(int input) {
    uint8_t result = 0;
    result = input;
    if (input > 255) {
        result = 255;
    } else if (input < 0) {
        result = 0;
    }

    return result;
}

pair<int, int> getNewLocFrom2DVec(vector<vector<int>> board) {
    srand(time(NULL));
    pair<int, int> result;
    vector<int> options;
    int boardHeight = board.size();
    int boardWidth = board[0].size();

    for (int row = 0; row < boardHeight; row++) {
        for (int loc = 0; loc < boardWidth; loc++) {
            if (board[row][loc] != -1) {
                options.push_back(row * boardWidth + loc);
            }
        }
    }
    if(options.empty()){
        return {-1, -1};
    }

    int pick = rand() % options.size();
    int location = options[pick];

    result.first = location / boardWidth;
    result.second = location % boardWidth;

    return result;
}

bool locWithinBoard(int x, int y, int width, int height){
    if(x < 0 || y < 0 || x >= width || y >= height){
        return false;
    }

    return true;
}

int getOpposite(int num) {
    int result = -1;

    switch (num)
    {
    case 0:
        result = 1;
        break;
    case 1:
        result = 0;
        break;
    case 2:
        result = 3;
        break;
    case 3:
        result = 2;
        break;
  
    default:
        break;
    }

    return result;
}