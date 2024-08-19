#include "Frame.h"
#include <SDL_image.h>
#include <iostream>
#include <Constants.h>

Frame::Frame(SDL_Renderer* renderer, int borderWidth)
    : mRenderer(renderer), mBorderWidth(borderWidth), mBottomHeight(198), mBottomTexture(nullptr), mHeaderTexture(nullptr) {
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = 100; // Default width
    mRect.h = 100; // Default height
}

Frame::~Frame() {
    // Clean up the texture
    if (mBottomTexture != nullptr) {
        SDL_DestroyTexture(mBottomTexture);
        mBottomTexture = nullptr;
    }

    // Clean up the header texture
    if (mHeaderTexture != nullptr) {
        SDL_DestroyTexture(mHeaderTexture);
        mHeaderTexture = nullptr;
    }
}

void Frame::setDimensions(int width, int height) {
    mRect.w = width;
    mRect.h = height;
    mRect.x = (SCREEN_WIDTH - mRect.w) / 2;
    mRect.y = (SCREEN_HEIGHT - mRect.h - 100) / 2; // Adjust y position if needed
}

bool Frame::loadBottomTexture(const std::string& path) {
    // Clean up any existing texture
    if (mBottomTexture != nullptr) {
        SDL_DestroyTexture(mBottomTexture);
        mBottomTexture = nullptr;
    }

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface pixels
    mBottomTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (mBottomTexture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return mBottomTexture != nullptr;
}


bool Frame::loadHeaderTexture(const std::string& path) {
    // Clean up any existing texture
    if (mHeaderTexture != nullptr) {
        SDL_DestroyTexture(mHeaderTexture);
        mHeaderTexture = nullptr;
    }

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface pixels
    mHeaderTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (mHeaderTexture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return mHeaderTexture != nullptr;
}

void Frame::render() {
    drawBorder();
    drawLines();
    drawBottom();
    drawHeader();
}

void Frame::drawBorder() {

    // Set the color for the metallic gray border
    SDL_SetRenderDrawColor(mRenderer, 75, 75, 68, 255); // Metallic gray color

    // Adjust the width of the border rectangle so that it extends more on the left and right sides
    int borderThickness = 20; // Thickness of the border
    int sideExtension = 5;   // Additional extension on the left and right sides

    // Calculate the border rectangle dimensions
    SDL_Rect borderRect = {
        mRect.x - sideExtension,            // Move left edge to the left
        mRect.y - sideExtension * 4,                            // Top edge stays the same
        mRect.w + 2 * sideExtension,        // Width increased by twice the side extension
        mRect.h + borderThickness           // Height includes bottom border
    };

    // Draw the border rectangle
    SDL_RenderFillRect(mRenderer, &borderRect);

    // Draw the black rectangle (background of the frame)
    SDL_SetRenderDrawColor(mRenderer, 10, 10, 10, 255);
    SDL_RenderFillRect(mRenderer, &mRect);
}



void Frame::drawHeader() {
    // Increase width and height slightly to make it larger than the frame
    int extraWidth = 10;
    int extraHeight = 1;

    // Calculate new position to keep the header rectangle centered
    int newX = mRect.x - (extraWidth / 2);
    int newY = mRect.y - mBottomHeight - extraHeight; // Adjust y to be slightly above the frame

    SDL_Rect headerRect = { newX, newY+150, mRect.w + extraWidth, 50 };

    if (mHeaderTexture != nullptr) {
        // Draw the texture in the header section
        SDL_RenderCopy(mRenderer, mHeaderTexture, nullptr, &headerRect);
    }
    else {
        // If no texture, draw a black rectangle as the background
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(mRenderer, &headerRect);

        // Draw the golden border around the header section
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xD7, 0x00, 0xFF); // Golden color
        SDL_RenderDrawRect(mRenderer, &headerRect);
    }
}



void Frame::drawBottom() {
    // Increase width and height slightly to make it larger than the frame
    int extraWidth = 12;
    int extraHeight = 1;

    // Calculate new position to keep the bottom rectangle centered
    int newX = mRect.x - (extraWidth / 2);
    int newY = mRect.y + mRect.h - (extraHeight / 2); // Adjust y to be slightly below the frame

    SDL_Rect bottomRect = { newX, newY, mRect.w + extraWidth, mBottomHeight + extraHeight };

    if (mBottomTexture != nullptr) {
        // Draw the texture in the bottom section
        SDL_RenderCopy(mRenderer, mBottomTexture, nullptr, &bottomRect);
    }
    else {
        // If no texture, draw a black rectangle as the background
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(mRenderer, &bottomRect);

        // Draw the golden border around the bottom section
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xD7, 0x00, 0xFF); // Golden color
        SDL_RenderDrawRect(mRenderer, &bottomRect);
    }
}

void Frame::drawLines() {
    int borderOffset = 1; // Adjust as needed to avoid overlap
    int numParts = 5;
    int partWidth = mRect.w / numParts;

    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xD7, 0x00, 0xFF); // Golden color
    for (int i = 1; i < numParts; ++i) {
        int x = mRect.x + i * partWidth;
        SDL_RenderDrawLine(mRenderer, x, mRect.y + borderOffset, x, mRect.y + mRect.h - borderOffset);
    }
}

// Getter methods
int Frame::getWidth() const {
    return mRect.w;
}

int Frame::getHeight() const {
    return mRect.h;
}

int Frame::getX() const {
    return mRect.x;
}

int Frame::getY() const {
    return mRect.y;
}

int Frame::getBottomHeight() const {
    return mBottomHeight;
}
