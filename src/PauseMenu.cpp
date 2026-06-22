#include "PauseMenu.h"

SDL_Texture* PauseMenu::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

bool PauseMenu::load(SDL_Renderer* renderer) {
    resumeBtn = loadTexture(renderer, "assets/startbutton.jpg");
    menuBtn   = loadTexture(renderer, "assets/startbutton.jpg");
    return resumeBtn && menuBtn;
}

SDL_Rect PauseMenu::getResumeRect(int winW, int winH) {
    int w = (int)(winW * 0.234f);
    int h = (int)(winH * 0.111f);
    return {(winW - w) / 2, (int)(winH * 0.40f), w, h};
}

SDL_Rect PauseMenu::getMenuRect(int winW, int winH) {
    int w = (int)(winW * 0.234f);
    int h = (int)(winH * 0.111f);
    return {(winW - w) / 2, (int)(winH * 0.55f), w, h};
}

PauseResult PauseMenu::handleEvent(SDL_Event& event, int winW, int winH) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;
        SDL_Rect rr = getResumeRect(winW, winH);
        SDL_Rect mr = getMenuRect(winW, winH);
        if (mx >= rr.x && mx <= rr.x + rr.w && my >= rr.y && my <= rr.y + rr.h) return PauseResult::Resume;
        if (mx >= mr.x && mx <= mr.x + mr.w && my >= mr.y && my <= mr.y + mr.h) return PauseResult::MainMenu;
    }
    return PauseResult::None;
}

void PauseMenu::draw(SDL_Renderer* renderer, int winW, int winH) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect overlay = {0, 0, winW, winH};
    SDL_RenderFillRect(renderer, &overlay);

    SDL_Rect rr = getResumeRect(winW, winH);
    SDL_Rect mr = getMenuRect(winW, winH);
    SDL_RenderCopy(renderer, resumeBtn, nullptr, &rr);
    SDL_RenderCopy(renderer, menuBtn,   nullptr, &mr);
}

void PauseMenu::free() {
    if (resumeBtn) { SDL_DestroyTexture(resumeBtn); resumeBtn = nullptr; }
    if (menuBtn)   { SDL_DestroyTexture(menuBtn);   menuBtn   = nullptr; }
}