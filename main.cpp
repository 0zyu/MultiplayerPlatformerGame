#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"


using namespace std;


int main(int argc, char* argv[])
{


   
    Player player;
    Enemy enemy;
    
    //Gravity variables
    float gravity = 1800.0f; //gravity strength and how much it pulls down
    float jumpStrength = -700.0f; //how high the player will jump
    float speed = 500.0f;

    //Camera variables
    float screenWidth = 1500;
    float screenHeight = 800;
    float scrollX = 0;
    float cameraX = 0.0f;
    float backgroundScrollX = cameraX * 0.3f; // smaller = slower parallax
    float rollingFrameDelay = 100.f;
    
    

    //Platform variables
    bool bottomReached = false;
    float platformWidth = 200.f;
    float platformHeight = 50.f;
    int numberOfPlatforms = 10;

    //Texture variables
    float width;
    float height;
    float scaleHeight = 1.2f; //just to stretch the image out vertically a bit
    float scaleWidth = 1.2f; //just to stretch the image out horizontally a bit

	//Enemy variables
    enemy.lastFrameTime = SDL_GetTicks();

    //Timer/Frame variables
    int i = 0;
    int frameDelay = 100; // milliseconds
    Uint64 lastFrameTime = SDL_GetTicks();
    Uint64 lastEnemyFrameTime = SDL_GetTicks();
    int coinFrameDelay = 100;
    int currentCoinFrame = 0;

    Uint64 lastCoinFrameTime = SDL_GetTicks();
    Uint64 lastFrameTimeRolling = SDL_GetTicks();
    Uint64 enemyTimer = 0.f;
    Uint64 previousTime = SDL_GetTicks();

    //Input variables
    bool wWasPressed = false;
   
    bool spaceWasPressed = false;
    
    bool gameLoop = true;
    bool gameStarted = false;

    //Moving platform variables
    float xMovingPlatformPosition = 1300.f;
    float xMovingPlatformSpeed = 200.f;
    float xMovingPlatformDirection = 1;
    float yMovingPlatformPosition = 560.f;
    float yMovingPlatformSpeed = 200.f;
    float yMovingPlatformDirection = 1;

    //flag variables
    float flagX = 5400.f;
    float flagY = 450.f;
    bool levelCompleted = false;

    //Surfaces
    SDL_Surface* surfaceKnight = IMG_Load("assets/knightSprite1.png");
    SDL_Surface* surfacePlatform = IMG_Load("assets/platform.png");
    SDL_Surface* surfaceBackground = IMG_Load("assets/Clouds.png");
    SDL_Surface* surfaceGround = IMG_Load("assets/ground.png");
    SDL_Surface* surfaceEnemy = IMG_Load("assets/enemy1.png");
    SDL_Surface* surfaceRolling = IMG_Load("assets/rollRight1.png");
    SDL_Surface* surfaceCoin = IMG_Load("assets/coin1.png");
    SDL_Surface* surfaceFlag = IMG_Load("assets/betterflag1.png");


    //Text colours
    SDL_Color primaryColour = { 255, 255, 255, 255 };
    SDL_Color secondaryColour = { 0,0,0,0 };

    SDL_Event event;

    //Platform limits
    float platformLimitLeft = 700.f;
    float platformLimitRight = 1300.f;
    float platformLimitTop = 250.f;
    float platformLimitBottom = 650.f;

    //Ground coordinates
    float groundY = 750.f;

    //Coin variables
    int coinWidth = 45;
    int coinHeight = 54;
    bool coinCollected = false;
    Uint64 coinTimer = 0.f;
    int coinCount = 0;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) //starts SDLs video system so i can create a window and draw the graphics, if it fails then print that
    {
        cout << "SDL failed: " << SDL_GetError() << endl;
        return -1;
    }


    TTF_Init();

    if (TTF_Init() < 0)
    {
        cout << SDL_GetError() << endl;
    }

    TTF_Font* font = TTF_OpenFont("assets/PixelOperator8-Bold.ttf", 95);
    if (!font)
    {
        cout << SDL_GetError() << endl;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Platformer", screenWidth, screenHeight, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if (!window)
    {
        cout << "Window failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!renderer)
    {
        cout << "Renderer failed: " << SDL_GetError() << endl;
        return -1;
    }


    if (!surfaceFlag)
    {
        cout << "Flag image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfaceCoin)
    {
        cout << "Coin image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfaceRolling)
    {
        cout << "Rolling image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfaceKnight)
    {
        cout << "Knight image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfacePlatform)
    {
        cout << "Platform image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfaceBackground)
    {
        cout << "Background image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfaceGround)
    {
        cout << "Ground image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfaceEnemy)
    {
        cout << "Enemy image failed: " << SDL_GetError() << endl;
        return -1;
    }


    SDL_Texture* flagTexture = SDL_CreateTextureFromSurface(renderer, surfaceFlag);
    SDL_SetTextureScaleMode(flagTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceFlag);

    SDL_Texture* coinTexture = SDL_CreateTextureFromSurface(renderer, surfaceCoin);
    SDL_SetTextureScaleMode(coinTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceCoin);

    SDL_Texture* rollingTexture = SDL_CreateTextureFromSurface(renderer, surfaceRolling);
    SDL_SetTextureScaleMode(rollingTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceRolling);

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    SDL_DestroySurface(surfaceBackground);

    SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, surfaceEnemy);
    SDL_SetTextureScaleMode(enemyTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceEnemy);

    SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, surfaceKnight);
    SDL_SetTextureScaleMode(knightTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceKnight);

    SDL_Texture* platformTexture = SDL_CreateTextureFromSurface(renderer, surfacePlatform);
    SDL_SetTextureScaleMode(platformTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfacePlatform);

    SDL_Texture* groundTexture = SDL_CreateTextureFromSurface(renderer, surfaceGround);
    SDL_SetTextureScaleMode(groundTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceGround);

    //text



    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, "Knight Jump!", 0, primaryColour);
    SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, "Knight Jump!", 0, secondaryColour);
    SDL_Surface* textSurface3 = TTF_RenderText_Blended(font, "Level Complete!", 0, primaryColour);
    SDL_Surface* textSurface4 = TTF_RenderText_Blended(font, "Level Complete!", 0, secondaryColour);
    SDL_Surface* textSurface5 = TTF_RenderText_Blended(font, "Press SPACE to Start", 0, secondaryColour);
    SDL_Surface* textSurface6 = TTF_RenderText_Blended(font, "Coins: ", 0, primaryColour);


    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
    SDL_Texture* textTexture4 = SDL_CreateTextureFromSurface(renderer, textSurface4);
    SDL_Texture* textTexture5 = SDL_CreateTextureFromSurface(renderer, textSurface5);
    SDL_Texture* textTexture6 = SDL_CreateTextureFromSurface(renderer, textSurface6);

    string coinText = "Coins: " + to_string(coinCount);
    SDL_Surface* textSurface7 = TTF_RenderText_Blended(font, coinText.c_str(), 0, primaryColour);
    SDL_Texture* textTexture7 = SDL_CreateTextureFromSurface(renderer, textSurface7);

    const bool* keys = SDL_GetKeyboardState(NULL);

    const int numberOfFramesForKnight = 8;
    const int numberOfFramesForEnemy = 12;

    SDL_Texture* rollingRightAnimation[numberOfFramesForKnight];
    for (int i = 0; i < 8; i++)
    {
        string filePath = "assets/rollRight" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        rollingRightAnimation[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); //filled the array with texture surfaces

        SDL_SetTextureScaleMode(rollingRightAnimation[i], SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(tempSurface);
    }

    SDL_Texture* rollingLeftAnimation[numberOfFramesForKnight];
    for (int i = 0; i < 8; i++)
    {
        string filePath = "assets/rollLeft" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        rollingLeftAnimation[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); //filled the array with texture surfaces
        SDL_SetTextureScaleMode(rollingLeftAnimation[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);
    }

    //enemy animation for going forward (Not done one for backwards)

    SDL_Texture* enemyRunFrameForwards[numberOfFramesForEnemy];
    for (int i = 0; i < 12; i++)
    {
        string filePath = "assets/enemy" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        enemyRunFrameForwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); //filled the array with texture surfaces

        SDL_SetTextureScaleMode(enemyRunFrameForwards[i], SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(tempSurface);
    }

    //sprite animation for going forward

    SDL_Texture* runFramesForwards[numberOfFramesForKnight];
    for (int i = 0; i < 8; i++)
    {
        string filePath = "assets/knightSprite" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        runFramesForwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); //filled the array with texture surfaces

        SDL_SetTextureScaleMode(runFramesForwards[i], SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(tempSurface);
    }


    //commit check
    //sprite animation for going backwards

    SDL_Texture* runFramesBackwards[numberOfFramesForKnight];
    for (int i = 0; i < 8; i++)
    {
        string filePath = "assets/knightSpriteReverse" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());


        if (!tempSurface)
        {
            cout << "Failed to load: " << filePath << endl;
            cout << SDL_GetError() << endl;
            return -1;
        }

        runFramesBackwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);

        SDL_SetTextureScaleMode(runFramesBackwards[i], SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(tempSurface);

    }

    const int numberOfCoinFrames = 8;
    SDL_Texture* coins[numberOfCoinFrames];
    for (int i = 0; i < numberOfCoinFrames; i++)
    {
        string filePath = "assets/coin" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        coins[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); //filled the array with texture surfaces
        SDL_SetTextureScaleMode(coins[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);

    }


    //coin positions
    vector<float> coinXPositions = { 525.f, 1575.f, 2075.f ,3275.f ,4375.f };
    vector<float> coinYPositions = { 550.f, 350.f , 475.f, 475.f, 575.f };



    const int numberOfFlagFrames = 6;
    SDL_Texture* flags[numberOfFlagFrames];
    for (int i = 0; i < numberOfFlagFrames; i++)
    {
        string filePath = "assets/betterflag" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());

        flags[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); //filled the array with texture surfaces
        SDL_SetTextureScaleMode(flags[i], SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(tempSurface);

    }
    int currentFlagFrame = 0;
    int flagFrameDelay = 250;
    Uint64 lastFlagFrameTime = SDL_GetTicks();

    while (gameLoop)
    {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f; //the time between the last frame and the current frame in seconds
        previousTime = currentTime;



        float previousXMovingPlatformPosition = xMovingPlatformPosition; //gets the last position from the last frame, because after this line the position gets updated 

        xMovingPlatformPosition += xMovingPlatformSpeed * xMovingPlatformDirection * deltaTime; //the new position of the platform

        float xMovingPlatformDifference = xMovingPlatformPosition - previousXMovingPlatformPosition; //the difference between the two platforms, showing how much it has moved


        if (xMovingPlatformPosition < platformLimitLeft)
        {
            xMovingPlatformDirection = 1;
        }
        else if (xMovingPlatformPosition > platformLimitRight)
        {
            xMovingPlatformDirection = -1;
        }


        float previousYMovingPlatformPosition = yMovingPlatformPosition; //gets the last position from the last frame, because after this line the position gets updated 

        //positions is based on how fast it moves and direction being whether its moved up or down then multiplied by delta for same speed at different frame rates
        yMovingPlatformPosition += yMovingPlatformSpeed * yMovingPlatformDirection * deltaTime;

        float yMovingPlatformDifference = yMovingPlatformPosition - previousYMovingPlatformPosition; //the difference between the two platforms, showing how much it has moved

        if (yMovingPlatformPosition < platformLimitTop)
        {
            yMovingPlatformDirection = 1;
        }

        else if (yMovingPlatformPosition > platformLimitBottom)
        {
            yMovingPlatformDirection = -1;
        }


        //platform positions
        vector<float> platformXPositions = { 450.f, xMovingPlatformPosition, 1500.f ,1800.f ,2000.f ,2200.f ,2600.f ,3200.f ,3800.f ,4300.f };
        vector<float> platformYPositions = { 650.f, 575.f , 450.f, 650.f, 650.f, 650.f, yMovingPlatformPosition, 550.f, 650.f, 650.f };

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
            }

        }

        player.rolling = false;

        bool moving = false;
        player.xVelocity = 0;

        if (levelCompleted == false && gameStarted == true)
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

            //if (w key is pressed AND not moving)
            //then roll and move at the same time 

            if (keys[SDL_SCANCODE_SPACE] && player.onGround)
            {
                player.yVelocity = jumpStrength;
                player.onGround = false;
                bottomReached = false;
            }
        }

        if (moving)
        {
            Uint64 currentTime = SDL_GetTicks();

            if (currentTime - lastFrameTime >= frameDelay) //if the time between last time check and current time check is more than 100 milliseconds then change the frame of the sprite
            {
                player.currentFrame = (player.currentFrame + 1) % 8;

                lastFrameTime = currentTime;

            }
        }

        else
        {
            player.currentFrame = 1;
        }

        if (currentTime - lastEnemyFrameTime >= enemy.frameDelay)
        {
            enemy.currentFrame = (enemy.currentFrame + 1) % 12;
            lastEnemyFrameTime = currentTime;
        }

        if (currentTime - lastCoinFrameTime >= coinFrameDelay)
        {
            currentCoinFrame = (currentCoinFrame + 1) % 8;
            lastCoinFrameTime = currentTime;
        }

        //flag
        if (levelCompleted == false)
        {
            if (currentTime - lastFlagFrameTime >= flagFrameDelay)
            {
                currentFlagFrame = (currentFlagFrame + 1) % numberOfFlagFrames;
                lastFlagFrameTime = currentTime;
            }
        }
        //knight roll
        SDL_GetTextureSize(rollingTexture, &width, &height);
        SDL_FRect rollingKnight = { player.xPosition - cameraX, player.yPosition + 20, 80.0f, 100.0f };


        if (player.rolling)
        {
            if (currentTime - lastFrameTimeRolling >= rollingFrameDelay)
            {
                player.currentFrameRolling = (player.currentFrameRolling + 1) % 8;
                lastFrameTimeRolling = currentTime;
            }
        }
        else
        {
            player.currentFrameRolling = 0;
        }


        //gravity
        if (bottomReached == false)
        {
            player.yVelocity += gravity * deltaTime; //meaning every frame gravity is stronger as the yvelocity adds more gravity onto it EVERY frame, e.g. frame 1 its 0.2 then frame 2 its 0.4 etc
            player.yPosition += player.yVelocity * deltaTime;
        }

        else
        {
            bottomReached = false;
        }


        bottomReached = false;
        for (int i = 0; i < numberOfPlatforms; i++)
        {

            if (collision(player.xPosition, player.yPosition, player.width, player.height, platformXPositions[i], platformYPositions[i],
                player.yVelocity, bottomReached, screenWidth, screenHeight, platformWidth, platformHeight, player.xVelocity, yMovingPlatformDifference,
                xMovingPlatformDifference, i, xMovingPlatformDirection)
                )
            {

                break;
            }
        }


        SDL_FRect textRect3 = { 50 - cameraX - 5, 200.f, (float)textSurface3->w,(float)textSurface3->h };

        if (collisionWithFlag(player.xPosition, player.yPosition, player.width, player.height,
            flagX, flagY, 150, 300) && levelCompleted == false)
        {

            levelCompleted = true;

        }


        //touching ground separate from collision detection because it allows all platforms to be collided with, not just the first few 
        if (player.yPosition + player.height >= groundY)
        {
            player.yPosition = groundY - player.height;
            player.yVelocity = 0.0f;
            bottomReached = true;

        }
        player.onGround = bottomReached;

        //Camera movement so that the camera is always focussed on the player
        cameraX = player.xPosition - screenWidth / 2 + player.width / 2;


        // Screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        //must render stuff after this




        float backgroundWidth = screenWidth;
        float backgroundHeight = screenHeight;
        float backgroundScrollX = cameraX * 0.1f; //moves with cameras X position but slower because of the 0.1 multiplier, creating a parallax effect where the background moves slower than the foreground, giving the illusion of depth

        while (backgroundScrollX >= backgroundWidth) // keeps within one screen width, because once you scroll past 1500 it'd start again at 0
        {
            backgroundScrollX -= backgroundWidth;
        }

        while (backgroundScrollX < 0)
        {
            backgroundScrollX += backgroundWidth;
        }

        SDL_FRect background1 = { -backgroundScrollX, 0, backgroundWidth, backgroundHeight }; //as player moves right, this background moves left and the scrollX becomes negative to go left
        SDL_FRect background2 = { backgroundWidth - backgroundScrollX, 0, backgroundWidth, backgroundHeight }; // the second image so that once the player is beyond image width, this one loads, then this pattern can loop forever and constantly have an image in front, hence why the image had to be seamless

        SDL_RenderTexture(renderer, backgroundTexture, NULL, &background1);
        SDL_RenderTexture(renderer, backgroundTexture, NULL, &background2);


        //knight
        SDL_GetTextureSize(knightTexture, &width, &height);
        SDL_FRect rectKnight = { player.xPosition - cameraX, player.yPosition, 100.0f, 100.0f * scaleHeight };


        //enemy
        SDL_GetTextureSize(enemyTexture, &width, &height);

        enemy.xPosition += enemy.speed * enemy.direction * deltaTime; //move at a certain speed and depending on where it is, the direction will be +ve or -ve and changes, then multiplied by deltatime which is the time between frames to make sure it moves the same at different frame rates, e.g. if its 0.016 seconds between frames then it moves 16 pixels every frame, but if its 0.033 seconds between frames then it moves 33 pixels every frame, meaning it moves the same distance over time at different frame rates

        if (enemy.xPosition <= 1800.f)
        {
            enemy.direction = 1;
        }

        else if (enemy.xPosition >= 2300.f)
        {
            enemy.direction = -1;
        }

        SDL_FRect rectEnemyForwards = { enemy.xPosition - cameraX, enemy.yPosition, 80.0f * scaleWidth, 80.0f };

        SDL_RenderTexture(renderer, enemyRunFrameForwards[enemy.currentFrame], NULL, &rectEnemyForwards);

        collisionWithEnemy(player.xPosition, player.yPosition, player.width, player.height, enemy.xPosition, enemy.yPosition, player.xVelocity, player.yVelocity, bottomReached, enemy.killed, enemyTimer, enemy.width, enemy.height);


        if (enemy.killed == true)
        {
            Uint64 currentTimeCheck = SDL_GetTicks();

            if (currentTimeCheck - enemyTimer >= 3000.f)
            {
                enemy.xPosition = 1800.f;
                enemy.yPosition = 580.f;
                enemy.killed = false;
            }
        }


        //Coin
        for (int i = 0; i < 5; i++)
        {

            if (collisionWithCoin(player.xPosition, player.yPosition, player.width, player.height,
                coinXPositions[i], coinYPositions[i],
                player.xVelocity, player.yVelocity, bottomReached,
                coinCollected, coinWidth, coinHeight, coinCount))
            {
                coinYPositions[i] = 2000.f;

                SDL_DestroyTexture(textTexture7);
                SDL_DestroySurface(textSurface7);

                coinText = "Coins: " + to_string(coinCount);

                textSurface7 = TTF_RenderText_Blended(font, coinText.c_str(), 0, primaryColour);
                textTexture7 = SDL_CreateTextureFromSurface(renderer, textSurface7);
            }

        }


        //flag
        SDL_FRect rectFlag = { flagX - cameraX, flagY, 150.f, 300.f };

        SDL_RenderTexture(renderer, flags[currentFlagFrame], NULL, &rectFlag);

        //Ground
        float groundWidth = screenWidth;
        float groundHeight = 80.f;
        float groundY = 750.f;

        float groundScrollX = cameraX;

        while (groundScrollX >= groundWidth)
        {
            groundScrollX -= groundWidth;
        }

        while (groundScrollX < 0)
        {
            groundScrollX += groundWidth;
        }

        SDL_FRect textRect7 = { screenWidth - 500 , 10.f, (float)textSurface3->w / 3, (float)textSurface7->h / 2 };
        SDL_RenderTexture(renderer, textTexture7, NULL, &textRect7);

        //We need two grounds because one isnt enough, 
        SDL_FRect ground1 = { -groundScrollX, groundY, groundWidth, groundHeight };
        SDL_FRect ground2 = { groundWidth - groundScrollX, groundY, groundWidth, groundHeight };

        SDL_RenderTexture(renderer, groundTexture, NULL, &ground1);
        SDL_RenderTexture(renderer, groundTexture, NULL, &ground2);

        for (int i = 0; i < numberOfPlatforms; i++)
        {
            SDL_GetTextureSize(platformTexture, &width, &height);
            SDL_FRect rectPlatforms = { platformXPositions[i] - cameraX, platformYPositions[i], platformWidth, platformHeight };
            SDL_RenderTexture(renderer, platformTexture, NULL, &rectPlatforms);
        }



        for (int i = 0; i < 5; i++)
        {
            //coin
            SDL_GetTextureSize(coinTexture, &width, &height);
            SDL_FRect rectCoins = { coinXPositions[i] - cameraX, coinYPositions[i], 45.0f, 45.0f * scaleHeight };
            SDL_RenderTexture(renderer, coins[currentCoinFrame], NULL, &rectCoins);

        }



        //ben
        if (gameStarted == false)
        {

            SDL_FRect textRect1 = { -450 - cameraX, 100.f, (float)textSurface1->w,(float)textSurface1->h };
            SDL_FRect textRect2 = { -450 - cameraX - 5, 95.f, (float)textSurface2->w,(float)textSurface2->h };
            SDL_FRect textRect5 = { -375 - cameraX - 5, 300.f, (float)textSurface5->w / 2,(float)textSurface5->h / 2 };




            SDL_RenderTexture(renderer, textTexture2, NULL, &textRect2);
            SDL_RenderTexture(renderer, textTexture1, NULL, &textRect1);
            SDL_RenderTexture(renderer, textTexture5, NULL, &textRect5);

            if (keys[SDL_SCANCODE_SPACE])
            {
                gameStarted = true;

            }
        }


        if (levelCompleted)
        {

            SDL_FRect textRect3 =
            {
                (screenWidth - textSurface3->w) / 2.0f, 150.f, (float)textSurface3->w, (float)textSurface3->h
            };

            SDL_FRect textRect4 =
            {
                ((screenWidth - textSurface4->w) / 2.0f) - 5.f, 145.f, (float)textSurface4->w, (float)textSurface4->h
            };

            SDL_RenderTexture(renderer, textTexture4, NULL, &textRect4);

            SDL_RenderTexture(renderer, textTexture3, NULL, &textRect3);

        }

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

    SDL_FRect textRect7 = { screenWidth - textSurface7->w - 30.f, 30.f, (float)textSurface7->w, (float)textSurface7->h };

    SDL_RenderTexture(renderer, textTexture7, NULL, &textRect7);


    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        SDL_DestroyTexture(runFramesForwards[i]);
    }

    for (int i = 0; i < numberOfFramesForKnight; i++)
    {
        SDL_DestroyTexture(runFramesBackwards[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        SDL_DestroyTexture(coins[i]);
    }
    if (levelCompleted == false)
    {
        for (int i = 0; i < 5; i++)
        {
            SDL_DestroyTexture(flags[i]);
        }
    }

    SDL_DestroyTexture(rollingTexture);

    SDL_DestroyTexture(textTexture7);
    SDL_DestroySurface(textSurface7);

    SDL_DestroyTexture(textTexture5);
    SDL_DestroySurface(textSurface5);

    SDL_DestroyTexture(textTexture4);
    SDL_DestroySurface(textSurface4);

    SDL_DestroyTexture(textTexture3);
    SDL_DestroySurface(textSurface3);

    SDL_DestroyTexture(textTexture2);
    SDL_DestroySurface(textSurface2);

    SDL_DestroyTexture(textTexture1);
    SDL_DestroySurface(textSurface1);

    TTF_CloseFont(font);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(platformTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();


}