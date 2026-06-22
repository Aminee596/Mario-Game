#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Menu.h"
#include "Game.h"
#include "PauseMenu.h"

enum class AppState { MainMenu, Playing, Paused, Dead};

const int GAME_W = 1920;
const int GAME_H = 1080;
const int Score = 0;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { SDL_Log("SDL_Init failed: %s", SDL_GetError()); return 1; }
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) { SDL_Log("IMG_Init failed: %s", IMG_GetError()); return 1; }
    SDL_Window* window = SDL_CreateWindow("Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, GAME_W, GAME_H);

    Menu menu; if (!menu.load(renderer)) { SDL_Log("Failed to load menu assets"); return 1; }
    PauseMenu pauseMenu; if (!pauseMenu.load(renderer)) { SDL_Log("Failed to load pause assets"); return 1; }

    Game game; AppState appState = AppState::MainMenu; bool isFullscreen = false;

    Uint64 lastTime = SDL_GetTicks64();
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint64 now = SDL_GetTicks64();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                isFullscreen = !isFullscreen;
                SDL_SetWindowFullscreen(window, isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            if (appState == AppState::MainMenu) {
                MenuResult result = menu.handleEvent(event, GAME_W, GAME_H);
                if (result == MenuResult::Start) {
                    game.free();
                    game.init(renderer, GAME_W, GAME_H);
                    appState = AppState::Playing;
                }
                if (result == MenuResult::Quit) running = false;
            }
            else if (appState == AppState::Playing) {
                GameResult result = game.handleEvent(event, GAME_W, GAME_H);
                if (result == GameResult::Pause || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    appState = AppState::Paused;
                }
            }
            else if (appState == AppState::Dead) {
                PauseResult result = pauseMenu.handleEvent(event, GAME_W, GAME_H , true);
                if (result == PauseResult::Restart) {
                    game.free();
                    game.init(renderer, GAME_W, GAME_H);
                    appState = AppState::Playing;
                }
                if (result == PauseResult::MainMenu) {
                    appState = AppState::MainMenu;
                }
                
            }
            else if (appState == AppState::Paused) {
                PauseResult result = pauseMenu.handleEvent(event, GAME_W, GAME_H, false);
                if (result == PauseResult::Resume || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    appState = AppState::Playing;
                }
                if (result == PauseResult::MainMenu) appState = AppState::MainMenu;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (appState == AppState::MainMenu) {
            menu.draw(renderer, GAME_W, GAME_H);
        } else if (appState == AppState::Paused) {
            game.draw(renderer, GAME_W, GAME_H);
            pauseMenu.draw(renderer, GAME_W, GAME_H, false);
        } else if (appState == AppState::Dead) {
            game.draw(renderer, GAME_W, GAME_H);
            pauseMenu.draw(renderer, GAME_W, GAME_H, true);
        } else {
            GameResult result = game.update(dt, GAME_W, GAME_H);
            if (result == GameResult::Death) {
                appState = AppState::Dead;
            }
            game.draw(renderer, GAME_W, GAME_H);
        }

        SDL_RenderPresent(renderer);
    }

    menu.free();
    pauseMenu.free();
    game.free();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
