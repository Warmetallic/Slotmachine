#include "Reel.h"
#include <SDL_image.h>
#include <stdio.h>
#include <cmath> // For std::fmod
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()

/**
 * Constructor for the Reel class.
 * Initializes the reel with the given parameters and loads the icons.
 * @param renderer The custom Renderer to use for rendering.
 * @param x The x-coordinate of the reel.
 * @param y The y-coordinate of the reel.
 * @param w The width of the reel.
 * @param h The height of the reel.
 * @param iconPaths A vector of file paths to the icons.
 */
Reel::Reel(Renderer* renderer, int x, int y, int w, int h, const std::vector<std::string>& iconPaths)
    : mRenderer(renderer), mReelRect{ x, y, w, h }, mCurrentIconIndex(0), mSpinning(false), mSpinDuration(2000),
    mStartPosition(0), mSpinSpeed(1.0f), mMaxPosition(1000), mStartPositionOffset(0), mStopDelay(0) {
    loadIcons(iconPaths);
    if (!mIcons.empty()) {
        SDL_QueryTexture(mIcons[0], NULL, NULL, NULL, &mOriginalIconHeight);
    }
    mClipRect = { x, y, w, h }; // Initialize the clip rectangle
}

/**
 * Destructor for the Reel class.
 * Cleans up the loaded textures.
 */
Reel::~Reel() {
    for (auto texture : mIcons) {
        SDL_DestroyTexture(texture);
    }
}

/**
 * Loads the icons from the given file paths.
 * @param iconPaths A vector of file paths to the icons.
 */
void Reel::loadIcons(const std::vector<std::string>& iconPaths) {
    for (const auto& path : iconPaths) {
        SDL_Texture* texture = mRenderer->loadTexture(path);
        if (texture == nullptr) {
            printf("Unable to load image %s!\n", path.c_str());
        }
        else {
            mIcons.push_back(texture);
        }
    }
}

/**
 * Sets the clipping rectangle for the reel.
 * @param clipRect The SDL_Rect defining the clipping rectangle.
 */
void Reel::setClipRect(const SDL_Rect& clipRect) {
    mClipRect = clipRect;
}

/**
 * Renders the reel, updating its position if it is spinning.
 * @param deltaTime The time elapsed since the last frame.
 */
void Reel::render(Uint32 deltaTime) {
    if (mSpinning) {
        update(deltaTime); // Update the position of the reel
    }

    mRenderer->setDrawColor(0, 0, 0, 255); // Set a draw color for the clipping rectangle if needed
    mRenderer->fillRect(mClipRect); // Fill the rectangle with the draw color
    SDL_RenderSetClipRect(mRenderer->getSDLRenderer(), &mClipRect); // Set the clipping rectangle

    size_t iconCount = mIcons.size();
    if (iconCount == 0) return; // Avoid division by zero

    int iconHeight = mReelRect.h / static_cast<int>(iconCount);

    // Render icons in a loop to ensure seamless scrolling
    for (int i = -1; i <= 1; ++i) { // Extend rendering to cover the reel height
        int yOffset = (i * mReelRect.h) - mStartPosition;

        // Render each icon at the calculated yOffset
        for (size_t j = 0; j < iconCount; ++j) {
            int currentYOffset = yOffset + static_cast<int>(j) * iconHeight;
            renderIcon(static_cast<int>(j), currentYOffset, iconHeight);
        }
    }

    SDL_RenderSetClipRect(mRenderer->getSDLRenderer(), NULL); // Reset the clipping rectangle
}

/**
 * Renders a single icon at the specified position.
 * @param index The index of the icon to render.
 * @param yOffset The y-offset for rendering the icon.
 * @param iconHeight The height of the icon.
 */
