#include "MainGame.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <ctime>

/**
 * MainGame class constructor.
 * Initializes member variables and seeds the random number generator.
 */
MainGame::MainGame()
    : gWindow(nullptr), gRenderer(nullptr), background(nullptr), frame(nullptr), button(nullptr), fpsMeter(nullptr),
    backgroundMusic(nullptr), lastTime(0), currentTime(0), deltaTime(0), areReelsSpinning(false) {
    std::srand(static_cast<unsigned>(std::time(0))); // Initialize random seed
}

/**
 * MainGame class destructor.
 * Calls the close method to clean up resources.
 */
MainGame::~MainGame() {
    close();
}

/**
 * Initializes SDL, creates the window and renderer, and initializes SDL_ttf and SDL_mixer.
 * @return True if initialization is successful, false otherwise.
 */
bool MainGame::init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("Slot Machine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            gRenderer = new Renderer(SCREEN_WIDTH, SCREEN_HEIGHT);  // Create Renderer instance
            if (!gRenderer->init("Slot Machine")) {
                printf("Renderer could not be initialized!\n");
                success = false;
            }
            else {
                // Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }

                // Initialize SDL_mixer
                if (Mix_Init(MIX_INIT_MP3) == 0) {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
                if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) != 0) {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

/**
 * Loads media assets such as textures, fonts, and sounds.
 * @return True if all media assets are loaded successfully, false otherwise.
 */
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
    if (font == nullptr) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    else {
        fpsMeter = new FPSMeter(gRenderer, font);
        fpsMeter->start();
    }

    // Create and load button using the custom Renderer class
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
    if (backgroundMusic == nullptr) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    if (Mix_PlayMusic(backgroundMusic, -1) == -1) { // Play music indefinitely (-1 for looping)
        printf("Failed to play background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

/**
 * Handles user input events such as quitting the game or pressing keys.
 * @param quit Reference to a boolean that indicates whether the game should quit.
 */
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

/**
 * Renders the game objects to the screen.
 */
void MainGame::render() {
    gRenderer->clearScreen(255, 255, 255, 255);  // Clear screen using the Renderer class

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

    gRenderer->present();  // Present the screen using the Renderer class
}

/**
 * Main game loop that handles events, updates game state, and renders the game.
 */
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

/**
 * Cleans up resources and quits SDL subsystems.
 */
void MainGame::close() {
    for (auto reel : mReels) {
        delete reel;
    }
    mReels.clear(); // Clear the vector after deleting

    if (backgroundMusic != nullptr) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }

    delete button;
    delete frame;
    delete background;
    delete fpsMeter;

    if (gRenderer != nullptr) {
        delete gRenderer;
        gRenderer = nullptr;
    }

    SDL_DestroyWindow(gWindow);
    Mix_Quit(); // Quit SDL_mixer
    TTF_Quit();
    SDL_Quit();
}
