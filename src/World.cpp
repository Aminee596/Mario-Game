#include "World.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Chunk.h"
#include "Mobs.h"

bool World::loadChunkSurfaces() {
    chunkSurfaces.clear();
    int i = 0;
    while (true) {
        std::string path = "assets/chunk" + std::to_string(i) + ".png";
        SDL_Surface* raw = IMG_Load(path.c_str());
        if (!raw) break;
        SDL_Surface* s = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(raw);
        if (!s) break;
        chunkSurfaces.push_back(s);
        i++;
    }
    return !chunkSurfaces.empty();
}

void World::freeSurfaces() {
    for (SDL_Surface* s : chunkSurfaces) SDL_FreeSurface(s);
    chunkSurfaces.clear();
}

void World::update(float playerX, int score, int winH) {
    currentTier = score / 1000;
    cameraX = playerX - chunkWidth * 0.33f;
    if (cameraX <= chunks[0].startX) cameraX = chunks[0].startX;
    float currentChunkStart = chunks[1].startX;
    if (playerX >= currentChunkStart + chunkWidth) {
        cycleForward(winH);
    }
}

void World::rebuildPlatforms() {
    activePlatforms.clear();
    for (auto& ch : chunks) {
        for (auto& p : ch.platforms) activePlatforms.push_back(p);
        for (auto& s : ch.stairs) activePlatforms.push_back(s);
        for (auto& s : ch.sewers) activePlatforms.push_back(s);
        for (auto& m : ch.mysteryBoxes) activePlatforms.push_back(m.rect);
        if (ch.hasLeftBarrier) {
            SDL_FRect wall = {ch.startX, 0, 20, (float)1080};
            activePlatforms.push_back(wall);
        }
    }
}

bool World::loadTexture(SDL_Renderer* renderer) {
    usedmysteryBoxes = IMG_LoadTexture(renderer, "assets/mysterybox_used.png");
    mysteryBoxes = IMG_LoadTexture(renderer, "assets/mysterybox.png");
    barrier = IMG_LoadTexture(renderer, "assets/barrier.png");
    stairs = IMG_LoadTexture(renderer, "assets/stairs.png");
    sewertop = IMG_LoadTexture(renderer, "assets/sewertop.png");
    sewerbottom = IMG_LoadTexture(renderer, "assets/sewerbottom.png");
    ground = IMG_LoadTexture(renderer, "assets/ground.png");
    water = IMG_LoadTexture(renderer, "assets/water.png");
    coins = IMG_LoadTexture(renderer, "assets/coin.png");
    return barrier && stairs && mysteryBoxes && usedmysteryBoxes && sewertop && sewerbottom && ground && water && coins;
}

bool World::HitMysteryBox(float playerX, float playerY, float playerW, float playerH, float playerVY) {
    for (auto& ch : chunks) {
        for (auto& m : ch.mysteryBoxes) {
            if (m.used) continue;
            bool horizontal = playerX < m.rect.x + m.rect.w && playerX + playerW > m.rect.x;
            float playerTop = playerY;
            float boxBottom = m.rect.y + m.rect.h;
            bool topNearBottom = playerTop <= boxBottom + 5 && playerTop >= boxBottom - 10;
            if (playerVY < 0 && horizontal && topNearBottom) {
                m.used = true;
                return true;
            }
        }
    }
    return false;
}

int World::hitMobs(float px, float py, float pw, float ph, float pvy) {
    SDL_FRect pr = {px, py, pw, ph};
    for (auto& mob : mobs) {
        if (!mob.alive) continue;
        SDL_FRect mr = {mob.x, mob.y, mob.w, mob.h};
        if (!SDL_HasIntersectionF(&pr, &mr)) continue;

        bool stomped = (pvy > 0 && (py + ph) < mob.y + mob.h * 0.5f);

        switch (mob.type) {
            case MobType::Poop:
                if (stomped) { mob.alive = false; return 1; }
                else return -1;
            case MobType::Turtle:
                if (stomped) { mob.alive = false; return 2; }
                else return -1;
            case MobType::Boss:
                if (stomped) {
                    mob.health--;
                    if (mob.health <= 0) { mob.alive = false; return 3; } 
                    return 4; 
                }
                else return -1;
        }
    }
    return 0;
}

bool World::collectCoins(float playerX, float playerY, float playerW, float playerH) {
    SDL_FRect playerRect = {playerX, playerY, playerW, playerH};
    for (auto& ch : chunks) {
        for (auto& c : ch.coins) {
            if (playerRect.x < c.x + c.w && playerRect.x + playerRect.w > c.x && playerRect.y < c.y + c.h && playerRect.y + playerRect.h > c.y) {
                c = ch.coins.back();
                ch.coins.pop_back();
                return true;
            }
        }
    }
    return false;
}

bool World::pointInWater(float ptx, float pty) {
    for (auto& ch : chunks)
        for (auto& wtr : ch.water)
            if (ptx >= wtr.x && ptx <= wtr.x + wtr.w && pty >= wtr.y && pty <= wtr.y + wtr.h)
                return true;
    return false;
}

