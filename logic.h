#ifndef _LOGIC__H
#define _LOGIC__H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "defs.h"

using namespace std;
struct Snake {
    vector <SDL_Rect> body;
    SDL_Rect head;
    int dx, dy;
    int speed = INITIAL_SPEED;

    Snake() {
        body.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CELL_SIZE, CELL_SIZE });
        head = body.front();
        dx = speed;
        dy = 0;
    }

    void grow() {
        SDL_Rect newSegment = body.back();
        body.push_back(newSegment);
    }
    
    void move() {
        // Tạo một bản sao của phần tử đầu tiên (đầu rắn)
        SDL_Rect newHead = body.front();
        newHead.x += dx * CELL_SIZE;
        newHead.y += dy * CELL_SIZE;

        newHead.x = ((newHead.x % SCREEN_WIDTH) + SCREEN_WIDTH) % SCREEN_WIDTH;
        newHead.y = ((newHead.y % SCREEN_HEIGHT) + SCREEN_HEIGHT) % SCREEN_HEIGHT;


        // Chèn newHead vào đầu danh sách, loại bỏ phần tử cuối cùng
        body.insert(body.begin(), newHead);
        body.pop_back();
    }

    void turnEast(){
        if (dx == 0) {
            dx = speed;
            dy = 0;
        }
    }
    void turnWest(){
        if (dx == 0) {
            dx = -speed;
            dy = 0;
        }
    }
    void turnNorth(){
        if (dy == 0) {
            dy = -speed;
            dx = 0;
        }
    }

    void turnSouth(){
        if (dy == 0) {
            dy = speed;
            dx = 0;
        }
    }
    /*bool checkCollision() {
        SDL_Rect head = body.front();
        if (head.x < 0 || head.x >= SCREEN_WIDTH || head.y < 0 || head.y >= SCREEN_HEIGHT)
            return true;
        for (size_t i = 1; i < body.size(); i++) {
            if (head.x == body[i].x && head.y == body[i].y)
                return true;
        }
        return false;
    }*/
};


#endif

