#ifndef _DEFS__H
#define _DEFS__H
#include <vector>
using namespace std;
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_SIZE = 20;
const char* WINDOW_TITLE = "Snake";
#define INITIAL_SPEED 1
//test thôi nè
const int MAP_WIDTH = SCREEN_WIDTH / CELL_SIZE;  // 600 / 20 = 30 ô
const int MAP_HEIGHT = SCREEN_HEIGHT / CELL_SIZE; // 600 / 20 = 30 ô

vector<vector<int>> map(MAP_HEIGHT, vector<int>(MAP_WIDTH, 0));

//không được thì xóa nè
#endif
