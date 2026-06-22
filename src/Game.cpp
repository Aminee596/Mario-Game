#include "Game.h"

void Game::init(SDL_Renderer* renderer, int winW, int winH) {
    player.loadSprites(renderer);
    player.x = 100;
    player.y = winH * 0.78f - player.h;
}

void Game::handleEvent(SDL_Event& event) {}

void Game::update(float dt, int winW, int winH) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    SDL_Rect ground = {0, (int)(winH * 0.78f), winW, winH};
    player.handleInput(keys, dt);
    player.update(dt, gravity, ground);
}

void Game::draw(SDL_Renderer* renderer, int winW, int winH) {
    SDL_Rect ground = {0, (int)(winH * 0.78f), winW, winH};
    SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
    SDL_RenderFillRect(renderer, &ground);
    player.draw(renderer);
}

void Game::free() {
    player.free();
}