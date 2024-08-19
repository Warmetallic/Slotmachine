#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <string>

class Background {
public:
    Background(SDL_Renderer* renderer);
    ~Background();
    bool loadMedia(const std::string& path);
    void render();

private:
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;
};

#endif // BACKGROUND_H
