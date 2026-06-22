#pragma once
#include <SDL2/SDL.h>
#include <vector>

struct MysteryBox {
    SDL_FRect rect;
    bool used = false;
};

struct Chunk {
    float startX = 0;
    float width  = 0;
    std::vector<SDL_FRect> platforms; std::vector<SDL_FRect> water; std::vector<SDL_FRect> coins; std::vector<SDL_FRect> stairs; std::vector<MysteryBox> mysteryBoxes; std::vector<SDL_FRect> sewers;
    float groundHeightStart = 0;
    float groundHeightEnd = 0;
    bool  hasLeftBarrier = false; 

    void generate(float x, float w, SDL_Surface* surface);
};