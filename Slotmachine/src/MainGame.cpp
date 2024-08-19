#include "MainGame.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h> // Ensure SDL_mixer is included
#include <stdio.h>
#include <ctime> // For std::time()

MainGame::MainGame()
    : gWindow(NULL), gRenderer(NULL), background(NULL), frame(NULL), button(NULL), fpsMeter(NULL),
    backgroundMusic(NULL), lastTime(0), currentTime(0), deltaTime(0), areReelsSpinning(false) {
    std::srand(static_cast<unsigned>(std::time(0))); // Initialize random seed
}

MainGame::~MainGame() {
    close();
}

bool MainGame::init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) { // Initialize both video and audio
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("Slot Machine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                else if (Mix_Init(MIX_INIT_MP3) == 0) { // Initialize SDL_mixer
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
                else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf("SDL_mixer could not initialize audio! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool MainGame::loadMedia() {
    background = new Background(gRenderer);
    if (!background->loadMedia("assets/textures/background.jpeg")) {
        printf("Failed to load media!\n");
        return false;
    }

    frame = new Frame(gRenderer);
    frame->setDimensions(500, 300); // Adjust dimensions if needed

    // Load texture for the bottom section
    if (!frame->loadBottomTexture("assets/textures/bottom.jpg")) {
        printf("Failed to load bottom texture!\n");
    }

    // Load texture for the header section
    if (!frame->loadHeaderTexture("assets/textures/top.jpg")) {
        printf("Failed to load bottom texture!\n");
    }

    // Load font for FPSMeter
    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 28);
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    else {
        fpsMeter = new FPSMeter(gRenderer, font);
        fpsMeter->start();
    }

    // Create and load button
    button = new Button(gRenderer, SCREEN_WIDTH / 2 + 115, SCREEN_HEIGHT - 128, 100, 50, "START");

    // Create reels and add them to the MainGame
    std::vector<std::string> iconPaths = { "assets/icons/watermelon.png", "assets/icons/apple.png", "assets/icons/cherries.png" };

    int frameWidth = frame->getWidth();
    int frameHeight = frame->getHeight();
    int reelWidth = frameWidth / 5;
    int reelHeight = frameHeight;

    for (int i = 0; i < 5; ++i) {
        Reel* reel = new Reel(gRenderer, frame->getX() + i * reelWidth, frame->getY(), reelWidth, reelHeight, iconPaths);
        mReels.push_back(reel);
    }

    // Load and play background music
    backgroundMusic = Mix_LoadMUS("assets/sounds/jazz.mp3"); // Replace with your music file path
    if (backgroundMusic == NULL) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    if (Mix_PlayMusic(backgroundMusic, -1) == -1) { // Play music indefinitely (-1 for looping)
        printf("Failed to play background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

void MainGame::handleEvents(bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        button->handleEvent(e);

        if (button->isClicked() && !areReelsSpinning) {
            Uint32 startTime = SDL_GetTicks();
            Uint32 stopDelay = 0;
            for (auto reel : mReels) {
                reel->startSpin(0, stopDelay);
                stopDelay += 500;
            }
            areReelsSpinning = true;
            button->setActive(false);
            button->resetClick();
        }
    }
}

void MainGame::render() {
    SDL_RenderClear(gRenderer);

    background->render();
    frame->render();
    for (auto reel : mReels) {
        reel->render(deltaTime);
    }
    button->render();
    if (fpsMeter) {
        fpsMeter->update();
        fpsMeter->render(10, SCREEN_HEIGHT - 30);
    }

    SDL_RenderPresent(gRenderer);
}

void MainGame::run() {
    bool quit = false;

    while (!quit) {
        Uint32 newTime = SDL_GetTicks();
        deltaTime = newTime - lastTime;
        lastTime = newTime;

        handleEvents(quit);

        for (auto& reel : mReels) {
            reel->update(deltaTime);
        }

        if (areReelsSpinning) {
            bool allStopped = true;
            for (auto& reel : mReels) {
                if (reel->isSpinning()) {
                    allStopped = false;
                    break;
                }
            }
            if (allStopped) {
                areReelsSpinning = false;
                button->setActive(true);
            }
        }

        render();
    }
}

void MainGame::close() {
    for (auto reel : mReels) {
        delete reel;
    }
    mReels.clear(); // Clear the vector after deleting

    if (backgroundMusic != NULL) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }

    delete button;
    delete frame;
    delete background;
    delete fpsMeter;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    Mix_Quit(); // Quit SDL_mixer
    TTF_Quit();
    SDL_Quit();
}
