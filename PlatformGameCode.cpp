#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>

using namespace std;


bool collision(float& xPosition, float& yPosition, int& playerWidth, int& playerHeight, float& platformXPosition, float& platformYPosition, float& yVelocity, bool& bottomReached, float screenWidth, float screenHeight, float platformWidth, float platformHeight, float xVelocity)
{

    float playerLeft = xPosition;
    float playerRight = xPosition + playerWidth;
    float playerBottom = yPosition + playerHeight;
    float playerTop = yPosition;

    float platformLeft = platformXPosition;
    float platformRight = platformXPosition + platformWidth;
    float platformTop = platformYPosition;
    float platformBottom = platformYPosition + platformHeight;

    //Landing on platform
    if (playerBottom >= platformTop &&
        playerBottom <= platformTop + 20 && //+20 because it would fall through the platform without it, like a catching value
        playerRight > platformLeft &&
        playerLeft < platformRight &&
        yVelocity >= 0)
    {
        yPosition = platformTop - playerHeight;
        yVelocity = 0.0f;
        bottomReached = true;
        return bottomReached;
    }
    //Hitting bottom of platform
    else if (playerTop <= platformBottom &&
             playerTop >= platformBottom - 5 &&
             playerRight > platformLeft &&
             playerLeft < platformRight &&
             yVelocity <= 0)
    {
        yPosition = platformBottom + playerHeight;
        yVelocity = 0.0f;
        bottomReached = true;
        return bottomReached;
    }
    //Moving left side of platform
    else if (xVelocity > 0 && 
             playerRight > platformLeft &&
             playerLeft < platformLeft && 
             playerTop < platformBottom &&
             playerBottom > platformTop
             )
    {
        xPosition = platformLeft - playerWidth;
        xVelocity = 0;
        bottomReached = false;
        return true;
    }
    //Moving right side of platform
    else if (xVelocity < 0 && 
             playerRight > platformRight &&
             playerLeft < platformRight && 
             playerTop < platformBottom &&
             playerBottom > platformTop
        )
    {
        xPosition = platformRight;
        xVelocity = 0;
        bottomReached = false;
        return true;
    }
    
    else
    {
        bottomReached = false;
        return bottomReached;
    }
}



