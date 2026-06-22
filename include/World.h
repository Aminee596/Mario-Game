#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Chunk.h"

struct World {
    std::vector<Chunk> chunks;
    float chunkWidth = 0;
    int   currentTier = 0;
    float cameraX = 0;
    int   winH = 1080;
    void init();
    bool collectCoins(float playerX, float playerY, float playerW, float playerH);
    bool HitMysteryBox(float playerX, float playerY, float playerW, float playerH, float playerVY);
    bool pointInWater(float ptx, float pty);
    bool fullyInWater(float px, float py, float pw, float ph);
    void update(float playerX, int score, int winH);
    void draw(SDL_Renderer* renderer, int winH);
    std::vector<SDL_FRect>& getPlatforms();
    SDL_Texture* barrier = nullptr;
    SDL_Texture* stairs = nullptr;
    SDL_Texture* mysteryBoxes = nullptr; SDL_Texture* usedmysteryBoxes = nullptr;
    SDL_Texture* sewertop = nullptr; SDL_Texture* sewerbottom = nullptr;
    SDL_Texture* ground = nullptr;
    SDL_Texture* water = nullptr;
    SDL_Texture* coins = nullptr;
    bool loadTexture(SDL_Renderer* renderer);
    std::vector<SDL_Surface*> chunkSurfaces;
    bool loadChunkSurfaces();
    void freeSurfaces(); 

private:
    std::vector<SDL_FRect> activePlatforms;
    void cycleForward(int winH);
    void rebuildPlatforms();
};