bool World::fullyInWater(float px, float py, float pw, float ph) {
    return pointInWater(px, py) && pointInWater(px + pw, py) &&
           pointInWater(px, py + ph) && pointInWater(px + pw, py + ph);
}

void World::spawnMobs(Chunk& chunk) {
    if (chunk.platforms.empty()) return;
    int level = currentTier + 1; 
    int sumToLevel = level * (level + 1) / 2;
    int mobCount = rand() % (sumToLevel + 1) + level;
    if (mobCount > 20) mobCount = 20;

    float speed = 60.0f + currentTier * 20.0f;
    for (int i = 0; i < mobCount; i++) {
        SDL_FRect tile = chunk.platforms[rand() % chunk.platforms.size()];
        Mob mob;
        int roll = rand() % 100;
        mob.type = (roll < 20 + currentTier * 10) ? MobType::Turtle : MobType::Poop;
        mob.w = 40; mob.h = 40;
        mob.x = tile.x;
        mob.y = tile.y - mob.h;
        mob.vx = (rand() % 2 == 0) ? speed : -speed;
        mob.facingLeft = mob.vx < 0;
        mob.vy = 0;
        mob.alive = true;
        mobs.push_back(mob);
    }
}

void World::cycleForward(int winH) {
    chunks.erase(chunks.begin());
    float lastEnd = chunks.back().startX + chunkWidth;
    Chunk c;
    c.generate(lastEnd, chunkWidth, chunkSurfaces[1 + rand() % (chunkSurfaces.size() - 1)]);
    spawnMobs(c);
    chunks.push_back(c);
    for (auto& ch : chunks) ch.hasLeftBarrier = false;
    chunks[0].hasLeftBarrier = true;
    rebuildPlatforms();
}

void World::init() {
    chunkWidth = 1920;
    chunks.clear();
    cameraX = 0;
    currentTier = 0;
    for (int i = 0; i < 4; i++) {
        Chunk c;
        SDL_Surface* chosen = chunkSurfaces[i == 0 ? 0 : 1 + rand() % (chunkSurfaces.size() - 1)];
        c.generate(i * chunkWidth, chunkWidth, chosen);
        chunks.push_back(c);
    }
    chunks[0].hasLeftBarrier = true;
    rebuildPlatforms();
}

void World::draw(SDL_Renderer* renderer, int winH) {
    const float MOUTH_H = 64.0f;
    for (auto& ch : chunks) {
        for (auto& p : ch.platforms) {
            SDL_Rect r = {(int)(p.x - cameraX), (int)p.y, (int)p.w, (int)p.h};
            SDL_RenderCopy(renderer, ground, nullptr, &r);
        }
        for (auto& w : ch.water) {
            SDL_Rect r = {(int)(w.x - cameraX), (int)w.y, (int)w.w, (int)w.h};
            SDL_RenderCopy(renderer, water, nullptr, &r);
        }
        for (auto& c : ch.coins) {
            SDL_Rect r = {(int)(c.x - cameraX), (int)c.y, (int)c.w, (int)c.h};
            SDL_RenderCopy(renderer, coins, nullptr, &r);
        }
        for (auto& s : ch.stairs) {
            SDL_Rect r = {(int)(s.x - cameraX), (int)s.y, (int)s.w, (int)s.h};
            SDL_RenderCopy(renderer, stairs, nullptr, &r);
        }
        for (auto& m : ch.mysteryBoxes) {
            SDL_Rect r = {(int)(m.rect.x - cameraX), (int)m.rect.y, (int)m.rect.w, (int)m.rect.h};
            SDL_RenderCopy(renderer, m.used ? usedmysteryBoxes : mysteryBoxes, nullptr, &r);
        }
        for (auto& s : ch.sewers) {
            SDL_Rect topR = {(int)(s.x - cameraX), (int)s.y, (int)s.w, (int)MOUTH_H};
            SDL_RenderCopy(renderer, sewertop, nullptr, &topR);
            float bodyH = s.h - MOUTH_H;
            if (bodyH > 0) {
                SDL_Rect bodyR = {(int)(s.x - cameraX), (int)(s.y + MOUTH_H), (int)s.w, (int)bodyH};
                SDL_RenderCopy(renderer, sewerbottom, nullptr, &bodyR);
            }
        }
        if (ch.hasLeftBarrier && barrier) {
            SDL_Rect dst = {(int)(ch.startX - cameraX), 0, 20, winH};
            SDL_RenderCopy(renderer, barrier, nullptr, &dst);
        }
    }
    for (auto& mob : mobs) {
        if (!mob.alive) continue;
        SDL_Texture* tex = Mob::Poomob;
        if (mob.type == MobType::Turtle) tex = Mob::Turtlemob; else if (mob.type == MobType::Boss) tex = Mob::Boss;
        SDL_Rect r = {(int)(mob.x - cameraX), (int)mob.y, (int)mob.w, (int)mob.h};
        SDL_RendererFlip flip = mob.facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer, tex, nullptr, &r, 0, nullptr, flip);
    }
}

std::vector<SDL_FRect>& World::getPlatforms() {
    return activePlatforms;
}