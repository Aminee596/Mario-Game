#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

enum class PlayerState { Idle, Walking, Jumping, ChargingJump , Falling};

struct Player {
    float x, y, vx, vy;
    float jumpCharge = 0; bool chargingJump = false;
    float speed = 550.0f; float gravity = 200.0f;
    bool onGround = false;
    bool facingLeft = false;
    PlayerState state = PlayerState::Idle;

    float animTimer = 0.0f;
    int walkFrame = 0;

    SDL_Texture* baseAnimation[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    int w = 0, h = 0;

    Player(float startX, float startY) : x(startX), y(startY), vx(0), vy(0) {}

    bool loadSprites(SDL_Renderer* renderer);
    void handleInput(const Uint8* keys, float dt);
    void update(float dt, float gravity, std::vector<SDL_FRect>& platforms);
    void draw(SDL_Renderer* renderer, float cameraX);
    void free();

private:
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
};