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
#include "NetworkManager.h"
#include "Platform.h"
#include "Flag.h"

using namespace std;

class Game
{
public:
    bool init();
    void run();
    void clean();

private:
    NetworkManager network;

    void handleEvents();
    void update(float deltaTime);
    void render();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

	//Still textures
    TTF_Font* font = nullptr;
    SDL_Texture* backgroundTexture = nullptr;
    SDL_Texture* groundTexture = nullptr;
    SDL_Texture* platformTexture = nullptr;
    SDL_Texture* loadTexture(const std::string& filePath);

	
    

    static const int numberOfFramesForKnight = 8;
    static const int numberOfFramesForEnemy = 12;
    static const int numberOfCoinFrames = 8;
    static const int numberOfFlagFrames = 6;

    SDL_Texture* runFramesForwards[numberOfFramesForKnight];
    SDL_Texture* runFramesBackwards[numberOfFramesForKnight];
    SDL_Texture* rollingRightAnimation[numberOfFramesForKnight];
    SDL_Texture* rollingLeftAnimation[numberOfFramesForKnight];

    SDL_Texture* enemyRunFrameForwards[numberOfFramesForEnemy];
    SDL_Texture* coinFrames[numberOfCoinFrames];
    SDL_Texture* flagFrames[numberOfFlagFrames];

    SDL_Texture* titleTextTexture = nullptr;
    SDL_Texture* titleShadowTexture = nullptr;
    SDL_Texture* startTextTexture = nullptr;

    SDL_Texture* levelCompleteTextTexture = nullptr;
    SDL_Texture* levelCompleteShadowTexture = nullptr;

    SDL_Texture* coinTextTexture = nullptr;

    SDL_Surface* coinTextSurface = nullptr;

    SDL_Color primaryColour = { 255, 255, 255, 255 };
    SDL_Color secondaryColour = { 0, 0, 0, 255 };

    string coinText = "Coins: 0";

    SDL_Event event;

    Player player;
    Player otherPlayer;
    Enemy enemy;
    Flag flag;

    bool playerMoving = false;
    bool otherPlayerMoving = false;


    vector<Coin> coinsList;
    vector<Platform> platforms;

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

    // General game state
    bool bottomReached = false;
    
    
    bool wWasPressed = false;
    bool spaceWasPressed = false;

    // Timing
    Uint64 previousTime = 0;
    Uint64 lastFrameTime = 0;
    Uint64 lastEnemyFrameTime = 0;
    Uint64 lastCoinFrameTime = 0;
    Uint64 lastFrameTimeRolling = 0;
    Uint64 lastFlagFrameTime = 0;
    Uint64 enemyTimer = 0;
    Uint64 lastInputSendTime = 0;
    int inputSendDelay = 33;

    // Animation
    int frameDelay = 100;
    int coinFrameDelay = 100;
    int currentCoinFrame = 0;
    float rollingFrameDelay = 100.f;
    Uint64 lastOtherPlayerFrameTime = 0;

    // Texture/render helpers
    float width = 0.f;
    float height = 0.f;
    float scaleHeight = 1.2f;
    float scaleWidth = 1.2f;

    // Ground
    float groundY = 750.f;

    // Coins
    int coinWidth = 45;
    int coinHeight = 54;
    int coinCount = 0;

    // Moving platforms
    float xMovingPlatformPosition = 1300.f;
    float xMovingPlatformSpeed = 200.f;
    float xMovingPlatformDirection = 1.f;

    float yMovingPlatformPosition = 560.f;
    float yMovingPlatformSpeed = 200.f;
    float yMovingPlatformDirection = 1.f;

    // Platform limits
    float platformLimitLeft = 700.f;
    float platformLimitRight = 1300.f;
    float platformLimitTop = 250.f;
    float platformLimitBottom = 650.f;

};