#include <iostream>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include "defs.h"
#include "logic.h"
#include "graphics.h"

using namespace std;

int main(int argc, char* argv[]) {
    Graphics graphic;
    graphic.initSDL();

    Snake snake;
    SDL_Rect food;
    spawnFood(food);

    bool running = true;
    SDL_Event event;
    
   
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_UP]) snake.turnNorth();
        if (currentKeyStates[SDL_SCANCODE_DOWN]) snake.turnSouth();
        if (currentKeyStates[SDL_SCANCODE_LEFT]) snake.turnWest();
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) snake.turnEast();

        snake.move();
        
        if (snake.eatFood(food)) {
            snake.grow();
            spawnFood(food);
        }

        render(snake, food, graphic);

        graphic.presentScene();
        SDL_Delay(100);
    }

    /*if (SDL_GetTicks() - lastMove > 100) {
        snake.move();
        lastMove = SDL_GetTicks();

        if (snake.body.front().x == food.x && snake.body.front().y == food.y) {
            snake.grow();
            food.x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
            food.y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
        }

        if (snake.checkCollision()) running = false;
    }*/

  

    graphic.quit();
    return 0;
}
