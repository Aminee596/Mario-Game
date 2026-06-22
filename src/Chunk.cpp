#include "Chunk.h"
#include <SDL2/SDL_image.h>

Uint32 getPixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    return *(Uint32*)p;
}

    void Chunk::generate(float x, float w, SDL_Surface* surface) {
        startX = x;
        width  = w;
        platforms.clear();
        water.clear();
        coins.clear();
        stairs.clear();
        mysteryBoxes.clear();
        sewers.clear();
        if (!surface) return;
        SDL_LockSurface(surface);

        std::vector<std::vector<bool>> consumed(surface->h, std::vector<bool>(surface->w, false));
        for (int row = 0; row < surface->h; row++) {
            for (int col = 0; col < surface->w; col++) {
                if (consumed[row][col]) continue;
                Uint32 raw = getPixel(surface, col, row);
                Uint8 r, g, b, a;
                SDL_GetRGBA(raw, surface->format, &r, &g, &b, &a);
                if (a < 128) continue;
                SDL_FRect tile = {x + col * 32.0f, row * 32.0f, 32.0f, 32.0f};
                if (r == 127 && g == 0 && b == 0) {platforms.push_back(tile); consumed[row][col] = true;}
                else if (r == 255 && g == 127 && b == 182) {stairs.push_back(tile); consumed[row][col] = true;}
                else if (r == 0 && g == 255 && b == 255) {water.push_back(tile); consumed[row][col] = true;}
                else if (r == 255 && g == 216 && b == 0) {coins.push_back(tile); consumed[row][col] = true;}
                else if (r == 178 && g == 0 && b == 255) {
                    SDL_FRect box = {x + col * 32.0f, row * 32.0f, 64.0f, 64.0f}; MysteryBox mb; mb.rect = box; mysteryBoxes.push_back(mb);
                        for (int dr = 0; dr < 2; dr++)
                            for (int dc = 0; dc < 2; dc++) {
                                int rr = row + dr, cc = col + dc;
                                if (rr < surface->h && cc < surface->w) consumed[rr][cc] = true; }
                }
            }
        }
        for (int row = 0; row < surface->h; row++) {
            for (int col = 0; col < surface->w; col++) {
                if (consumed[row][col]) continue;
                Uint32 raw = getPixel(surface, col, row);
                Uint8 r, g, b, a;
                SDL_GetRGBA(raw, surface->format, &r, &g, &b, &a);
                if (a < 128) continue;
                if (r == 38 && g == 127 && b == 0) {
                    int groundRow = surface->h;
                    for (int scan = row + 1; scan < surface->h; scan++) {
                        if (consumed[scan][col]) { groundRow = scan; break; }
                    }
                    float height = (groundRow - row) * 32.0f;
                    SDL_FRect sewer = {x + col * 32.0f, row * 32.0f, 4 * 32.0f, height};
                    sewers.push_back(sewer);
                    for (int rr = row; rr < groundRow; rr++)
                        for (int dc = 0; dc < 4; dc++) {
                            int cc = col + dc;
                            if (rr < surface->h && cc < surface->w) consumed[rr][cc] = true;
                        }
                }
            }
        } 
        SDL_UnlockSurface(surface);
    }