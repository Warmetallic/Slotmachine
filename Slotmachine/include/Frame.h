#ifndef FRAME_H
#define FRAME_H

#include <string>
#include "Renderer.h"  // Include the custom Renderer class

class Frame {
public:
    // Constructor and Destructor
    Frame(Renderer* renderer, int borderWidth = 1);  // Use custom Renderer class
    ~Frame();

    // Set the dimensions of the frame
    void setDimensions(int width, int height);

    // Load texture for the bottom section
    bool loadBottomTexture(const std::string& path);

    bool loadHeaderTexture(const std::string& path); // Method to load header texture

    // Render the frame and its bottom section
    void render();

    // Getter methods
    int getWidth() const;
    int getHeight() const;
    int getX() const;
    int getY() const;
    int getBottomHeight() const;

private:
    // Draw the border of the frame
    void drawBorder();

    // Draw the internal lines of the frame
    void drawLines();

    // Draw the bottom section of the frame with texture
    void drawBottom();
    void drawHeader();

    Renderer* mRenderer;      // Pointer to the custom Renderer class
    SDL_Rect mRect;           // Rectangle defining the main frame
    int mBorderWidth;         // Border width of the frame
    int mBottomHeight;        // Height of the bottom section
    SDL_Texture* mBottomTexture;  // Texture for the bottom section
    SDL_Texture* mHeaderTexture;  // Texture for the header section
};

#endif // FRAME_H
