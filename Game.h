#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <vector>
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"

#include "Platform.h"
#include "Flag.h"

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
    SDL_Texture* backgroundTexture = nullptr;
    SDL_Texture* groundTexture = nullptr;
    SDL_Texture* platformTexture = nullptr;
    SDL_Texture* knightTexture = nullptr;
    SDL_Texture* enemyTexture = nullptr;
    SDL_Texture* rollingTexture = nullptr;
    SDL_Texture* coinTexture = nullptr;
    SDL_Texture* flagTexture = nullptr;


    SDL_Event event;

    Player player;
    Enemy enemy;
    Flag flag;

    std::vector<Coin> coinsList;
    std::vector<Platform> platforms;

    bool gameLoop = true;
    bool gameStarted = false;

	//Camera variables
    float cameraX = 0.0f;
    
    //Gravity variables
    float gravity = 1800.0f; //gravity strength and how much it pulls down
    float jumpStrength = -700.0f; //how high the player will jump
    float speed = 500.0f;

    float screenWidth = 1500;
    float screenHeight = 800;
};