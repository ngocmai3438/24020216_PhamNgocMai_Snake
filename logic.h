#ifndef _LOGIC__H
#define _LOGIC__H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "defs.h"

using namespace std;

bool overlap(const SDL_Rect& r1, const SDL_Rect& r2) {
    return SDL_HasIntersection(&r1, &r2);
}
struct Snake {
    vector <SDL_Rect> body;
    SDL_Rect head;
    int dx, dy;
    // Ảnh
    SDL_Texture* headTurnEast;
    SDL_Texture* headTurnWest;
    SDL_Texture* headTurnSouth;
    SDL_Texture* headTurnNorth;
    SDL_Texture* bodyImage;
    SDL_Texture* tailTurnEast;
    SDL_Texture* tailTurnWest;
    SDL_Texture* tailTurnSouth;
    SDL_Texture* tailTurnNorth;



    int speed = INITIAL_SPEED;

    Snake() {
        body.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CELL_SIZE, CELL_SIZE });
        head = body.front();
        body.push_back(head);

       
        dx = speed;
        dy = 0;
    }

    void resetSnake() {
        body = { { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CELL_SIZE, CELL_SIZE } };
        head = body.front();
        body.push_back(head);


        dx = speed;
        dy = 0;
    }
    void grow() {
        SDL_Rect newSegment = body.back();
        if (tailDirection() == "East") newSegment.x -= CELL_SIZE; // Rắn đi phải, đuôi thêm vào bên trái
        else if (tailDirection() == "West") newSegment.x += CELL_SIZE; // Rắn đi trái, đuôi thêm vào bên phải
        else if (tailDirection() == "South") newSegment.y -= CELL_SIZE; // Rắn đi xuống, đuôi thêm vào trên
        else if (tailDirection() == "North") newSegment.y += CELL_SIZE; // Rắn đi lên, đuôi thêm vào dưới
        body.push_back(newSegment);
       
    }

    void move() {
        // Tạo một bản sao của phần tử đầu tiên (đầu rắn)
        SDL_Rect newHead = body.front();
        newHead.x += dx * CELL_SIZE;
        newHead.y += dy * CELL_SIZE;

        newHead.x = ((newHead.x % SCREEN_WIDTH) + SCREEN_WIDTH) % SCREEN_WIDTH;
        newHead.y = ((newHead.y % SCREEN_HEIGHT) + SCREEN_HEIGHT) % SCREEN_HEIGHT;

        //Kiểm tra va chạm trước khi cập nhật vị trí
        if (gameOver()) return;
        // Chèn newHead vào đầu danh sách, loại bỏ phần tử cuối cùng
        body.insert(body.begin(), newHead);
        body.pop_back();
    }

    void turnEast() {
        if (dx == 0) {
            dx = speed;
            dy = 0;
        }
    }
    bool check_turnEast() const {
        if (dx == speed && dy == 0) return true;
        else return false;
    }
    void turnWest() {
        if (dx == 0) {
            dx = -speed;
            dy = 0;
        }
    }
    bool check_turnWest() const {
        if (dx == -speed && dy == 0) return true;
        else return false;
    }
    void turnNorth() {
        if (dy == 0) {
            dy = -speed;
            dx = 0;
        }
    }
    bool check_turnNorth() const {
        if (dy == -speed && dx == 0) return true;
        else return false;
    }
    void turnSouth() {
        if (dy == 0) {
            dy = speed;
            dx = 0;
        }
    }
    bool check_turnSouth() const {
        if (dy == speed && dx == 0) return true;
        else return false;
    }
    bool eatFood(SDL_Rect food) {
        SDL_Rect newHead = body.front();
        return overlap(newHead, food);
    }

    bool gameOver() {
        bool rel = false;
        SDL_Rect newHead = body.front();
        newHead.x += dx * CELL_SIZE;
        newHead.y += dy * CELL_SIZE;
        for (size_t i = 1; i < body.size(); i++) {
            if (SDL_HasIntersection(&newHead, &body[i])) rel = true;
        }
        return rel;
        
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

    //Xác định hướng của đuôi rắn
    string tailDirection() const {
        if (body.size() < 2) return "Unknown";
        SDL_Rect tail = body.back();
        SDL_Rect pretail = body[body.size() - 2];
        if (tail.x == pretail.x && tail.y < pretail.y) return "South";
        if (tail.x == pretail.x && tail.y > pretail.y) return "North";
        if (tail.x > pretail.x && tail.y == pretail.y) return "West";
        if (tail.x < pretail.x && tail.y == pretail.y) return "East";
        return "Unknown";
    }
    void quitTexture() {
        
        SDL_DestroyTexture(headTurnNorth); headTurnNorth = nullptr;
        SDL_DestroyTexture(headTurnSouth); headTurnSouth = nullptr;
        SDL_DestroyTexture(headTurnEast); headTurnEast = nullptr;
        SDL_DestroyTexture(headTurnWest); headTurnWest = nullptr;

        SDL_DestroyTexture(bodyImage); bodyImage = nullptr;

        SDL_DestroyTexture(tailTurnNorth); tailTurnNorth = nullptr;
        SDL_DestroyTexture(tailTurnSouth); tailTurnSouth = nullptr;
        SDL_DestroyTexture(tailTurnWest); tailTurnWest = nullptr;
        SDL_DestroyTexture(tailTurnEast); tailTurnEast = nullptr;
    }
};

struct Button {
    SDL_Texture* line; //dòng chữ
    SDL_Rect rect; //vị trí button
    Button() {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0;
        rect.h = 0;

    }
};
#endif
