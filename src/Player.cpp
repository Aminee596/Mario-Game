#include "Player.h"
#include <cmath>

bool Player::loadSprite(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("assets/marioidle.png");
    if (!surface) return false;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    w = surface->w;
    h = surface->h;
    SDL_FreeSurface(surface);
    return texture != nullptr;
}

void Player::handleInput(const Uint8* keys, float dt) {
    if (keys[SDL_SCANCODE_LEFT]) { x -= speed * dt; facingRight = true; }
    if (keys[SDL_SCANCODE_RIGHT]) { x += speed * dt; facingRight = false; }
    if (keys[SDL_SCANCODE_SPACE] && onGround) { vy = -600.0f; onGround = false; }
}

void Player::update(float dt, float gravity, SDL_Rect ground) {
    vy += gravity * dt;
    y += vy * dt;
    SDL_Rect player = {(int)roundf(x), (int)roundf(y), w, h};
    if (vy > 0 && player.y + player.h >= ground.y) { y = ground.y - h; vy = 0; onGround = true; }
    else onGround = false;
}

void Player::draw(SDL_Renderer* renderer) {
    SDL_Rect dst = {(int)roundf(x), (int)roundf(y), w, h};
    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, texture, nullptr, &dst, 0, nullptr, flip);
}

void Player::free() {
    if (texture) { SDL_DestroyTexture(texture); texture = nullptr; }
}