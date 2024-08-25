#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <string>
#include <Renderer.h>
#include <memory>


class Background {
public:
    Background(std::shared_ptr<Renderer> renderer);
    ~Background();
    bool loadMedia(const std::string& path);
    void render();

private:
    std::shared_ptr<Renderer> mRenderer;
    SDL_Texture* mTexture;
};

#endif // BACKGROUND_H
