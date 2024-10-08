#ifndef FPSMETER_H
#define FPSMETER_H

#include "Renderer.h"
#include <SDL_ttf.h>
#include <string>
#include "LTexture.h"
#include <memory>


class FPSMeter {
public:
    FPSMeter(std::shared_ptr<Renderer> renderer, TTF_Font* font);
    ~FPSMeter();
    void start();
    void update();
    void render(int x, int y);

private:
    std::shared_ptr<Renderer> mRenderer;  // Pointer to the custom Renderer class
    TTF_Font* gFont;      // Pointer to the TTF_Font
    int frameCount;
    Uint32 startTime;
    LTexture fpsTexture;
};

#endif // FPSMETER_H
