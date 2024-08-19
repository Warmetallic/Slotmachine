#include "Button.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>

/**
 * Constructor for the Button class.
 * Initializes the button with the given renderer, position, size, and text.
 * @param renderer The SDL_Renderer to use for rendering.
 * @param x The x-coordinate of the button.
 * @param y The y-coordinate of the button.
 * @param w The width of the button.
 * @param h The height of the button.
 * @param text The text to display on the button.
 */
Button::Button(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text)
    : mRenderer(renderer), mButtonRect{ x, y, w, h }, mText(text), mHighlighted(false),
    mAnimationStartTime(SDL_GetTicks()), mClicked(false), mActive(true), mClickSound(NULL)
{
    // Initialize colors
    mBaseColor = { 255, 0, 0, 255 }; // Red
    mHighlightColor = { 255, 100, 100, 255 }; // Lighter red
    mInactiveColor = { 0, 255, 0, 255 }; // Green for inactive
    mCurrentColor = mBaseColor;

    // Load click sound
    mClickSound = Mix_LoadWAV("assets/sounds/click2.mp3"); // Replace with your click sound file path
    if (mClickSound == NULL) {
        printf("Failed to load click sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

/**
 * Destructor for the Button class.
 * Cleans up the loaded sound effect.
 */
Button::~Button() {
    // Free sound effect
    if (mClickSound != NULL) {
        Mix_FreeChunk(mClickSound);
        mClickSound = NULL;
    }
}

/**
 * Renders the button, including its text.
 * Animates the button if it is active.
 */
void Button::render() {
    // Animate the button to change color
    if (mActive) {
        animate();
    }
    else {
        setColor(mInactiveColor); // Set inactive color
    }

    // Set the color and fill the button rectangle
    SDL_SetRenderDrawColor(mRenderer, mCurrentColor.r, mCurrentColor.g, mCurrentColor.b, mCurrentColor.a);
    SDL_RenderFillRect(mRenderer, &mButtonRect);

    // Render button text
    renderText(mText, mButtonRect.x + 10, mButtonRect.y + 10);
}

/**
 * Sets the color of the button.
 * @param color The new color of the button.
 */
void Button::setColor(const SDL_Color& color) {
    mCurrentColor = color;
}

/**
 * Renders the text on the button.
 * @param text The text to render.
 * @param x The x-coordinate where the text should be rendered.
 * @param y The y-coordinate where the text should be rendered.
 */
void Button::renderText(const std::string& text, int x, int y) {
    // Load font
    TTF_Font* font = TTF_OpenFont("assets/fonts/FalloutFont.ttf", 26); // Replace with your font and size
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Render text surface
    SDL_Color textColor = { 0, 0, 0, 255 }; // Black text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    if (textTexture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    // Calculate the center position of the button
    int centerX = mButtonRect.x + (mButtonRect.w / 2);
    int centerY = mButtonRect.y + (mButtonRect.h / 2);

    // Get text dimensions
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    // Adjust the x and y coordinates to center the text
    int renderX = centerX - (textWidth / 2);
    int renderY = centerY - (textHeight / 2);

    // Set rendering space and render to screen
    SDL_Rect renderQuad = { renderX, renderY, textWidth, textHeight };
    SDL_RenderCopy(mRenderer, textTexture, NULL, &renderQuad);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

/**
 * Animates the button by changing its color periodically.
 */
void Button::animate() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - mAnimationStartTime;

    if (elapsedTime > mAnimationDuration) {
        mAnimationStartTime = currentTime;
        if (mHighlighted) {
            setColor(mBaseColor);
        }
        else {
            setColor(mHighlightColor);
        }
        mHighlighted = !mHighlighted;
    }
}

/**
 * Handles SDL events for the button, such as mouse clicks.
 * @param e The SDL_Event to handle.
 */
void Button::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && mActive) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= mButtonRect.x && mouseX <= mButtonRect.x + mButtonRect.w &&
            mouseY >= mButtonRect.y && mouseY <= mButtonRect.y + mButtonRect.h) {
            mClicked = true; // Set clicked state to true
            std::cout << "Button clicked!" << std::endl;

            if (mClickSound != NULL && mActive) { // Play sound if button is active
                playClickSound(); // Play sound if button is active
            }
        }
    }
}

/**
 * Checks if the button has been clicked.
 * @return True if the button has been clicked, false otherwise.
 */
bool Button::isClicked() const {
    return mClicked;
}

/**
 * Resets the clicked state of the button.
 */
void Button::resetClick() {
    mClicked = false;
}

/**
 * Sets the active state of the button.
 * @param active The new active state of the button.
 */
void Button::setActive(bool active) {
    mActive = active;
    if (active) {
        mCurrentColor = mBaseColor; // Reset to base color when activated
    }
}

/**
 * Plays the click sound effect.
 */
void Button::playClickSound() {
    if (mClickSound != NULL) {
        if (Mix_PlayChannel(-1, mClickSound, 0) == -1) {
            printf("Failed to play sound! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }
}
