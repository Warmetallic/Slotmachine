#ifndef FPSMETER_H
#define FPSMETER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "LTexture.h"

class FPSMeter {
public:
    FPSMeter(SDL_Renderer* renderer, TTF_Font* font);
    ~FPSMeter();
    void start();
    void update();
    void render(int x, int y);

private:
    SDL_Renderer* gRenderer;
    TTF_Font* gFont;
    int frameCount;
    Uint32 startTime;
    LTexture fpsTexture;
};

#endif // FPSMETER_H