void Reel::renderIcon(int index, int yOffset, int iconHeight) {
    int borderOffset = 22; // Border width
    int drawableWidth = mReelRect.w - 2 * borderOffset; // Drawable width within the border
    int drawableHeight = iconHeight; // Height of the drawable area for one icon

    // Calculate icon width and height based on the drawable area
    int iconWidth = drawableWidth; // Use full width available within the drawable area
    int renderIconHeight = iconHeight; // Use full height of the icon

    // Scale the icon to fit within the drawable area while maintaining aspect ratio
    SDL_Rect iconSize;
    SDL_QueryTexture(mIcons[index], NULL, NULL, &iconSize.w, &iconSize.h); // Get original icon dimensions

    float widthRatio = static_cast<float>(drawableWidth) / iconSize.w;
    float heightRatio = static_cast<float>(drawableHeight) / iconSize.h;
    float scaleRatio = std::min(widthRatio, heightRatio); // Scale to fit within the drawable area

    iconWidth = static_cast<int>(iconSize.w * scaleRatio);
    renderIconHeight = static_cast<int>(iconSize.h * scaleRatio);

    // Prepare the rectangle for rendering
    SDL_Rect renderQuad;
    renderQuad.w = iconWidth;
    renderQuad.h = renderIconHeight;

    // Center the icon horizontally within the drawable area
    renderQuad.x = mReelRect.x + borderOffset + (drawableWidth - iconWidth) / 2;
    // Position vertically based on yOffset
    renderQuad.y = mReelRect.y + yOffset + borderOffset;

    // Ensure rendering happens within frame bounds
    mRenderer->renderTexture(mIcons[index], NULL, &renderQuad);
}

/**
 * Updates the position of the reel if it is spinning.
 * @param deltaTime The time elapsed since the last frame.
 */
void Reel::update(Uint32 deltaTime) {
    if (mSpinning) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsed = currentTime - mSpinStartTime;

        // Update position based on deltaTime and spin speed
        mStartPosition = (mStartPosition - static_cast<int>(deltaTime * mSpinSpeed)) % mReelRect.h;

        // Check if the reel should stop
        if (elapsed >= mSpinDuration) {
            if (currentTime >= mStopTime) {
                mSpinning = false;
                setRandomPosition(); // Optionally set a random position after stopping
                printf("Reel stopped at position %d\n", mStartPosition);
            }
        }
    }
}

/**
 * Sets a random position for the reel.
 */
void Reel::setRandomPosition() {
    if (mIcons.empty()) return;
    int iconCount = static_cast<int>(mIcons.size());
    int iconHeight = mReelRect.h / iconCount;
    int randomIndex = std::rand() % iconCount;
    mStartPosition = randomIndex * iconHeight;
}

/**
 * Sets the position of the reel.
 * @param position The new position of the reel.
 */
void Reel::setPosition(int position) {
    int totalHeight = mReelRect.h;

    // Normalize the position to ensure it wraps around properly
    mStartPosition = position % totalHeight;
    if (mStartPosition < 0) {
        mStartPosition += totalHeight;
    }

    printf("Setting reel position to: %d\n", mStartPosition);
}

/**
 * Starts spinning the reel with a random speed and specified stop delay.
 * @param startOffset The initial offset for the start position.
 * @param stopDelay The delay before the reel stops.
 */
void Reel::startSpin(int startOffset, Uint32 stopDelay) {
    setRandomSpinSpeed();
    mSpinning = true;
    mSpinStartTime = SDL_GetTicks();
    mStartPositionOffset = startOffset;
    mStopDelay = stopDelay;
    mStopTime = mSpinStartTime + mSpinDuration + stopDelay; // Calculate stop time
}

/**
 * Sets the stop time for the reel.
 * @param time The time at which the reel should stop.
 */
void Reel::setStopTime(Uint32 time) {
    mStopTime = time;
}

/**
 * Checks if the reel should stop spinning.
 * @param currentTime The current time.
 * @return True if the reel should stop, false otherwise.
 */
bool Reel::shouldStop(Uint32 currentTime) {
    return currentTime >= mStopTime;
}

/**
 * Stops the reel from spinning and sets a random position.
 */
void Reel::stopSpin() {
    mSpinning = false;
    setRandomPosition();
}

/**
 * Checks if the reel is currently spinning.
 * @return True if the reel is spinning, false otherwise.
 */
bool Reel::isSpinning() const {
    return mSpinning;
}

/**
 * Sets a random spin speed for the reel.
 */
void Reel::setRandomSpinSpeed() {
    // Set a random spin speed between 0.5 and 2.0
    mSpinSpeed = 0.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (0.5f - 0.1f)));
}
