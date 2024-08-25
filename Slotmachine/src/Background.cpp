#include "Background.h"
#include <SDL_image.h>
#include <stdio.h>

/**
 * Constructor for the Background class.
 * Initializes the background with the given renderer.
 * @param renderer The SDL_Renderer to use for rendering.
 */
Background::Background(Renderer* renderer)
    : mRenderer(renderer), mTexture(NULL) {}

/**
 * Destructor for the Background class.
 * Cleans up the loaded texture.
 */
Background::~Background() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
}

/**
 * Loads the background texture from the given file path.
 * @param path The file path to the texture.
 * @return True if the texture was loaded successfully, false otherwise.
 */
bool Background::loadMedia(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    mTexture = SDL_CreateTextureFromSurface(mRenderer->getSDLRenderer(), loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (mTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    return true;
}

/**
 * Renders the background texture to the screen.
 */
void Background::render() {
    mRenderer->renderTexture(mTexture, NULL, NULL);
}
