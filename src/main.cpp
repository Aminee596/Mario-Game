#include <SDL2/SDL.h>
#include "Player.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player player(100, 100);
    if (!player.loadSprite(renderer)) { SDL_Log("Failed to load sprite"); return 1; }
    float gravity = 1000.0f;
    bool isFullscreen = false;

    Uint64 lastTime = SDL_GetTicks64();
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint64 now = SDL_GetTicks64();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);
        SDL_Rect ground = {0, (int)(winH * 0.78f), winW, winH};
        
       
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                isFullscreen = !isFullscreen;
                if (isFullscreen) { SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);} else {SDL_SetWindowFullscreen(window, 0);}
            }       
        }
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_ESCAPE]) running = false;

        player.handleInput(keys, dt);
        player.update(dt, gravity, ground);

        SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
        SDL_RenderFillRect(renderer, &ground);
        player.draw(renderer);
        SDL_RenderPresent(renderer);
    }

    player.free();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}