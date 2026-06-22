#include "Menu.h"

SDL_Texture* Menu::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

bool Menu::load(SDL_Renderer* renderer) {
    bg = loadTexture(renderer, "assets/bg1.jpg");
    startBtn = loadTexture(renderer, "assets/startbutton.png");
    quitBtn = loadTexture(renderer, "assets/quitbutton.png");
    return bg && startBtn && quitBtn;
}

SDL_Rect Menu::getStartRect(int winW, int winH) {
    int w = (int)(winW * 0.234f);
    int h = (int)(winH * 0.111f);
    return {(winW - w) / 2, (int)(winH * 0.55f), w, h};
}

SDL_Rect Menu::getQuitRect(int winW, int winH) {
    int w = (int)(winW * 0.234f);
    int h = (int)(winH * 0.111f);
    return {(winW - w) / 2, (int)(winH * 0.70f), w, h};
}

MenuResult Menu::handleEvent(SDL_Event& event, int winW, int winH) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;
        SDL_Rect sr = getStartRect(winW, winH);
        SDL_Rect qr = getQuitRect(winW, winH);
        if (mx >= sr.x && mx <= sr.x + sr.w && my >= sr.y && my <= sr.y + sr.h) return MenuResult::Start;
        if (mx >= qr.x && mx <= qr.x + qr.w && my >= qr.y && my <= qr.y + qr.h) return MenuResult::Quit;
    }
    return MenuResult::None;
}

void Menu::draw(SDL_Renderer* renderer, int winW, int winH) {
    SDL_Rect full = {0, 0, winW, winH};
    SDL_RenderCopy(renderer, bg, nullptr, &full);
    SDL_Rect sr = getStartRect(winW, winH);
    SDL_Rect qr = getQuitRect(winW, winH);
    SDL_RenderCopy(renderer, startBtn, nullptr, &sr);
    SDL_RenderCopy(renderer, quitBtn,  nullptr, &qr);
}

void Menu::free() {
    if (bg) { SDL_DestroyTexture(bg); bg = nullptr; }
    if (startBtn) { SDL_DestroyTexture(startBtn); startBtn = nullptr; }
    if (quitBtn) { SDL_DestroyTexture(quitBtn); quitBtn  = nullptr; }
}