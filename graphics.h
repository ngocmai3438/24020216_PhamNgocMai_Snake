#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"

struct Graphics {
	SDL_Renderer* renderer;
	SDL_Window* window;
	
	//các hàm khác

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

		//khởi tạo thư viện cho 2 loại ảnh PNG và JPG _ (để SDL_image hoạt động hiệu quả)
		if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
			logErrorAndExit("SDL_image error:", IMG_GetError());

		//Hàm tạo renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC);
		//Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
		//renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
		if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	
	//cnay ở trong main nma bê vào đây
	void prepareScene()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}
	void prepareScene(SDL_Texture* background) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, NULL, NULL);
	}
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
		//(để SDL_image hoạt động hiệu quả)
		
		IMG_Quit();

		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
};

//Vẽ snake
void render( SDL_Texture* background, const Snake& snake,const SDL_Rect& food, Graphics& graphics) {
	// Đặt màu nền
	graphics.prepareScene(background);
	// Vẽ thức ăn (màu đỏ)
	SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(graphics.renderer, &food);
	// Vẽ từng đoạn của Snake (màu xanh lá)
	SDL_SetRenderDrawColor(graphics.renderer, 0, 100, 0, 255);
	for (const SDL_Rect& segment : snake.body) {
		SDL_RenderFillRect(graphics.renderer, &segment);
	}
}
//Tạo food
void spawnFood(SDL_Rect& food) {
	food.x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
	food.y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
	food.w = CELL_SIZE;
	food.h = CELL_SIZE;
}


#endif
