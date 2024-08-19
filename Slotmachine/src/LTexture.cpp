#include "LTexture.h"
#include <SDL_ttf.h>
#include <stdio.h>

LTexture::LTexture(SDL_Renderer* renderer)
    : mRenderer(renderer), mTexture(nullptr), mWidth(0), mHeight(0) {}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(const std::string& path) {
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (mTexture == nullptr) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return false;
    }
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return true;
}

bool LTexture::loadFromRenderedText(const std::string& textureText, SDL_Color textColor, TTF_Font* font) {
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface == nullptr) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    if (mTexture == nullptr) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return false;
    }
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    SDL_FreeSurface(textSurface);
    return true;
}

void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y) {
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderQuad);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}
