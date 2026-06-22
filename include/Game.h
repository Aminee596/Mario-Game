#pragma once
#include <SDL2/SDL.h>
#include "Player.h"

struct Game {
    Player player;
    float gravity = 1000.0f;

    Game() : player(0, 0) {}

    void init(SDL_Renderer* renderer, int winW, int winH);
    void handleEvent(SDL_Event& event);
    void update(float dt, int winW, int winH);
    void draw(SDL_Renderer* renderer, int winW, int winH);
    void free();
};