#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum class MenuResult { None, Start, Quit };

struct Menu {
    SDL_Texture* bg = nullptr;
    SDL_Texture* startBtn = nullptr;
    SDL_Texture* quitBtn = nullptr;

    bool load(SDL_Renderer* renderer);
    MenuResult handleEvent(SDL_Event& event, int winW, int winH);
    void draw(SDL_Renderer* renderer, int winW, int winH);
    void free();

private:
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
    SDL_Rect getStartRect(int winW, int winH);
    SDL_Rect getQuitRect(int winW, int winH);
};