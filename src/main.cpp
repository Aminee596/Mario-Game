#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Menu.h"
#include "Game.h"
#include "PauseMenu.h"

enum class AppState { MainMenu, Playing , Paused };

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Menu menu;
    if (!menu.load(renderer)) { SDL_Log("Failed to load menu assets"); return 1; }

    Game game;
    AppState appState = AppState::MainMenu;
    PauseMenu pauseMenu;
    if (!pauseMenu.load(renderer)) { SDL_Log("Failed to load pause assets"); return 1; }
    bool isFullscreen = false;

    Uint64 lastTime = SDL_GetTicks64();
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint64 now = SDL_GetTicks64();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;
        const Uint8* keys = SDL_GetKeyboardState(nullptr);

        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                isFullscreen = !isFullscreen;
                SDL_SetWindowFullscreen(window, isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            if (appState == AppState::MainMenu) {
                MenuResult result = menu.handleEvent(event, winW, winH);
                if (result == MenuResult::Start) {
                    game.init(renderer, winW, winH);
                    appState = AppState::Playing;
                }
                if (result == MenuResult::Quit) running = false;
            }
            if (appState == AppState::Playing) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    appState = AppState::Paused;
                }
            }
            else if (appState == AppState::Paused) {
                PauseResult result = pauseMenu.handleEvent(event, winW, winH);
                if (result == PauseResult::Resume || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    appState = AppState::Playing;
                }
            if (result == PauseResult::MainMenu) {
                appState = AppState::MainMenu;
                }
            }
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (appState == AppState::MainMenu) {
            menu.draw(renderer, winW, winH);
        } else if (appState == AppState::Paused) {
            game.draw(renderer, winW, winH);
            pauseMenu.draw(renderer, winW, winH);
        } else {
            game.update(dt, winW, winH);
            game.draw(renderer, winW, winH);
        }

        SDL_RenderPresent(renderer);
    }

    menu.free();
    game.free();
    pauseMenu.free();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}