#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Player {
    float x, y, vy;
    float speed = 500.0f;
    bool onGround = false;
    bool facingRight = true;

    SDL_Texture* texture = nullptr;
    int w = 0, h = 0;

    Player(float startX, float startY) : x(startX), y(startY), vy(0) {}

    bool loadSprite(SDL_Renderer* renderer);
    void handleInput(const Uint8* keys, float dt);
    void update(float dt, float gravity, SDL_Rect ground);
    void draw(SDL_Renderer* renderer);
    void free();
};