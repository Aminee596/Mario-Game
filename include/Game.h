#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.h"
#include "World.h"

enum class GameResult { None, Pause, Death};

struct Game {
    Player player;
    float gravity = 1000.0f;
    SDL_Texture* pauseBtn = nullptr;
    World world;
    int score = 0;

    Game() : player(0, 0) {}

    void init(SDL_Renderer* renderer, int winW, int winH);
    bool loadPauseButton(SDL_Renderer* renderer);
    GameResult handleEvent(SDL_Event& event, int winW, int winH);
    GameResult update(float dt, int winW, int winH);
    void draw(SDL_Renderer* renderer, int winW, int winH);
    void free();

private:
    SDL_Rect getPauseRect(int winW, int winH);
};