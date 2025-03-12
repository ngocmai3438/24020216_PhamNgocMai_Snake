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
    snake.headTurnNorth = graphic.loadTexture("headNorth.png");
    snake.headTurnSouth = graphic.loadTexture("headSouth.png");
    snake.headTurnWest = graphic.loadTexture("headWest.png");
    snake.headTurnEast = graphic.loadTexture("headEast.png");

    snake.bodyImage = graphic.loadTexture("bodyImage.png");

    snake.tailTurnNorth = graphic.loadTexture("tailNorth.png");
    snake.tailTurnSouth = graphic.loadTexture("tailSouth.png");
    snake.tailTurnWest = graphic.loadTexture("tailWest.png");
    snake.tailTurnEast = graphic.loadTexture("tailEast.png");

    SDL_Rect food;
    SDL_Texture* cherry = graphic.loadTexture("apple.png");

    spawnFood(food);

    bool running = true;
    SDL_Event event;


    while (running) {


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        if (snake.gameOver()) running = false;
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
        SDL_Delay(150);
    }

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(cherry);
    SDL_DestroyTexture(snake.headTurnNorth);
    SDL_DestroyTexture(snake.headTurnSouth);
    SDL_DestroyTexture(snake.headTurnEast);
    SDL_DestroyTexture(snake.headTurnWest);

    SDL_DestroyTexture(snake.bodyImage);

    SDL_DestroyTexture(snake.tailTurnNorth);
    SDL_DestroyTexture(snake.tailTurnSouth);
    SDL_DestroyTexture(snake.tailTurnWest);
    SDL_DestroyTexture(snake.tailTurnEast);

    graphic.quit();
    return 0;
}