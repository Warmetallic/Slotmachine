#ifndef MAINGAME_H
#define MAINGAME_H

#include <SDL.h>
#include "Background.h"
#include "Frame.h"
#include "Button.h"
#include "Constants.h"
#include "Reel.h"
#include <vector>
#include "FPSMeter.h"
#include <SDL_mixer.h> 
#include "Renderer.h" // Include the Renderer header file
#include <memory>


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
	bool areReelsSpinning; 

private:
    SDL_Window* gWindow;
	std::shared_ptr<Renderer> gRenderer;
	std::unique_ptr<Background> background;
	std::unique_ptr<Frame> frame;
    std::unique_ptr<Button> button;
    std::vector<std::unique_ptr<Reel>> mReels;
	std::unique_ptr<FPSMeter> fpsMeter;
    //Renderer* gRenderer;
    //Background* background;
    //Frame* frame;
    //Button* button;
    //std::vector<Reel*> mReels;
    //FPSMeter* fpsMeter; 

    // Time management
    Uint32 lastTime;
    Uint32 currentTime;
    Uint32 deltaTime;
    

 
    bool allReelsStopped() const;
    Mix_Music* backgroundMusic;
};

#endif // MAINGAME_H