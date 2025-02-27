#include <SDL.h>
#include <vector>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Snake {
    std::vector<SDL_Rect> body;
    Direction dir;

    Snake() {
        body.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CELL_SIZE, CELL_SIZE });
        dir = RIGHT;
    }

    void move() {
        SDL_Rect newHead = body.front();
        switch (dir) {
        case UP: newHead.y -= CELL_SIZE; break;
        case DOWN: newHead.y += CELL_SIZE; break;
        case LEFT: newHead.x -= CELL_SIZE; break;
        case RIGHT: newHead.x += CELL_SIZE; break;
        }
        body.insert(body.begin(), newHead);
        body.pop_back();
    }

    void grow() {
        SDL_Rect newSegment = body.back();
        body.push_back(newSegment);
    }

    bool checkCollision() {
        SDL_Rect head = body.front();
        if (head.x < 0 || head.x >= SCREEN_WIDTH || head.y < 0 || head.y >= SCREEN_HEIGHT)
            return true;
        for (size_t i = 1; i < body.size(); i++) {
            if (head.x == body[i].x && head.y == body[i].y)
                return true;
        }
        return false;
    }
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Snake snake;
    SDL_Rect food = { rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE, rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE, CELL_SIZE, CELL_SIZE };

    bool running = true;
    SDL_Event event;
    Uint32 lastMove = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP: if (snake.dir != DOWN) snake.dir = UP; break;
                case SDLK_DOWN: if (snake.dir != UP) snake.dir = DOWN; break;
                case SDLK_LEFT: if (snake.dir != RIGHT) snake.dir = LEFT; break;
                case SDLK_RIGHT: if (snake.dir != LEFT) snake.dir = RIGHT; break;
                }
            }
        }

        if (SDL_GetTicks() - lastMove > 100) {
            snake.move();
            lastMove = SDL_GetTicks();

            if (snake.body.front().x == food.x && snake.body.front().y == food.y) {
                snake.grow();
                food.x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
                food.y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
            }

            if (snake.checkCollision()) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &food);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (auto& segment : snake.body) {
            SDL_RenderFillRect(renderer, &segment);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
