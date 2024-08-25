#include "Frame.h"
#include <SDL_image.h>
#include <iostream>
#include <Constants.h>

/**
 * Constructor for the Frame class.
 * Initializes the frame with the given renderer and border width.
 * @param renderer The custom Renderer to use for rendering.
 * @param borderWidth The width of the border.
 */
Frame::Frame(Renderer* renderer, int borderWidth)
    : mRenderer(renderer), mBorderWidth(borderWidth), mBottomHeight(198), mBottomTexture(nullptr), mHeaderTexture(nullptr) {
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = 100; // Default width
    mRect.h = 100; // Default height
}

/**
 * Destructor for the Frame class.
 * Cleans up the loaded textures.
 */
Frame::~Frame() {
    // Clean up the bottom texture
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

/**
 * Sets the dimensions of the frame.
 * @param width The new width of the frame.
 * @param height The new height of the frame.
 */
void Frame::setDimensions(int width, int height) {
    mRect.w = width;
    mRect.h = height;
    mRect.x = (SCREEN_WIDTH - mRect.w) / 2;
    mRect.y = (SCREEN_HEIGHT - mRect.h - 100) / 2; // Adjust y position if needed
}

/**
 * Loads the bottom texture from the given file path.
 * @param path The file path to the texture.
 * @return True if the texture was loaded successfully, false otherwise.
 */
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
    mBottomTexture = SDL_CreateTextureFromSurface(mRenderer->getSDLRenderer(), loadedSurface);
    if (mBottomTexture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return mBottomTexture != nullptr;
}

/**
 * Loads the header texture from the given file path.
 * @param path The file path to the texture.
 * @return True if the texture was loaded successfully, false otherwise.
 */
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
    mHeaderTexture = SDL_CreateTextureFromSurface(mRenderer->getSDLRenderer(), loadedSurface);
    if (mHeaderTexture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return mHeaderTexture != nullptr;
}

/**
 * Renders the frame, including its border, lines, bottom, and header.
 */
void Frame::render() {
    drawBorder();
    drawLines();
    drawBottom();
    drawHeader();
}

/**
 * Draws the border of the frame.
 */
void Frame::drawBorder() {
    // Set the color for the metallic gray border
    mRenderer->setDrawColor(75, 75, 68, 255); // Metallic gray color

    // Adjust the width of the border rectangle so that it extends more on the left and right sides
    int borderThickness = 20; // Thickness of the border
    int sideExtension = 5;   // Additional extension on the left and right sides

    // Calculate the border rectangle dimensions
    SDL_Rect borderRect = {
        mRect.x - sideExtension,            // Move left edge to the left
        mRect.y - sideExtension * 4,        // Top edge stays the same
        mRect.w + 2 * sideExtension,        // Width increased by twice the side extension
        mRect.h + borderThickness           // Height includes bottom border
    };

    // Draw the border rectangle
    mRenderer->fillRect(borderRect);

    // Draw the black rectangle (background of the frame)
    mRenderer->setDrawColor(10, 10, 10, 255);
    mRenderer->fillRect(mRect);
}

/**
 * Draws the header of the frame.
 */
void Frame::drawHeader() {
    // Increase width and height slightly to make it larger than the frame
    int extraWidth = 10;
    int extraHeight = 1;

    // Calculate new position to keep the header rectangle centered
    int newX = mRect.x - (extraWidth / 2);
    int newY = mRect.y - mBottomHeight - extraHeight; // Adjust y to be slightly above the frame

    SDL_Rect headerRect = { newX, newY + 150, mRect.w + extraWidth, 50 };

    if (mHeaderTexture != nullptr) {
        // Draw the texture in the header section
        mRenderer->renderTexture(mHeaderTexture, nullptr, &headerRect);
    }
    else {
        // If no texture, draw a black rectangle as the background
        mRenderer->setDrawColor(0, 0, 0, 255);
        mRenderer->fillRect(headerRect);

        // Draw the golden border around the header section
        mRenderer->setDrawColor(0xFF, 0xD7, 0x00, 0xFF); // Golden color
        mRenderer->fillRect(headerRect);
    }
}

/**
 * Draws the bottom of the frame.
 */
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
        mRenderer->renderTexture(mBottomTexture, nullptr, &bottomRect);
    }
    else {
        // If no texture, draw a black rectangle as the background
        mRenderer->setDrawColor(0, 0, 0, 255);
        mRenderer->fillRect(bottomRect);

        // Draw the golden border around the bottom section
        mRenderer->setDrawColor(0xFF, 0xD7, 0x00, 0xFF); // Golden color
        mRenderer->fillRect(bottomRect);
    }
}

/**
 * Draws the vertical lines within the frame.
 */
void Frame::drawLines() {
    int borderOffset = 1; // Adjust as needed to avoid overlap
    int numParts = 5;
    int partWidth = mRect.w / numParts;

    mRenderer->setDrawColor(0xFF, 0xD7, 0x00, 0xFF); // Golden color
    for (int i = 1; i < numParts; ++i) {
        int x = mRect.x + i * partWidth;
        mRenderer->drawLine(x, mRect.y + borderOffset, x, mRect.y + mRect.h - borderOffset);
    }
}

// Getter methods

/**
 * Gets the width of the frame.
 * @return The width of the frame.
 */
int Frame::getWidth() const {
    return mRect.w;
}

/**
 * Gets the height of the frame.
 * @return The height of the frame.
 */
int Frame::getHeight() const {
    return mRect.h;
}

/**
 * Gets the x-coordinate of the frame.
 * @return The x-coordinate of the frame.
 */
int Frame::getX() const {
    return mRect.x;
}

/**
 * Gets the y-coordinate of the frame.
 * @return The y-coordinate of the frame.
 */
int Frame::getY() const {
    return mRect.y;
}

/**
 * Gets the height of the bottom section of the frame.
 * @return The height of the bottom section of the frame.
 */
int Frame::getBottomHeight() const {
    return mBottomHeight;
}
