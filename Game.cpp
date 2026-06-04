#include "Game.h"
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_image/SDL_image.h>

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }

    if (TTF_Init() < 0)
    {
        return false;
    }

    font = TTF_OpenFont("assets/PixelOperator8-Bold.ttf", 95);

    if (!font)
    {
        return false;
    }

    window = SDL_CreateWindow("Platformer", screenWidth, screenHeight, 0);

    if (!window)
    {
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer)
    {
        return false;
    }

    coinsList =
    {
        { 525.f, 550.f },
        { 1575.f, 350.f },
        { 2075.f, 475.f },
        { 3275.f, 475.f },
        { 4375.f, 575.f }
    };

    SDL_Surface* surfaceKnight = IMG_Load("assets/knightSprite1.png");
    SDL_Surface* surfacePlatform = IMG_Load("assets/platform.png");
    SDL_Surface* surfaceBackground = IMG_Load("assets/Clouds.png");
    SDL_Surface* surfaceGround = IMG_Load("assets/ground.png");
    SDL_Surface* surfaceEnemy = IMG_Load("assets/enemy1.png");
    SDL_Surface* surfaceRolling = IMG_Load("assets/rollRight1.png");
    SDL_Surface* surfaceCoin = IMG_Load("assets/coin1.png");
    SDL_Surface* surfaceFlag = IMG_Load("assets/betterflag1.png");

    if (!surfaceKnight || !surfacePlatform || !surfaceBackground || !surfaceGround ||
        !surfaceEnemy || !surfaceRolling || !surfaceCoin || !surfaceFlag)
    {
        return false;
    }

    flagTexture = SDL_CreateTextureFromSurface(renderer, surfaceFlag);
    coinTexture = SDL_CreateTextureFromSurface(renderer, surfaceCoin);
    rollingTexture = SDL_CreateTextureFromSurface(renderer, surfaceRolling);
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    enemyTexture = SDL_CreateTextureFromSurface(renderer, surfaceEnemy);
    knightTexture = SDL_CreateTextureFromSurface(renderer, surfaceKnight);
    platformTexture = SDL_CreateTextureFromSurface(renderer, surfacePlatform);
    groundTexture = SDL_CreateTextureFromSurface(renderer, surfaceGround);

    SDL_SetTextureScaleMode(flagTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(coinTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(rollingTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(enemyTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(knightTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(platformTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(groundTexture, SDL_SCALEMODE_NEAREST);


    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/knightSprite" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        runFramesForwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(runFramesForwards[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/knightSpriteReverse" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        runFramesBackwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(runFramesBackwards[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/rollRight" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        rollingRightAnimation[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(rollingRightAnimation[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/rollLeft" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        rollingLeftAnimation[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(rollingLeftAnimation[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    for (int i = 0; i < numberOfFramesForEnemy; i++)
    {
        std::string filePath = "assets/enemy" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        enemyRunFrameForwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(enemyRunFrameForwards[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    for (int i = 0; i < numberOfCoinFrames; i++)
    {
        std::string filePath = "assets/coin" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        coinFrames[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(coinFrames[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    for (int i = 0; i < numberOfFlagFrames; i++)
    {
        std::string filePath = "assets/betterflag" + std::to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        if (!tempSurface)
        {
            return false;
        }

        flagFrames[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_SetTextureScaleMode(flagFrames[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }




    SDL_DestroySurface(surfaceFlag);
    SDL_DestroySurface(surfaceCoin);
    SDL_DestroySurface(surfaceRolling);
    SDL_DestroySurface(surfaceBackground);
    SDL_DestroySurface(surfaceEnemy);
    SDL_DestroySurface(surfaceKnight);
    SDL_DestroySurface(surfacePlatform);
    SDL_DestroySurface(surfaceGround);




    return true;
}


void Game::run()
{
    while (gameLoop)
    {
        handleEvents();
    }
}

void Game::clean()
{

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        SDL_DestroyTexture(runFramesForwards[i]);
        SDL_DestroyTexture(runFramesBackwards[i]);
        SDL_DestroyTexture(rollingRightAnimation[i]);
        SDL_DestroyTexture(rollingLeftAnimation[i]);
    }

    for (int i = 0; i < numberOfFramesForEnemy; i++)
    {
        SDL_DestroyTexture(enemyRunFrameForwards[i]);
    }

    for (int i = 0; i < numberOfCoinFrames; i++)
    {
        SDL_DestroyTexture(coinFrames[i]);
    }

    for (int i = 0; i < numberOfFlagFrames; i++)
    {
        SDL_DestroyTexture(flagFrames[i]);
    }

    SDL_DestroyTexture(flagTexture);
    SDL_DestroyTexture(coinTexture);
    SDL_DestroyTexture(rollingTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(knightTexture);
    SDL_DestroyTexture(platformTexture);
    SDL_DestroyTexture(groundTexture);
    

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

void Game::handleEvents()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            gameLoop = false;
        }
    }
}

void Game::update(float deltaTime)
{

}

void Game::render()
{

}