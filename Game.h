#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Game
{
public:
    bool init();
    void run();
    void clean();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    bool gameLoop = true;

    float screenWidth = 1500;
    float screenHeight = 800;
};