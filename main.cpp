#include <iostream>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>
#include <cstdlib> 
#include <ctime> 
#include "defs.h"
#include "logic.h"
#include "graphics.h"

using namespace std;

int loadHighScore(const string& filename) {
    int highScore = 0;
    ifstream inFile(filename);

    if (inFile) {
        if (!(inFile >> highScore)) {
            highScore = 0;
        }
        inFile.close();
    }
    return highScore;
}
void saveHighScore(const string& filename,int score) {
    int currentHighScore = loadHighScore(filename);
    if (score > currentHighScore) {
        ofstream outFile(filename);
        if (outFile) {
            outFile << score;
            outFile.close();
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    Graphics graphic;
    graphic.initSDL();

    bool game = true;
    do {
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
        graphic.renderTexture(chooseSpeed, 100, 290);

        Button slow, medium, fast;
        TTF_Font* font20 = graphic.loadFont("Fonts/Game Paused DEMO.otf", 20);
        SDL_Color color = { 204,204,0 };
        slow.line = graphic.renderText("slow", font20, color);
        medium.line = graphic.renderText("medium", font20, color);
        fast.line = graphic.renderText("fast", font20, color);
        graphic.renderButton(110, 360, slow);
        graphic.renderButton(260, 360, medium);
        graphic.renderButton(420, 360, fast);

        //Chọn round
        Button easy, hard;
        easy.line = graphic.renderText("EASY", font20, color);
        hard.line = graphic.renderText("HARD", font20, color);
        graphic.renderButton(200, 255, easy);
        graphic.renderButton(350, 255, hard);

        graphic.presentScene();

        bool firstFrame = true;
        SDL_Event firstEvent;
        int x, y;

        SDL_Event event;
        bool speedClicked = false;
        bool roundClicked = false;
        bool round1 = false;
        bool round2 = false;
        while (firstFrame) {
            SDL_WaitEvent(&firstEvent);

            switch (firstEvent.type) {
            case SDL_QUIT:
                firstFrame = false;
                /*game = false;*/
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);

                // Chọn tốc độ
                if (buttonClicked(x, y, slow)) {
                    speed = 280;
                    speedClicked = true;
                }
                else if (buttonClicked(x, y, medium)) {
                    speed = 200;
                    speedClicked = true;
                }
                else if (buttonClicked(x, y, fast)) {
                    speed = 100;
                    speedClicked = true;
                }

                // Chọn độ khó
                if (buttonClicked(x, y, easy)) {
                    round1 = true;
                    round2 = false;
                    roundClicked = true;

                }
                else if (buttonClicked(x, y, hard)) {
                    round1 = false;
                    round2 = true;
                    roundClicked = true;

                }
                break;
            }
            if (speedClicked && roundClicked) firstFrame = 0;
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

        string filename = round1 ? "highscore1.txt" : "highscore2.txt";
        int highScore = loadHighScore(filename);
        
        while (round2 || round1) {
            // Vẽ map
            if (round2) {
                for (int i = 6; i < 14; i++) {
                    map[6][i] = 1;
                    map[i][6] = 1;
                }
                for (int i = 24; i > 15; i--) {
                    map[24][i] = 1;
                    map[i][24] = 1;
                }
                for (int i = 17; i < 20; i++) {
                    map[10][i] = 1;
                    map[i][10] = 1;
                }
                for (int i = 10; i < 14; i++) {
                    map[20][i] = 1;
                    map[i][20] = 1;
                }
            }
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    round2 = false;
                    game = false;
                }
            }

            // Hủy texture score và record score cũ nếu đã tồn tại
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
            string record = "RECORD SCORE: " + to_string(highScore);
            recordTexture = graphic.renderText(record.c_str(), numFont30, colorSpeed);

            // Game Over hoặc Lập Record Mới
            if (snake.gameOver()) {
                int oldHighScore = loadHighScore(filename);
                saveHighScore(filename,score);
                highScore = loadHighScore(filename);
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
                        round2 = false;
                        gameOverLoop = false;
                        game = false;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        SDL_GetMouseState(&x, &y);
                        if (buttonClicked(x, y, again)) {
                            resetGame(snake, food, graphic);
                            gameOverLoop = false;
                        }
                        else if (buttonClicked(x, y, exi)) {

                            gameOverLoop = false;
                            round1 = false;
                            round2 = false;
                            game = false;
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
            graphic.renderTexture(recordTexture, 10, 50);
            renderMapWall(map, graphic);

            graphic.presentScene();
            SDL_Delay(speed);
        }


        //Lưu điểm
        saveHighScore(filename,highScore);


        //Destroy mọi thứ
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_HEIGHT; j++) {
                map[i][j] = 0;
            }
        }

        SDL_DestroyTexture(background); background = nullptr;
        SDL_DestroyTexture(cherry); cherry = nullptr;

        snake.quitTexture();

        TTF_CloseFont(font100);
        TTF_CloseFont(font70);
        TTF_CloseFont(font30);
        TTF_CloseFont(font20);
        TTF_CloseFont(numFont30);

        graphic.quitText(&chooseSpeed);
        graphic.quitText(&name);
        graphic.quitText(&slow.line);
        graphic.quitText(&medium.line);
        graphic.quitText(&fast.line);
        graphic.quitText(&easy.line);
        graphic.quitText(&hard.line);

        graphic.quitText(&recordTexture);
        graphic.quitText(&scoreTexture);

    } while (game);
    graphic.quit();
    return 0;
}

