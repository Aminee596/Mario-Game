#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum class PauseResult { None, Resume, MainMenu };

struct PauseMenu {
    SDL_Texture* resumeBtn = nullptr;
    SDL_Texture* menuBtn = nullptr;

    bool load(SDL_Renderer* renderer);
    PauseResult handleEvent(SDL_Event& event, int winW, int winH);
    void draw(SDL_Renderer* renderer, int winW, int winH);
    void free();

private:
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
    SDL_Rect getResumeRect(int winW, int winH);
    SDL_Rect getMenuRect(int winW, int winH);
};