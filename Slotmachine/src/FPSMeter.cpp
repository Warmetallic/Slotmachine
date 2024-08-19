#include "FPSMeter.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <sstream>

FPSMeter::FPSMeter(SDL_Renderer* renderer, TTF_Font* font)
    : gRenderer(renderer), gFont(font), frameCount(0), startTime(0), fpsTexture(renderer) {
}

FPSMeter::~FPSMeter() {
    if (gFont != nullptr) {
        TTF_CloseFont(gFont);
        gFont = nullptr;
    }
}

void FPSMeter::start() {
    startTime = SDL_GetTicks();
    frameCount = 0;
}

void FPSMeter::update() {
    frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - startTime >= 1000) {
        int fps = frameCount / ((currentTime - startTime) / 1000);
        std::stringstream ss;
        ss << "FPS: " << fps;

        // Render text to texture
        SDL_Color textColor = { 255, 255, 255, 255 }; // White color
        if (!fpsTexture.loadFromRenderedText(ss.str(), textColor, gFont)) {
            printf("Unable to render FPS texture!\n");
        }

        startTime = currentTime;
        frameCount = 0;
    }
}

void FPSMeter::render(int x, int y) {
    fpsTexture.render(x, y);
}
