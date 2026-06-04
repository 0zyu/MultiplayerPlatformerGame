#include "Game.h"
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_image/SDL_image.h>
#include "Collision.h"

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

    previousTime = SDL_GetTicks();
    lastFrameTime = SDL_GetTicks();
    lastEnemyFrameTime = SDL_GetTicks();
    lastCoinFrameTime = SDL_GetTicks();
    lastFrameTimeRolling = SDL_GetTicks();
    lastFlagFrameTime = SDL_GetTicks();

    enemy.lastFrameTime = SDL_GetTicks();
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
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        handleEvents();
        update(deltaTime);

        



        render();
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
    //platform moving code
    float previousXMovingPlatformPosition = xMovingPlatformPosition;

    xMovingPlatformPosition += xMovingPlatformSpeed * xMovingPlatformDirection * deltaTime;

    float xMovingPlatformDifference = xMovingPlatformPosition - previousXMovingPlatformPosition;

    if (xMovingPlatformPosition < platformLimitLeft)
    {
        xMovingPlatformDirection = 1;
    }
    else if (xMovingPlatformPosition > platformLimitRight)
    {
        xMovingPlatformDirection = -1;
    }


    float previousYMovingPlatformPosition = yMovingPlatformPosition;

    yMovingPlatformPosition += yMovingPlatformSpeed * yMovingPlatformDirection * deltaTime;

    float yMovingPlatformDifference = yMovingPlatformPosition - previousYMovingPlatformPosition;

    if (yMovingPlatformPosition < platformLimitTop)
    {
        yMovingPlatformDirection = 1;
    }
    else if (yMovingPlatformPosition > platformLimitBottom)
    {
        yMovingPlatformDirection = -1;
    }


    platforms =
    {
        {450.f, 650.f},
        {xMovingPlatformPosition, 575.f},
        {1500.f, 450.f},
        {1800.f, 650.f},
        {2000.f, 650.f},
        {2200.f, 650.f},
        {2600.f, yMovingPlatformPosition},
        {3200.f, 550.f},
        {3800.f, 650.f},
        {4300.f, 650.f}
    };




    //Player input code
    const bool* keys = SDL_GetKeyboardState(NULL);

    player.rolling = false;
    moving = false;
    player.xVelocity = 0;

    if (flag.levelCompleted == false && gameStarted == true)
    {
        if (keys[SDL_SCANCODE_D])
        {
            player.xVelocity = speed;
            player.xPosition += player.xVelocity * deltaTime;
            player.facingRight = true;
            moving = true;
        }

        if (keys[SDL_SCANCODE_A] && player.xPosition > 0.f)
        {
            player.xVelocity = -speed;
            player.xPosition += player.xVelocity * deltaTime;
            player.facingRight = false;
            moving = true;
        }

        if (keys[SDL_SCANCODE_W] && player.xPosition > 0.f && moving)
        {
            player.rolling = true;
            moving = false;
            wWasPressed = keys[SDL_SCANCODE_W];

            if (player.facingRight)
            {
                player.xVelocity = 0;
                player.xVelocity += speed * deltaTime;
            }
            else
            {
                player.xVelocity = 0;
                player.xVelocity += -speed * deltaTime;
            }

            player.xPosition += player.xVelocity * deltaTime;
        }

        if (keys[SDL_SCANCODE_SPACE] && player.onGround)
        {
            player.yVelocity = jumpStrength;
            player.onGround = false;
            bottomReached = false;
        }
    }

    //Collision and gravity code
    if (bottomReached == false)
    {
        player.yVelocity += gravity * deltaTime;
        player.yPosition += player.yVelocity * deltaTime;
    }
    else
    {
        bottomReached = false;
    }

    bottomReached = false;

    for (int i = 0; i < platforms.size(); i++)
    {
        if (collision(player.xPosition, player.yPosition,
            player.width, player.height,
            platforms[i].xPosition, platforms[i].yPosition,
            player.yVelocity, bottomReached,
            screenWidth, screenHeight,
            platforms[i].width, platforms[i].height,
            player.xVelocity,
            yMovingPlatformDifference,
            xMovingPlatformDifference,
            i,
            xMovingPlatformDirection))
        {
            break;
        }
    }

    // Ground collision
    if (player.yPosition + player.height >= groundY)
    {
        player.yPosition = groundY - player.height;
        player.yVelocity = 0.0f;
        bottomReached = true;
    }

    player.onGround = bottomReached;

    // Camera follows player
    cameraX = player.xPosition - screenWidth / 2 + player.width / 2;
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    float backgroundWidth = screenWidth;
    float backgroundHeight = screenHeight;
    float backgroundScrollX = cameraX * 0.1f;

    while (backgroundScrollX >= backgroundWidth)
    {
        backgroundScrollX -= backgroundWidth;
    }

    while (backgroundScrollX < 0)
    {
        backgroundScrollX += backgroundWidth;
    }

    SDL_FRect background1 =
    {
        -backgroundScrollX,
        0,
        backgroundWidth,
        backgroundHeight
    };

    SDL_FRect background2 =
    {
        backgroundWidth - backgroundScrollX,
        0,
        backgroundWidth,
        backgroundHeight
    };

    SDL_RenderTexture(renderer, backgroundTexture, NULL, &background1);
    SDL_RenderTexture(renderer, backgroundTexture, NULL, &background2);

    SDL_RenderPresent(renderer);
}