#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>  // Include this for TTF_Font
#include <string>

class LTexture {
public:
    LTexture(SDL_Renderer* renderer);
    ~LTexture();
    bool loadFromFile(const std::string& path);
    bool loadFromRenderedText(const std::string& textureText, SDL_Color textColor, TTF_Font* font);
    void free();
    void render(int x, int y);
    int getWidth();
    int getHeight();

private:
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

#endif // LTEXTURE_H
#pragma once
