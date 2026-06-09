#include "Game.h"
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_image/SDL_image.h>
#include "Collision.h"

SDL_Texture* Game::loadTexture(const std::string& filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());

    if (!surface)
    {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture)
    {
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    }

    SDL_DestroySurface(surface);

    return texture;
}

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

    // Initialize ENet, if it didnt then it returns false and the game will not run
    if (!network.init())
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

    //Text
    SDL_Surface* titleTextSurface = TTF_RenderText_Blended(font, "Knight Jump!", 0, primaryColour);
    SDL_Surface* titleShadowSurface = TTF_RenderText_Blended(font, "Knight Jump!", 0, secondaryColour);
    SDL_Surface* startTextSurface = TTF_RenderText_Blended(font, "Press SPACE to Start", 0, secondaryColour);

    SDL_Surface* levelCompleteTextSurface = TTF_RenderText_Blended(font, "Level Complete!", 0, primaryColour);
    SDL_Surface* levelCompleteShadowSurface = TTF_RenderText_Blended(font, "Level Complete!", 0, secondaryColour);

    coinTextSurface = TTF_RenderText_Blended(font, coinText.c_str(), 0, primaryColour);

    titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);
    titleShadowTexture = SDL_CreateTextureFromSurface(renderer, titleShadowSurface);
    startTextTexture = SDL_CreateTextureFromSurface(renderer, startTextSurface);

    levelCompleteTextTexture = SDL_CreateTextureFromSurface(renderer, levelCompleteTextSurface);
    levelCompleteShadowTexture = SDL_CreateTextureFromSurface(renderer, levelCompleteShadowSurface);

    coinTextTexture = SDL_CreateTextureFromSurface(renderer, coinTextSurface);

    
    SDL_DestroySurface(titleTextSurface);
    SDL_DestroySurface(titleShadowSurface);
    SDL_DestroySurface(startTextSurface);
    SDL_DestroySurface(levelCompleteTextSurface);
    SDL_DestroySurface(levelCompleteShadowSurface);

    //Object Textures
    SDL_Surface* surfacePlatform = IMG_Load("assets/platform.png");
    SDL_Surface* surfaceBackground = IMG_Load("assets/Clouds.png");
    SDL_Surface* surfaceGround = IMG_Load("assets/ground.png");
    

	//Timing initialization
    previousTime = SDL_GetTicks();
    lastFrameTime = SDL_GetTicks();
    lastEnemyFrameTime = SDL_GetTicks();
    lastCoinFrameTime = SDL_GetTicks();
    lastFrameTimeRolling = SDL_GetTicks();
    lastFlagFrameTime = SDL_GetTicks();

    enemy.lastFrameTime = SDL_GetTicks();
    if (!surfacePlatform || !surfaceBackground || !surfaceGround)
    {
        return false;
    }

    backgroundTexture = loadTexture("assets/Clouds.png");
    platformTexture = loadTexture("assets/platform.png");
    groundTexture = loadTexture("assets/ground.png");

    if (!backgroundTexture || !platformTexture || !groundTexture)
    {
        return false;
    }
   

    SDL_SetTextureScaleMode(platformTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(groundTexture, SDL_SCALEMODE_NEAREST);


    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/knightSprite" + std::to_string(i + 1) + ".png";
        runFramesForwards[i] = loadTexture(filePath);

        if (!runFramesForwards[i])
        {
            return false;
        }
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/knightSpriteReverse" + std::to_string(i + 1) + ".png";
        runFramesBackwards[i] = loadTexture(filePath);

        if (!runFramesBackwards[i])
        {
            return false;
        }
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/rollRight" + std::to_string(i + 1) + ".png";
        rollingRightAnimation[i] = loadTexture(filePath);

        if (!rollingRightAnimation[i])
        {
            return false;
        }
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        std::string filePath = "assets/rollLeft" + std::to_string(i + 1) + ".png";
        rollingLeftAnimation[i] = loadTexture(filePath);

        if (!rollingLeftAnimation[i])
        {
            return false;
        }
    }

    for (int i = 0; i < numberOfFramesForEnemy; i++)
    {
        std::string filePath = "assets/enemy" + std::to_string(i + 1) + ".png";
        enemyRunFrameForwards[i] = loadTexture(filePath);

        if (!enemyRunFrameForwards[i])
        {
            return false;
        }
    }

    for (int i = 0; i < numberOfCoinFrames; i++)
    {
        std::string filePath = "assets/coin" + std::to_string(i + 1) + ".png";
        coinFrames[i] = loadTexture(filePath);

        if (!coinFrames[i])
        {
            return false;
        }
    }

    for (int i = 0; i < numberOfFlagFrames; i++)
    {
        std::string filePath = "assets/betterflag" + std::to_string(i + 1) + ".png";
        flagFrames[i] = loadTexture(filePath);

        if (!flagFrames[i])
        {
            return false;
        }
    }

    SDL_DestroySurface(surfaceBackground);
   
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

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(platformTexture);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(titleTextTexture);
    SDL_DestroyTexture(titleShadowTexture);
    SDL_DestroyTexture(startTextTexture);

    SDL_DestroyTexture(levelCompleteTextTexture);
    SDL_DestroyTexture(levelCompleteShadowTexture);

    SDL_DestroyTexture(coinTextTexture);
    SDL_DestroySurface(coinTextSurface);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    network.clean();

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
    const bool* keys = SDL_GetKeyboardState(NULL);

    if (gameStarted == false && keys[SDL_SCANCODE_SPACE])
    {
        gameStarted = true;
    }

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

    //Enemy movement code
    enemy.xPosition += enemy.speed * enemy.direction * deltaTime;

    if (enemy.xPosition <= 1800.f)
    {
        enemy.direction = 1;
    }
    else if (enemy.xPosition >= 2300.f)
    {
        enemy.direction = -1;
    }

	//Player animation code
    if (moving)
    {
        Uint64 currentTime = SDL_GetTicks();

        if (currentTime - lastFrameTime >= frameDelay)
        {
            player.currentFrame = (player.currentFrame + 1) % numberOfFramesForKnight;
            lastFrameTime = currentTime;
        }
    }
    else
    {
        player.currentFrame = 1;
    }

    if (player.rolling)
    {
        Uint64 currentTime = SDL_GetTicks();

        if (currentTime - lastFrameTimeRolling >= rollingFrameDelay)
        {
            player.currentFrameRolling = (player.currentFrameRolling + 1) % numberOfFramesForKnight;
            lastFrameTimeRolling = currentTime;
        }
    }
    else
    {
        player.currentFrameRolling = 0;
    }

    //Coin animation code
    Uint64 currentTime = SDL_GetTicks();

    if (currentTime - lastCoinFrameTime >= coinFrameDelay)
    {
        currentCoinFrame = (currentCoinFrame + 1) % numberOfCoinFrames;
        lastCoinFrameTime = currentTime;
    }

    //Flag animation code

    if (flag.levelCompleted == false)
    {
        if (currentTime - lastFlagFrameTime >= flag.frameDelay)
        {
            flag.currentFrame = (flag.currentFrame + 1) % numberOfFlagFrames;
            lastFlagFrameTime = currentTime;
        }
    }

	//Enemy animation code
    if (currentTime - lastEnemyFrameTime >= enemy.frameDelay)
    {
        enemy.currentFrame = (enemy.currentFrame + 1) % numberOfFramesForEnemy;
        lastEnemyFrameTime = currentTime;
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

    //Coin collision code
    for (int i = 0; i < coinsList.size(); i++)
    {
        if (!coinsList[i].collected)
        {
            int oldCoinCount = coinCount;

            collisionWithCoin(player.xPosition, player.yPosition,
                player.width, player.height,
                coinsList[i].xPosition, coinsList[i].yPosition,
                player.xVelocity, player.yVelocity,
                bottomReached,
                coinsList[i].collected,
                coinWidth, coinHeight, coinCount);

            if (coinCount != oldCoinCount)
            {
                SDL_DestroyTexture(coinTextTexture);
                SDL_DestroySurface(coinTextSurface);

                coinText = "Coins: " + std::to_string(coinCount);

                coinTextSurface = TTF_RenderText_Blended(font, coinText.c_str(), 0, primaryColour);
                coinTextTexture = SDL_CreateTextureFromSurface(renderer, coinTextSurface);
            }
        }
    }

    //Collision with enemy
    collisionWithEnemy(
        player.xPosition, player.yPosition,
        player.width, player.height,
        enemy.xPosition, enemy.yPosition,
        player.xVelocity, player.yVelocity,
        bottomReached,
        enemy.killed,
        enemyTimer,
        enemy.width,
        enemy.height
    );

    //Respawn logic
    if (enemy.killed == true)
    {
        Uint64 currentTimeCheck = SDL_GetTicks();

        if (currentTimeCheck - enemyTimer >= 3000.f)
        {
            enemy.xPosition = 1800.f;
            enemy.yPosition = 575.f;
            enemy.killed = false;
        }
    }


    //Flag collision code
    if (collisionWithFlag(player.xPosition, player.yPosition,
        player.width, player.height,
        flag.xPosition, flag.yPosition,
        flag.width, flag.height) && flag.levelCompleted == false)
    {
        flag.levelCompleted = true;
    }

}






