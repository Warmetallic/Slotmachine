#include "Background.h"
#include <SDL_image.h>
#include <stdio.h>

Background::Background(SDL_Renderer* renderer)
    : mRenderer(renderer), mTexture(NULL) {}

Background::~Background() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
}

bool Background::loadMedia(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    mTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (mTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    return true;
}

void Background::render() {
    SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
}
