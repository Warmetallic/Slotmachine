#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

class Renderer {
public:
    // Constructor and Destructor
    Renderer(int screenWidth, int screenHeight);
    ~Renderer();

    // Initializes SDL, creates window and renderer
    bool init(const std::string& windowTitle);

    // Clears the screen with a specified color
    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    // Presents the rendered content on the screen
    void present();

    // Loads a texture from a file
    SDL_Texture* loadTexture(const std::string& filePath);

    // Renders a texture to the screen
    void renderTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);

    // Renders text to the screen
    SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Color color);

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a); // Declare the setDrawColor method
    void fillRect(const SDL_Rect& rect);  // Declare the fillRect method

	void drawLine(int x1, int y1, int x2, int y2); // Declare the drawLine method

    // Accessor for SDL_Renderer
    SDL_Renderer* getSDLRenderer() const;

    // Cleans up SDL resources
    void cleanup();

private:
    int mScreenWidth;
    int mScreenHeight;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
};

#endif // RENDERER_H
