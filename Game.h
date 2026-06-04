#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Flag.h"
#include "Coin.h"
#include "Platform.h"

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

    Player player;
    Enemy enemy;
    Flag flag;

    std::vector<Coin> coinsList;
    std::vector<Platform> platforms;

    bool gameLoop = true;
    bool gameStarted = false;

    float screenWidth = 1500;
    float screenHeight = 800;
};