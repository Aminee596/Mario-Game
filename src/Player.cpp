#include "Player.h"
#include <cmath>

SDL_Texture* Player::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

bool Player::loadSprites(SDL_Renderer* renderer) {
    baseAnimation[0] = loadTexture(renderer, "assets/idle.png");
    baseAnimation[1] = loadTexture(renderer, "assets/walk1.png");
    baseAnimation[2] = loadTexture(renderer, "assets/walk2.png");
    baseAnimation[3] = loadTexture(renderer, "assets/walk3.png");
    baseAnimation[4] = loadTexture(renderer, "assets/jump.png");
    baseAnimation[5] = loadTexture(renderer, "assets/fall.png");
    baseAnimation[6] = loadTexture(renderer, "assets/jumpcharge.png");
    for (int i = 0; i < 7; i++) if (!baseAnimation[i]) return false;
    if (baseAnimation[0]) SDL_QueryTexture(baseAnimation[0], nullptr, nullptr, &w, &h); w *= 2;h *= 2;
    return true;
}

void Player::handleInput(const Uint8* keys, float dt) {
    vx = 0;
    if (keys[SDL_SCANCODE_LEFT])  { vx = -speed; facingLeft = true; }
    if (keys[SDL_SCANCODE_RIGHT]) { vx = speed;  facingLeft = false; }
    if (keys[SDL_SCANCODE_UP] && onGround)  { vy = -500.0f; onGround = false; }

    bool jumpKey = keys[SDL_SCANCODE_SPACE];

    if (jumpKey && onGround) {
        chargingJump = true; jumpCharge += 1500.0f * dt;
        if (jumpCharge > 590.0f) jumpCharge = 590.0f;
    }
    else if (!jumpKey && chargingJump && onGround) {
        vy = -jumpCharge -350.0f; onGround = false; chargingJump = false; jumpCharge = 0;
    }
}

void Player::update(float dt, float gravity, std::vector<SDL_FRect>& platforms) {
    vy += gravity * dt;
    x += vx * dt;
    SDL_FRect pr = {x, y, (float)w, (float)h};
    for (auto& p : platforms) {
        if (pr.x < p.x + p.w && pr.x + pr.w > p.x && pr.y < p.y + p.h && pr.y + pr.h > p.y) {
            if (vx > 0) x = p.x - w;
            else if (vx < 0) x = p.x + p.w;
            pr.x = x;
        }
    }
    y += vy * dt;
    pr = {x, y, (float)w, (float)h};
    onGround = false;
    for (auto& p : platforms) {
        if (pr.x < p.x + p.w && pr.x + pr.w > p.x && pr.y < p.y + p.h && pr.y + pr.h > p.y) {
            if (vy > 0) { y = p.y - h; vy = 0; onGround = true; }
            else if (vy < 0) { y = p.y + p.h; vy = 0; }
            pr.y = y;
        }
    }
    if (!onGround) {
        SDL_FRect feet = {x, y + h, (float)w, 2.0f};
        for (auto& p : platforms) {
            if (feet.x < p.x + p.w && feet.x + feet.w > p.x && feet.y < p.y + p.h && feet.y + feet.h > p.y)
                { onGround = true; break; }
        }
    }

    if (onGround && chargingJump) state = PlayerState::ChargingJump;
    else if (onGround && vx == 0.0f) state = PlayerState::Idle;
    else if (onGround && vx != 0) state = PlayerState::Walking;
    else if (!onGround && vy < 0) state = PlayerState::Jumping;
    else state = PlayerState::Falling;

    if (state == PlayerState::Walking) {
        animTimer += dt;
        if (animTimer >= 0.08f) { walkFrame = (walkFrame + 1) % 3; animTimer = 0; }
    } else {
        animTimer = 0;
        walkFrame = 0;
    }
}

void Player::draw(SDL_Renderer* renderer, float cameraX) {
    SDL_Texture* current = baseAnimation[0];
    if (state == PlayerState::Walking) current = baseAnimation[1 + walkFrame];
    if (state == PlayerState::Jumping) current = baseAnimation[4];
    if (state == PlayerState::Falling) current = baseAnimation[5];
    if (state == PlayerState::ChargingJump) current = baseAnimation[6];
    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect dst = {(int)roundf(x - cameraX), (int)roundf(y), w, h};
    SDL_RenderCopyEx(renderer, current, nullptr, &dst, 0, nullptr, flip);
}

void Player::free() {
    for (int i = 0; i < 7; i++) if (baseAnimation[i]) { SDL_DestroyTexture(baseAnimation[i]); baseAnimation[i] = nullptr; }
}