void Game::render()
{
	// Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

	// Background rendering
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

    // Ground rendering
    float groundWidth = screenWidth;
    float groundHeight = 80.f;

    float groundScrollX = cameraX;

    while (groundScrollX >= groundWidth)
    {
        groundScrollX -= groundWidth;
    }

    while (groundScrollX < 0)
    {
        groundScrollX += groundWidth;
    }

    SDL_FRect ground1 =
    {
        -groundScrollX,
        groundY,
        groundWidth,
        groundHeight
    };

    SDL_FRect ground2 =
    {
        groundWidth - groundScrollX,
        groundY,
        groundWidth,
        groundHeight
    };

    SDL_RenderTexture(renderer, groundTexture, NULL, &ground1);
    SDL_RenderTexture(renderer, groundTexture, NULL, &ground2);

    // Platform rendering
    for (int i = 0; i < platforms.size(); i++)
    {
        SDL_FRect rectPlatform =
        {
            platforms[i].xPosition - cameraX,
            platforms[i].yPosition,
            platforms[i].width,
            platforms[i].height
        };

        SDL_RenderTexture(renderer, platformTexture, NULL, &rectPlatform);
    }

    //Coins
   
    for (int i = 0; i < coinsList.size(); i++)
    {
        if (!coinsList[i].collected)
        {
            SDL_FRect rectCoin =
            {
                coinsList[i].xPosition - cameraX,
                coinsList[i].yPosition,
                45.0f,
                45.0f * scaleHeight
            };

            SDL_RenderTexture(renderer, coinFrames[currentCoinFrame], NULL, &rectCoin);
        }
    }

    SDL_FRect coinTextRect =
    {
        screenWidth - 320.f,
        25.f,
        280.f,
        60.f
    };

    SDL_RenderTexture(renderer, coinTextTexture, NULL, &coinTextRect);

   

    //Flag rendering

    SDL_FRect rectFlag =
    {
        flag.xPosition - cameraX,
        flag.yPosition,
        (float)flag.width,
        (float)flag.height
    };

    SDL_RenderTexture(renderer, flagFrames[flag.currentFrame], NULL, &rectFlag);


    //Enemy rendering
    SDL_FRect rectEnemy =
    {
        enemy.xPosition - cameraX,
        enemy.yPosition,
        enemy.width * scaleWidth,
        (float)enemy.height
    };

    SDL_RenderTexture(renderer, enemyRunFrameForwards[enemy.currentFrame], NULL, &rectEnemy);
    
    //Text
    if (gameStarted == false)
    {
        SDL_FRect titleShadowRect =
        {
            (screenWidth / 2.0f) - 505.f,
            115.f,
            1010.f,
            120.f
        };

        SDL_FRect titleTextRect =
        {
            (screenWidth / 2.0f) - 500.f,
            110.f,
            1000.f,
            120.f
        };

        SDL_FRect startTextRect =
        {
            (screenWidth / 2.0f) - 335.f,
            330.f,
            670.f,
            65.f
        };

        SDL_RenderTexture(renderer, titleShadowTexture, NULL, &titleShadowRect);
        SDL_RenderTexture(renderer, titleTextTexture, NULL, &titleTextRect);
        SDL_RenderTexture(renderer, startTextTexture, NULL, &startTextRect);
    }

    if (flag.levelCompleted)
    {
        SDL_FRect levelCompleteShadowRect =
        {
            245.f,
            145.f,
            1000.f,
            100.f
        };

        SDL_FRect levelCompleteTextRect =
        {
            250.f,
            150.f,
            1000.f,
            100.f
        };

        SDL_RenderTexture(renderer, levelCompleteShadowTexture, NULL, &levelCompleteShadowRect);
        SDL_RenderTexture(renderer, levelCompleteTextTexture, NULL, &levelCompleteTextRect);
    }

    // Player rendering
    SDL_FRect rectKnight =
    {
        player.xPosition - cameraX,
        player.yPosition,
        100.0f,
        100.0f * scaleHeight
    };

    SDL_FRect rollingKnight =
    {
        player.xPosition - cameraX,
        player.yPosition + 20,
        80.0f,
        100.0f
    };

    if (player.rolling)
    {
        if (player.facingRight)
        {
            SDL_RenderTexture(renderer, rollingRightAnimation[player.currentFrameRolling], NULL, &rollingKnight);
        }
        else
        {
            SDL_RenderTexture(renderer, rollingLeftAnimation[player.currentFrameRolling], NULL, &rollingKnight);
        }
    }
    else
    {
        if (player.facingRight)
        {
            SDL_RenderTexture(renderer, runFramesForwards[player.currentFrame], NULL, &rectKnight);
        }
        else
        {
            SDL_RenderTexture(renderer, runFramesBackwards[player.currentFrame], NULL, &rectKnight);
        }
    }
    SDL_RenderPresent(renderer);
}