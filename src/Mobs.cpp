#include "Mobs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* Mob::Poomob = nullptr;
SDL_Texture* Mob::Turtlemob = nullptr;
SDL_Texture* Mob::Boss = nullptr;

void Mob::loadSprites(SDL_Renderer* renderer) {
    Poomob = IMG_LoadTexture(renderer, "assets/poo.png");
    Turtlemob = IMG_LoadTexture(renderer, "assets/turtle.png");
    Boss = IMG_LoadTexture(renderer, "assets/Boss.png");

}

void Mob::updateMobs(std::vector<Mob>& mobs, float dt, float gravity, std::vector<SDL_FRect>& platforms) {
    for (auto& mob : mobs) {
        if (!mob.alive) continue;

        mob.vy += gravity * dt;
        mob.x += mob.vx * dt;
        SDL_FRect mr = {mob.x, mob.y, mob.w, mob.h};
        for (auto& p : platforms) {
            if (SDL_HasIntersectionF(&mr, &p)) {
                if (mob.vx > 0) mob.x = p.x - mob.w;
                else if (mob.vx < 0) mob.x = p.x + p.w;
                mob.vx = -mob.vx;
                mob.facingLeft = !mob.facingLeft;
                mr = {mob.x, mob.y, mob.w, mob.h};
            }
        }

        float aheadX = mob.facingLeft ? mob.x - 1 : mob.x + mob.w + 1;
        float belowY = mob.y + mob.h + 2;
        bool groundAhead = false;
        for (auto& p : platforms) {
            if (aheadX >= p.x && aheadX <= p.x + p.w && belowY >= p.y && belowY <= p.y + p.h) {
                groundAhead = true;
                break;
            }
        }
        if (mob.vy == 0 && !groundAhead) {
            mob.vx = -mob.vx;
            mob.facingLeft = !mob.facingLeft;
        }

        mob.y += mob.vy * dt;
        mr = {mob.x, mob.y, mob.w, mob.h};
        for (auto& p : platforms) {
            if (SDL_HasIntersectionF(&mr, &p)) {
                if (mob.vy > 0) { mob.y = p.y - mob.h; mob.vy = 0; }
                else if (mob.vy < 0) { mob.y = p.y + p.h; mob.vy = 0; }
                mr = {mob.x, mob.y, mob.w, mob.h};
            }
        }
    }
}