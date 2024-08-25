#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <string>
#include <Renderer.h>

class Background {
public:
    Background(Renderer* renderer);
    ~Background();
    bool loadMedia(const std::string& path);
    void render();

private:
    Renderer* mRenderer;
    SDL_Texture* mTexture;
};

#endif // BACKGROUND_H
