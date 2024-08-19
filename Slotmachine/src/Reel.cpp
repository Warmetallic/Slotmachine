#include "Reel.h"
#include <SDL_image.h>
#include <stdio.h>
#include <cmath> // For std::fmod
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()

Reel::Reel(SDL_Renderer* renderer, int x, int y, int w, int h, const std::vector<std::string>& iconPaths)
    : mRenderer(renderer), mReelRect{ x, y, w, h }, mCurrentIconIndex(0), mSpinning(false), mSpinDuration(2000),
    mStartPosition(0), mSpinSpeed(1.0f), mMaxPosition(1000), mStartPositionOffset(0), mStopDelay(0) {
    loadIcons(iconPaths);
    if (!mIcons.empty()) {
        SDL_QueryTexture(mIcons[0], NULL, NULL, NULL, &mOriginalIconHeight);
    }
    mClipRect = { x, y, w, h }; // Initialize the clip rectangle
}

Reel::~Reel() {
    for (auto texture : mIcons) {
        SDL_DestroyTexture(texture);
    }
}

void Reel::loadIcons(const std::vector<std::string>& iconPaths) {
    for (const auto& path : iconPaths) {
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        }
        else {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);
            if (texture == NULL) {
                printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            }
            else {
                mIcons.push_back(texture);
            }
        }
    }
}

void Reel::setClipRect(const SDL_Rect& clipRect) {
    mClipRect = clipRect;
}

void Reel::render(Uint32 deltaTime) {
    if (mSpinning) {
        update(deltaTime); // Update the position of the reel
    }

    SDL_RenderSetClipRect(mRenderer, &mClipRect); // Set the clipping rectangle

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

    SDL_RenderSetClipRect(mRenderer, NULL); // Reset the clipping rectangle
}

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
    SDL_RenderCopy(mRenderer, mIcons[index], NULL, &renderQuad);
}

//void Reel::update(Uint32 deltaTime) {
//    if (mSpinning) {
//        Uint32 currentTime = SDL_GetTicks();
//        Uint32 elapsed = currentTime - mSpinStartTime;
//
//        if (elapsed < mSpinDuration) {
//            mStartPosition = (mStartPosition + static_cast<int>(deltaTime * mSpinSpeed)) % mReelRect.h;
//        }
//        else if (currentTime >= mStopTime) {
//            mSpinning = false;
//            setRandomPosition(); // Set a random position after stopping
//            printf("Reel stopped at position %d\n", mStartPosition); // Debug statement
//        }
//    }
//}
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


void Reel::setRandomPosition() {
    if (mIcons.empty()) return;
    int iconCount = static_cast<int>(mIcons.size());
    int iconHeight = mReelRect.h / iconCount;
    int randomIndex = std::rand() % iconCount;
    mStartPosition = randomIndex * iconHeight;
}

void Reel::setPosition(int position) {
    int totalHeight = mReelRect.h;

    // Normalize the position to ensure it wraps around properly
    mStartPosition = position % totalHeight;
    if (mStartPosition < 0) {
        mStartPosition += totalHeight;
    }

    printf("Setting reel position to: %d\n", mStartPosition);
}

void Reel::startSpin(int startOffset, Uint32 stopDelay) {
    setRandomSpinSpeed();
    mSpinning = true;
    mSpinStartTime = SDL_GetTicks();
    mStartPositionOffset = startOffset;
    mStopDelay = stopDelay;
    mStopTime = mSpinStartTime + mSpinDuration + stopDelay; // Calculate stop time
}

void Reel::setStopTime(Uint32 time) {
    mStopTime = time;
}

bool Reel::shouldStop(Uint32 currentTime) {
    return currentTime >= mStopTime;
}


void Reel::stopSpin() {
    mSpinning = false;
    setRandomPosition();
}

bool Reel::isSpinning() const {
    return mSpinning;
}

void Reel::setRandomSpinSpeed() {
    // Set a random spin speed between 0.5 and 2.0
    mSpinSpeed = 0.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (0.5f - 0.1f)));
}