int main(int argc, char* argv[])
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) //starts SDLs video system so i can create a window and draw the graphics, if it fails then print that
    {
        cout << "SDL failed: " << SDL_GetError() << endl;
        return -1;
    }
    
    float screenWidth = 1500;
    float screenHeight = 800;


    // Create window (SDL3 version)
    SDL_Window* window = SDL_CreateWindow("Platformer", screenWidth, screenHeight, 0);

    if (!window)
    {
        cout << "Window failed: " << SDL_GetError() << endl;
        return -1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer)
    {
        cout << "Renderer failed: " << SDL_GetError() << endl;
        return -1;
    }

    SDL_Surface* surfaceKnight = IMG_Load("assets/knightSprite1.png");
    SDL_Surface* surfacePlatform = IMG_Load("assets/platform.png");
    SDL_Surface* surfaceBackground = IMG_Load("assets/Clouds.png");
    SDL_Surface* surfaceGround = IMG_Load("assets/ground.png");

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

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    
    
    SDL_DestroySurface(surfaceBackground);

    SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, surfaceKnight);
    SDL_SetTextureScaleMode(knightTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceKnight);

    SDL_Texture* platformTexture = SDL_CreateTextureFromSurface(renderer, surfacePlatform);
    SDL_SetTextureScaleMode(platformTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfacePlatform);
    
    SDL_Texture* groundTexture = SDL_CreateTextureFromSurface(renderer, surfaceGround);
    SDL_SetTextureScaleMode(groundTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceGround);


    const bool* keys = SDL_GetKeyboardState(NULL); 


    //sprite animation for going forward
    SDL_Texture* runFramesForwards[8];
    for (int i = 0; i < 8; i++)
    {
        string filePath = "assets/knightSprite" + to_string(i + 1) + ".png";
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        runFramesForwards[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);

        SDL_SetTextureScaleMode(runFramesForwards[i], SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(tempSurface);
    }

    //sprite animation for going backwards

    SDL_Texture* runFramesBackwards[8];
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


    // player spawn/position variable
    float xPosition = 150.f;
    float yPosition = 5.f;
    float scrollX = 0;
    float cameraX = 0.0f;
    float backgroundScrollX = cameraX * 0.3f; // smaller = slower parallax

    float xVelocity = 0.f; //how fast the player moves in the X Direction
    float yVelocity = 0.f; //how fast its falling/moving in Y direction
    
    float gravity = 1800.0f; //gravity strength and how much it pulls down
    float jumpStrength = -700.0f; //how high the player will jump
    float speed = 500.0f;

    float platformWidth = 200.f;
    float platformHeight = 50.f;

    int playerHeight = 120;
    int playerWidth = 100;
    int currentFrame = 0;




    Uint64 lastFrameTime = SDL_GetTicks();
    int frameDelay = 100; // milliseconds

    bool onGround = false;
    bool spaceWasPressed = false;
    bool facingRight = true;
    bool gameLoop = true;

    SDL_Event event;

    
    vector<float> platformXPositions = { 350.f, 1000.f ,1500.f ,1800.f ,2000.f ,2200.f ,2600.f ,3300.f ,3800.f ,4500.f };
    vector<float> platformYPositions = { 650.f, 550.f , 450.f, 650.f, 650.f, 650.f, 550.f, 550.f, 650.f, 650.f };

    Uint64 previousTime = SDL_GetTicks();
    bool bottomReached = false;
    while (gameLoop)
    {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
            }
            
        }


        bool moving = false;
        xVelocity = 0;

        if (keys[SDL_SCANCODE_D])
        {
            xVelocity = speed;
            xPosition += xVelocity * deltaTime;
            facingRight = true;
            moving = true;
        }

        if (keys[SDL_SCANCODE_A])
        {
            xVelocity = -speed;
            xPosition += xVelocity * deltaTime;
            facingRight = false;
            moving = true;
        }
        if (keys[SDL_SCANCODE_SPACE] && onGround)
        {
            yVelocity = jumpStrength;
            onGround = false;
            bottomReached = false;
        }
        
        
        if (moving)
        {
            Uint64 currentTime = SDL_GetTicks();

            if (currentTime - lastFrameTime >= frameDelay) //if the time between last time check and current time check is more than 100 milliseconds then change the frame of the sprite
            {
                currentFrame = (currentFrame + 1) % 8;
                lastFrameTime = currentTime;
            }
        }
        else
        {
            currentFrame = 1;
        }

        //gravity
        
        if (bottomReached == false)
        {
            yVelocity += gravity * deltaTime; //meaning every frame gravity is stronger as the yvelocity adds more gravity onto it EVERY frame, e.g. frame 1 its 0.2 then frame 2 its 0.4 etc
            yPosition += yVelocity * deltaTime;
        }
        
        else
        {
            bottomReached = false;
        }


        bottomReached = false;
        for (int i = 0; i < 10; i++)
        {

            if (collision(xPosition, yPosition, playerWidth, playerHeight, platformXPositions[i], platformYPositions[i], 
                yVelocity, bottomReached, screenWidth, screenHeight, platformWidth, platformHeight, xVelocity)
                )
            {
                
                break;
            }
        }
        //touching ground separate from collision detection because it allows all platforms to be collided with, not just the first few 
        if (yPosition + playerHeight >= 750.f)
        {
            yPosition = 750.f - playerHeight;
            yVelocity = 0.0f;
            bottomReached = true;
            
        }
        onGround = bottomReached;
        cameraX = xPosition - screenWidth / 2 + playerWidth / 2; 
        

        

        // Clear screen
      
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);

        float backgroundWidth = screenWidth;
        float backgroundHeight = screenHeight;

        float backgroundScrollX = cameraX * 0.3f; //moves with cameras X position 

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

        float width;
        float height;

        float scaleHeight = 1.2f; //just to stretch the image out vertically a bit

        //knight
        SDL_GetTextureSize(knightTexture, &width, &height);
        SDL_FRect rectKnight = { xPosition - cameraX, yPosition, 100.0f, 100.0f * scaleHeight};
        
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

        //We need two grounds because one isnt enough, 
        SDL_FRect ground1 = { -groundScrollX, groundY, groundWidth, groundHeight };
        SDL_FRect ground2 = { groundWidth - groundScrollX, groundY, groundWidth, groundHeight };

        SDL_RenderTexture(renderer, groundTexture, NULL, &ground1);
        SDL_RenderTexture(renderer, groundTexture, NULL, &ground2);
        
        for (int i = 0; i < 10; i++)
        {
            SDL_GetTextureSize(platformTexture, &width, &height);
            SDL_FRect rectPlatforms = { platformXPositions[i] - cameraX, platformYPositions[i], platformWidth, platformHeight};
            SDL_RenderTexture(renderer, platformTexture, NULL, &rectPlatforms);
        }

        if (facingRight)
        {
            SDL_RenderTexture(renderer, runFramesForwards[currentFrame], NULL, &rectKnight);
        }
        else
        {
            SDL_RenderTexture(renderer, runFramesBackwards[currentFrame], NULL, &rectKnight);
        }

        SDL_RenderPresent(renderer);
       
    }
   
   
    for (int i = 0; i < 8; i++)
    {
        SDL_DestroyTexture(runFramesForwards[i]);
    }
    for (int i = 0; i < 8; i++)
    {
        SDL_DestroyTexture(runFramesBackwards[i]);
    }

    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(platformTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}