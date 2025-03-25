#include <iostream>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>
#include "defs.h"
#include "logic.h"
#include "graphics.h"

using namespace std;

int loadHighScore() {
    int highScore = 0; 
    ifstream inFile("highscore.txt");

    if (inFile) {
        if (!(inFile >> highScore)) {  // Nếu file rỗng hoặc dữ liệu không hợp lệ
            highScore = 0;
        }
        inFile.close();
    }
    return highScore;
}
void saveHighScore(int score) {
    ofstream outFile("highscore.txt");
    int currentHighScore = loadHighScore();
    if (score > currentHighScore) {
        if (outFile) {
            outFile << score;
            outFile.close();
        }
    }
}

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
    SDL_Texture* background = graphic.loadTexture("Img/background.jpg");
    graphic.prepareScene(background);

    TTF_Font* font100 = graphic.loadFont("Fonts/Game Paused DEMO.otf", 100);
    SDL_Color colorName = { 204,0,0,0 };
    SDL_Texture* name = graphic.renderText("SNAKE", font100, colorName);

    //Cái font này cho đoạn sau thui
    TTF_Font* font70 = graphic.loadFont("Fonts/Game Paused DEMO.otf", 90);
    
    graphic.renderTexture(name, 200, 170);
    //Enter Speed
    TTF_Font* font30 = graphic.loadFont("Fonts/Game Paused DEMO.otf", 60);
    SDL_Color colorSpeed = { 153,0,0 };
    SDL_Texture* chooseSpeed = graphic.renderText("Choose snake speed", font30, colorSpeed);
    graphic.renderTexture(chooseSpeed, 100, 270);

    Button slow, medium, fast;
    TTF_Font* font20 = graphic.loadFont("Fonts/Game Paused DEMO.otf", 20);
    SDL_Color color = { 204,204,0 };
    slow.line = graphic.renderText("slow", font20, color);
    medium.line = graphic.renderText("medium", font20, color);
    fast.line = graphic.renderText("fast", font20, color);
    graphic.renderButton(110, 350, slow);
    graphic.renderButton(260, 350, medium);
    graphic.renderButton(420, 350, fast);
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

    // Game chính
    Snake snake;
    snake.headTurnNorth = graphic.loadTexture("Img/headNorth.png");
    snake.headTurnSouth = graphic.loadTexture("Img/headSouth.png");
    snake.headTurnWest = graphic.loadTexture("Img/headWest.png");
    snake.headTurnEast = graphic.loadTexture("Img/headEast.png");

    snake.bodyImage = graphic.loadTexture("Img/bodyImage.png");

    snake.tailTurnNorth = graphic.loadTexture("Img/tailNorth.png");
    snake.tailTurnSouth = graphic.loadTexture("Img/tailSouth.png");
    snake.tailTurnWest = graphic.loadTexture("Img/tailWest.png");
    snake.tailTurnEast = graphic.loadTexture("Img/tailEast.png");

    SDL_Rect food;
    SDL_Texture* cherry = graphic.loadTexture("Img/apple.png");
    spawnFood(food);

    int score = 0;
    TTF_Font* numFont30 = graphic.loadFont("Fonts/RetroGaming.ttf", 30);
    SDL_Texture* scoreTexture = nullptr; 

    SDL_Texture* recordTexture = nullptr;

    int highScore = loadHighScore();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        /*graphic.prepareScene(background);*/
        // Hủy texture cũ nếu đã tồn tại
        if (scoreTexture) {
            SDL_DestroyTexture(scoreTexture);
        }
        if (recordTexture) {
            SDL_DestroyTexture(recordTexture);
        }

        // Tạo chuỗi hiển thị điểm số
        string scoreStr = "SCORE: " + to_string(score);
        scoreTexture = graphic.renderText(scoreStr.c_str(), numFont30, colorSpeed);

        // Hiển thị record score
        /*highScore = loadHighScore();*/
        /*string record = "RECORD SCORE: " + to_string(highScore);
        recordTexture = graphic.renderText(record.c_str(), numFont30, colorSpeed);*/

        cout << score << " " << highScore << '\n';
        // Game Over hoặc Lập Record Mới
        if (snake.gameOver()) {
            int oldHighScore = loadHighScore();
            saveHighScore(score);
            highScore = loadHighScore();
            string newRecord = "NEW RECORD: " + to_string(highScore);
            SDL_Texture* win = graphic.renderText(newRecord.c_str(), font70, colorName);
            SDL_Texture* end = graphic.renderText("GAME OVER", font70, colorName);
            if (score > oldHighScore) {
                graphic.renderTexture(win, 70, 200);
            }
            else {   
                graphic.renderTexture(end, 130, 200);
            }
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
            score = 0;
            //destroy texture để giữ lại font
            SDL_DestroyTexture(win); end = nullptr;
            SDL_DestroyTexture(end); end = nullptr;
            SDL_DestroyTexture(again.line); again.line = nullptr;
            SDL_DestroyTexture(exi.line); exi.line = nullptr;
           

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
            score++;            
        }

        render(background, snake, food, cherry, graphic);
        graphic.renderTexture(scoreTexture, 10, 10);
        graphic.presentScene();
        SDL_Delay(speed);
    }

    if (score > loadHighScore()) {
        saveHighScore(highScore);
    } 
    SDL_DestroyTexture(background); background = nullptr;
    SDL_DestroyTexture(cherry); cherry = nullptr;

    snake.quitTexture();

    graphic.quitText(&chooseSpeed, &font30);
    graphic.quitText(&name, &font100);
    graphic.quitText(&name, &font70);
    graphic.quitText(&slow.line, &font20);
    graphic.quitText(&medium.line, &font20);
    graphic.quitText(&fast.line, &font20);

    graphic.quitText(&scoreTexture, &font30);
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }
    TTF_CloseFont(numFont30);
    graphic.quit();

    return 0;
}