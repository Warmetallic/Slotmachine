#ifndef REEL_H
#define REEL_H

#include <SDL.h>
#include <vector>
#include <string>

class Reel {
public:
    Reel(SDL_Renderer* renderer, int x, int y, int w, int h, const std::vector<std::string>& iconPaths);
    ~Reel();

    void loadIcons(const std::vector<std::string>& iconPaths);
    void setClipRect(const SDL_Rect& clipRect);
    void render(Uint32 deltaTime);
    void renderIcon(int index, int yOffset, int iconHeight);
    void update(Uint32 deltaTime);
    void setPosition(int position);
    void startSpin(int startOffset, Uint32 stopDelay);
    void stopSpin();
    bool isSpinning() const;
    void setRandomSpinSpeed();
    void setStopTime(Uint32 time);
    bool shouldStop(Uint32 currentTime);
    void stopSpinAfterDelay(Uint32 delay);


private:
    void setRandomPosition();

    SDL_Renderer* mRenderer;
    SDL_Rect mReelRect;
    SDL_Rect mClipRect;
    std::vector<SDL_Texture*> mIcons;
    int mCurrentIconIndex;
    bool mSpinning;
    Uint32 mSpinStartTime;
    Uint32 mSpinDuration;
    int mStartPosition;
    int mMaxPosition;
    int mOriginalIconHeight;
    int mStartPositionOffset; // Offset for staggered start
    int mStopDelay; // Delay before stopping
    Uint32 mStopTime; // Time when the reel should stop
    float mSpinSpeed; 

    // Prevent copying
    Reel(const Reel&) = delete;
    Reel& operator=(const Reel&) = delete;
};

#endif // REEL_H
