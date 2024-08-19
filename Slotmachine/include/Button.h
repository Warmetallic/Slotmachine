#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <string>
#include <SDL_mixer.h>

class Button {
public:
    Button(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text);
    ~Button();

    void render();
    void handleEvent(const SDL_Event& e);
    bool isClicked() const;
    void resetClick();
    void setActive(bool active); // New method to set the button active/inactive

private:
    SDL_Renderer* mRenderer;
    SDL_Rect mButtonRect;
    std::string mText;
    bool mHighlighted;
    bool mClicked;
    bool mActive; // New variable to track active state
    Uint32 mAnimationStartTime;
    const Uint32 mAnimationDuration = 500; // Duration in milliseconds

    SDL_Color mBaseColor;
    SDL_Color mHighlightColor;
    SDL_Color mInactiveColor; // Color for inactive state
    SDL_Color mCurrentColor;

    void renderText(const std::string& text, int x, int y);
    void animate();
    void setColor(const SDL_Color& color);
    void playClickSound(); // Add this method to play sound

    Mix_Chunk* mClickSound; // Add this for sound effect
};

#endif
