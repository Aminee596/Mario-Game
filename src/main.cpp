#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Mario",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1920, 1080,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    float x = 100, y = 100;
    float speed = 500.0f;
    float vy = 0;          
    float gravity = 1000.0f;
    Uint64 lastTime = SDL_GetTicks64();
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint64 now = SDL_GetTicks64();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        while (SDL_PollEvent(&event)) {if (event.type == SDL_QUIT) running = false;}
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_LEFT])  x -= speed * dt;
        if (keys[SDL_SCANCODE_RIGHT]) x += speed * dt;
        if (keys[SDL_SCANCODE_ESCAPE]) running = false;
        SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 20, 5, 5, 25);SDL_Rect ground = {(int)0, (int)842, 1920, 432};SDL_RenderFillRect(renderer,&ground);
        SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
        SDL_Rect player = {(int)x, (int)y, 32, 48};
        vy += gravity * dt; y  += vy * dt; if (y >= 800) {y  = 800;vy = 0;}
        if(keys[SDL_SCANCODE_SPACE] && y>=800) vy = -600.0f;
        SDL_RenderFillRect(renderer, &player);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}