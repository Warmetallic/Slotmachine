#include "MainGame.h"

/**
 * The main entry point of the application.
 * Initializes the game, loads media, and runs the game loop.
 * @param argc The number of command-line arguments.
 * @param args The array of command-line arguments.
 * @return The exit status of the application.
 */
int main(int argc, char* args[]) {
    MainGame game;

    // Initialize the game
    if (!game.init()) {
        printf("Failed to initialize!\n");
    }
    else {
        // Load game media
        if (!game.loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {
            // Run the game loop
            game.run();
        }
    }

    return 0;
}
