    #include "Player.h"
    #include <cmath>

    SDL_Texture* Player::loadTexture(SDL_Renderer* renderer, const char* path) {
        SDL_Surface* surface = IMG_Load(path);
        if (!surface) return nullptr;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        w = surface->w;
        h = surface->h;
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
        for (int i = 0; i < 6; i++) if (!baseAnimation[i]) return false;
        return true;
    }

    void Player::handleInput(const Uint8* keys, float dt) {
        if (keys[SDL_SCANCODE_LEFT]) { vx = -speed; facingLeft = true; }
        if (keys[SDL_SCANCODE_RIGHT]) { vx = speed;  facingLeft = false; }
        if (keys[SDL_SCANCODE_SPACE] && onGround) { vy = -600.0f; onGround = false; }
    }

    void Player::update(float dt, float gravity, SDL_Rect ground) {
        vy += gravity * dt;
        x += vx * dt;
        y += vy * dt;

        SDL_Rect player = {(int)roundf(x), (int)roundf(y), w, h};
        if (player.y + player.h >= ground.y) { y = ground.y - h; vy = 0; onGround = true; }
        else onGround = false;

        if (onGround && vx == 0.0f) state = PlayerState::Idle;
        else if (onGround && vx != 0) state = PlayerState::Walking;
        else if (!onGround && vy < 0) state = PlayerState::Jumping;
        else state = PlayerState::Falling;

        if (state == PlayerState::Walking) {
            animTimer += dt;
            if (animTimer >= 0.12f) { walkFrame = (walkFrame + 1) % 3; animTimer = 0; }
        } else {
            animTimer = 0;
            walkFrame = 0;
        }
    }

    void Player::draw(SDL_Renderer* renderer) {
        SDL_Texture* current = baseAnimation[0];
        if (state == PlayerState::Idle) current = baseAnimation[0];
        if (state == PlayerState::Walking) current = baseAnimation[1 + walkFrame];
        if (state == PlayerState::Jumping) current = baseAnimation[4];
        if (state == PlayerState::Falling) current = baseAnimation[5];

        SDL_RendererFlip flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_Rect dst = {(int)roundf(x), (int)roundf(y), w, h};
        SDL_RenderCopyEx(renderer, current, nullptr, &dst, 0, nullptr, flip);
    }

    void Player::free() {
        for (int i = 0; i < 6; i++) if (baseAnimation[i]) { SDL_DestroyTexture(baseAnimation[i]); baseAnimation[i] = nullptr; }
    }