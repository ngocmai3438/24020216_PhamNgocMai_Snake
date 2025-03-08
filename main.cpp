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

    SDL_Texture* background = graphic.loadTexture("background.jpg");

    Snake snake;
    SDL_Rect food;
    SDL_Texture* cherry = graphic.loadTexture("apple.png");
    spawnFood(food);

    bool running = true;
    SDL_Event event;


    while (running) {


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || snake.gameOver()) running = false;
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

        render(background, snake, food,cherry, graphic);


        graphic.presentScene();
        SDL_Delay(100);
    }

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(cherry);
    graphic.quit();
    return 0;
}