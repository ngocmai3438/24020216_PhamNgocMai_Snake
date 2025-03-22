#include <iostream>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "logic.h"
#include "graphics.h"

using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(200);
    }
}

int main(int argc, char* argv[]) {

    Graphics graphic;
    graphic.initSDL();
    int speed;

    //Mở đầu
    SDL_Texture* background = graphic.loadTexture("background.jpg");
    graphic.prepareScene(background);

    TTF_Font* font70 = graphic.loadFont("Game Paused DEMO.otf", 70);
    SDL_Color colorName = { 204,0,0,0 };
    SDL_Texture* name = graphic.renderText("SNAKE", font70, colorName);
    graphic.renderTexture(name, 200, 170);
    //Enter Speed
    TTF_Font* font30 = graphic.loadFont("Game Paused DEMO.otf", 30);
    SDL_Color colorSpeed = { 153,0,0 };
    SDL_Texture* chooseSpeed = graphic.renderText("Choose snake speed", font30, colorSpeed);
    graphic.renderTexture(chooseSpeed, 160, 250);

    Button slow, medium, fast;
    TTF_Font* font20 = graphic.loadFont("Game Paused DEMO.otf", 20);
    SDL_Color color = { 204,204,0 };
    slow.line = graphic.renderText("slow", font20, color);
    medium.line = graphic.renderText("medium", font20, color);
    fast.line = graphic.renderText("fast", font20, color);
    graphic.renderButton(110, 300, slow);
    graphic.renderButton(260, 300, medium);
    graphic.renderButton(420, 300, fast);
    graphic.presentScene();

    bool firstFrame = true;
    SDL_Event firstEvent;
    int x, y;

    bool running = true;
    SDL_Event event;

    while (firstFrame) {
        SDL_WaitEvent(&firstEvent);

        switch (firstEvent.type) {
        case SDL_QUIT:
            firstFrame = false;
            running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            if (buttonClicked(x, y, slow)) {
                speed = 280;
                firstFrame = false;
            }
            else if (buttonClicked(x, y, medium)) {
                speed = 200;
                firstFrame = false;
            }
            else if (buttonClicked(x, y, fast)) {
                speed = 100;
                firstFrame = false;
            }
            break;
        }
        SDL_Delay(100);
    }

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

    

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        if (snake.gameOver()) {
            SDL_Texture* end = graphic.renderText("GAME OVER", font70, colorName);
            graphic.renderTexture(end, 130, 200);
            graphic.presentScene();

            Button again, exi;
            again.line = graphic.renderText("again", font20, color);
            exi.line = graphic.renderText("exit", font20, color);
            graphic.renderButton(180, 300, again);
            graphic.renderButton(380, 300, exi);

            graphic.presentScene();

            bool gameOverLoop = true;
            while (gameOverLoop) {
                SDL_WaitEvent(&event);
                switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    gameOverLoop = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&x, &y);
                    if (buttonClicked(x, y, again)) {
                        resetGame(snake, food, graphic);
                        gameOverLoop = false;
                    }
                    else if (buttonClicked(x, y, exi)) {
                        
                        gameOverLoop = false;
                        running = false;
                    }
                    break;
                }
            }
            graphic.quitText(&end, &font70);
            graphic.quitText(&again.line, &font20);
            graphic.quitText(&exi.line, &font20);
           
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

        render(background, snake, food, cherry, graphic);

        graphic.presentScene();
        SDL_Delay(speed);
    }

    SDL_DestroyTexture(background); background = nullptr;
    SDL_DestroyTexture(cherry); cherry = nullptr;

    snake.quitTexture();

    graphic.quitText(&name, &font70);
    graphic.quitText(&slow.line, &font20);
    graphic.quitText(&medium.line, &font20);
    graphic.quitText(&fast.line, &font20);

    graphic.quit();

    return 0;
}