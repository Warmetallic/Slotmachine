#ifndef MAINGAME_H
#define MAINGAME_H

#include <SDL.h>
#include "Background.h"
#include "Frame.h"
#include "Button.h"
#include "Constants.h"
#include "Reel.h"
#include <vector>
#include "FPSMeter.h" // Include the FPSMeter header
#include <SDL_mixer.h> // Include for SDL_mixer


class Background;
class Button;
class Frame;
class Reel;

class MainGame {
public:
    MainGame();
    ~MainGame();

    bool init();
    bool loadMedia();
    void handleEvents(bool& quit);
    void render();
    void run();
    void close();
	bool areReelsSpinning; // Add flag to track if reels are spinning

private:
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Background* background;
    Frame* frame;
    Button* button;
    std::vector<Reel*> mReels;
    FPSMeter* fpsMeter; // Add FPSMeter pointer

    // Time management
    Uint32 lastTime;
    Uint32 currentTime;
    Uint32 deltaTime;
    

    // New method to check if all reels have stopped spinning
    bool allReelsStopped() const;
    Mix_Music* backgroundMusic; // Add this line
};

#endif // MAINGAME_H