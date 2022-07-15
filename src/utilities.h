#pragma once

#include <math.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

uint8_t clip(int input);
pair<int, int> getNewLocFrom2DVec(vector<vector<int>> board);
bool locWithinBoard(int x, int y, int width, int height);
int getOpposite(int num);
vector<int> string2vec(string input, string split);