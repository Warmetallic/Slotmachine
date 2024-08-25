#include "Renderer.h"
#include <stdexcept>
#include <iostream>

Renderer::Renderer(int screenWidth, int screenHeight)
    : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mWindow(nullptr), mRenderer(nullptr) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(const std::string& windowTitle) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN);
    if (!mWindow) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void Renderer::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
    SDL_RenderClear(mRenderer);
}

void Renderer::present() {
    SDL_RenderPresent(mRenderer);
}

SDL_Texture* Renderer::loadTexture(const std::string& filePath) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (!texture) {
        std::cerr << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void Renderer::renderTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect) {
    SDL_RenderCopy(mRenderer, texture, srcRect, destRect);
}

SDL_Texture* Renderer::renderText(const std::string& text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!texture) {
        std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(mRenderer, r, g, b, a); // Set the color for drawing operations
}

void Renderer::fillRect(const SDL_Rect& rect) {
	SDL_RenderFillRect(mRenderer, &rect); // Fill the rectangle with the current draw color
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(mRenderer, x1, y1, x2, y2);
}

SDL_Renderer* Renderer::getSDLRenderer() const {
    return mRenderer;
}

void Renderer::cleanup() {
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
