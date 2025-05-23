﻿#ifndef _GRAPHICS__H﻿
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "defs.h"

struct Graphics {
	SDL_Renderer* renderer;
	SDL_Window* window;

	//Ghi lỗi và thoát chương trình
	void logErrorAndExit(const char* msg, const char* error)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
		SDL_Quit();
	}

	//Khởi tạo SDL và tạo cửa sổ
	void initSDL()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			logErrorAndExit("SDL_Init", SDL_GetError());
		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		//full screen
		//window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

		// Khởi tạo thư viện cho 2 loại ảnh PNG và JPG
		if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
			logErrorAndExit("SDL_image error:", IMG_GetError());

		//Hàm tạo renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

		// Xử lí font chữ
		if (TTF_Init() == -1) {
			logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
				TTF_GetError());
		}

	}

	//Chuẩn bị màn hình với nền đen
	void prepareScene()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}

	//Chuẩn bị màn hình với ảnh nền
	void prepareScene(SDL_Texture* background) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, NULL, NULL);
	}

	//Hiển thị màn hình
	void presentScene()
	{
		SDL_RenderPresent(renderer);
	}
	//Hàm nạp ảnh vào texture
	SDL_Texture* loadTexture(const char* filename)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loadding %s", filename);
		SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
		if (texture == NULL) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
		}
		return texture;
	}

	//Hiện ảnh ở vị trí cụ thể
	void renderTexture(SDL_Texture* texture, int x, int y)
	{
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(renderer, texture, NULL, &dest);
	}
	void quit()
	{
		IMG_Quit();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
	}
	//Nạp Font
	TTF_Font* loadFont(const char* path, int size)
	{
		TTF_Font* gFont = TTF_OpenFont(path, size);
		if (gFont == nullptr) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
		}
		return gFont;
	}
	//Tạo chữ với font đã nạp
	SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
	{
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
		if (textSurface == nullptr) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == nullptr) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
				SDL_LOG_PRIORITY_ERROR,
				"Create texture from text %s", SDL_GetError());

		}
		SDL_FreeSurface(textSurface);
		return texture;
	}
	//Đóng font
	void closeFont(TTF_Font** font) {
		if (font && *font) {
			TTF_CloseFont(*font);
			*font = nullptr;
		}
	}
	//Giải phóng chữ
	void quitText(SDL_Texture** text) {
		if (text && *text) {
			SDL_DestroyTexture(*text);
			*text = nullptr;
		}
	}

	//Tạo nút (button) theo kích thước của chữ
	void renderButton(int x, int y, Button& button) {
		// Lấy kích thước chữ từ texture
		int textW, textH;
		SDL_QueryTexture(button.line, NULL, NULL, &textW, &textH);
		// Tạo button có kích thước phù hợp với chữ
		button.rect.x = x;
		button.rect.y = y;
		button.rect.w = textW + 20; // Thêm padding ngang
		button.rect.h = textH + 10; // Thêm padding dọc

		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 256);
		SDL_RenderFillRect(renderer, &button.rect);

		SDL_RenderCopy(renderer, button.line, nullptr, &button.rect);


	}
};

//Kiểm tra nút được click
bool buttonClicked(int mouseX, int mouseY, Button& button) {

	if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
		mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h) {
		return true;
	}
	return false;
}
//Vẽ snake, thức ăn
void render(SDL_Texture* background, const Snake& snake, const SDL_Rect& food, SDL_Texture* cherry, Graphics& graphics) {
	// Đặt màu nền
	graphics.prepareScene(background);
	// Vẽ thức ăn (cherry)
	SDL_RenderCopy(graphics.renderer, cherry, nullptr, &food);

	// Vẽ snake
	int s = snake.body.size();
	if (snake.check_turnNorth()) {
		SDL_RenderCopy(graphics.renderer, snake.headTurnNorth, nullptr, &snake.body[0]);
	}
	if (snake.check_turnSouth()) {
		SDL_RenderCopy(graphics.renderer, snake.headTurnSouth, nullptr, &snake.body[0]);
	}
	if (snake.check_turnEast()) {
		SDL_RenderCopy(graphics.renderer, snake.headTurnEast, nullptr, &snake.body[0]);
	}
	if (snake.check_turnWest()) {
		SDL_RenderCopy(graphics.renderer, snake.headTurnWest, nullptr, &snake.body[0]);
	}
	for (int i = 1; i < s - 1; i++) {
		SDL_RenderCopy(graphics.renderer, snake.bodyImage, nullptr, &snake.body[i]);
	}

	//Vẽ đuôi rắn
	if (snake.tailDirection() == "North") {
		SDL_RenderCopy(graphics.renderer, snake.tailTurnNorth, nullptr, &snake.body.back());
	}
	if (snake.tailDirection() == "South") {
		SDL_RenderCopy(graphics.renderer, snake.tailTurnSouth, nullptr, &snake.body.back());
	}
	if (snake.tailDirection() == "East") {
		SDL_RenderCopy(graphics.renderer, snake.tailTurnEast, nullptr, &snake.body.back());
	}
	if (snake.tailDirection() == "West") {
		SDL_RenderCopy(graphics.renderer, snake.tailTurnWest, nullptr, &snake.body.back());
	}
}

//Tạo food ở vị trí ngẫu nhiên
void spawnFood(SDL_Rect& food) {

	do {
		food.x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
		food.y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
	} while (map[food.y / CELL_SIZE][food.x / CELL_SIZE] == 1);
	food.w = CELL_SIZE; // Đặt chiều rộng
	food.h = CELL_SIZE;  // Đặt chiều cao
}

//Reset game khi người chơi chọn chơi lại
void resetGame(Snake& snake, SDL_Rect& food, Graphics& graphic) {
	snake.resetSnake();
	spawnFood(food);
	graphic.presentScene();
}

//Vẽ map wall
void renderMapWall(vector<vector<int>>& map, Graphics& graphic) {

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map[i][j] == 1) {
				Wall wall(j * CELL_SIZE, i * CELL_SIZE);
				wall.render(graphic.renderer);
			}

		}
	}
}


#endif