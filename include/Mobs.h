#pragma once
#include <SDL2/SDL.h>
#include <vector>

enum class MobType { Poop, Turtle, Boss };

struct Mob {
    float x, y, vx, vy, w, h;
    bool alive = true;
    bool facingLeft = false;
    int health = 1;
    MobType type = MobType::Poop;
    static SDL_Texture* Poomob ;
    static SDL_Texture* Turtlemob ;
    static SDL_Texture* Boss;
    static void updateMobs(std::vector<Mob>& mobs, float dt, float gravity, std::vector<SDL_FRect>& platforms);
    static void loadSprites(SDL_Renderer* renderer);
};
