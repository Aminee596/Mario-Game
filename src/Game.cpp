#include "Game.h"
#include "World.h"

void Game::init(SDL_Renderer* renderer, int winW, int winH) {
    player.loadSprites(renderer);
    loadPauseButton(renderer);
    world.loadChunkSurfaces();
    world.loadTexture(renderer);
    world.init();
    score = 0;
    player.x = 100;
    player.y = winH * 0.80f;
}

bool Game::loadPauseButton(SDL_Renderer* renderer) {
    pauseBtn = IMG_LoadTexture(renderer, "assets/pausebutton.png");
    return pauseBtn != nullptr;
}

SDL_Rect Game::getPauseRect(int winW, int winH) {
    int side = (int)(winH * 0.05f);
    return {(winW - side) / 2, (int)(winH * 0.035f), side, side};
}

GameResult Game::handleEvent(SDL_Event& event, int winW, int winH) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;
        SDL_Rect pr = getPauseRect(winW, winH);
        if (mx >= pr.x && mx <= pr.x + pr.w && my >= pr.y && my <= pr.y + pr.h) {
            return GameResult::Pause;
        }
    }
    return GameResult::None;
}

GameResult Game::update(float dt, int winW, int winH) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    player.handleInput(keys, dt);
    world.update(player.x, score, winH);
    float vyBefore = player.vy;
    player.update(dt, gravity, world.getPlatforms());
    world.HitMysteryBox(player.x, player.y, player.w, player.h, vyBefore);
    if (world.collectCoins(player.x, player.y, player.w, player.h)) { score += 10; }
    if (world.fullyInWater(player.x, player.y, player.w, player.h)) { return GameResult::Death; }
    return GameResult::None;
}

void Game::draw(SDL_Renderer* renderer, int winW, int winH) {
    SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255);
    SDL_RenderClear(renderer);
    world.draw(renderer, winH);
    player.draw(renderer, world.cameraX);
    SDL_Rect pr = getPauseRect(winW, winH);
    SDL_RenderCopy(renderer, pauseBtn, nullptr, &pr);
}

void Game::free() {
    player.free();
    world.freeSurfaces();
    if (pauseBtn) { SDL_DestroyTexture(pauseBtn); pauseBtn = nullptr; }
}