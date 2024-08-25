#include "FPSMeter.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <sstream>

/**
 * Constructor for the FPSMeter class.
 * Initializes the FPS meter with the given renderer and font.
 * @param renderer The custom Renderer to use for rendering.
 * @param font The TTF_Font to use for rendering text.
 */
FPSMeter::FPSMeter(Renderer* renderer, TTF_Font* font)
    : mRenderer(renderer), gFont(font), frameCount(0), startTime(0), fpsTexture(renderer->getSDLRenderer()) {
}

/**
 * Destructor for the FPSMeter class.
 * Cleans up the font resource.
 */
FPSMeter::~FPSMeter() {
    // The font should be closed by the class that owns it
    // We don't close it here to avoid premature deallocation
}

/**
 * Starts the FPS meter by initializing the start time and frame count.
 */
void FPSMeter::start() {
    startTime = SDL_GetTicks();
    frameCount = 0;
}

/**
 * Updates the FPS meter by incrementing the frame count and calculating the FPS.
 * If one second has passed, it updates the FPS texture with the new FPS value.
 */
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

/**
 * Renders the FPS texture at the specified position.
 * @param x The x-coordinate where the FPS texture should be rendered.
 * @param y The y-coordinate where the FPS texture should be rendered.
 */
void FPSMeter::render(int x, int y) {
    fpsTexture.render(x, y); // Ensure that fpsTexture is compatible with mRenderer
}
