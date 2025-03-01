#ifndef _DEFS__H
#define _DEFS__H
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_SIZE = 20;
const char* WINDOW_TITLE = "Snake";
SDL_Rect food = { rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE, rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE, CELL_SIZE, CELL_SIZE };
#define INITIAL_SPEED 1

#endif
