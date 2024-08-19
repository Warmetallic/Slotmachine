#include "MainGame.h"

int main(int argc, char* args[]) {
    MainGame game;

    if (!game.init()) {
        printf("Failed to initialize!\n");
    }
    else {
        if (!game.loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {
            game.run();
        }
    }

    return 0;
}
