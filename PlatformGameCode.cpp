#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) //starts SDLs video system so i can create a window and draw the graphics, if it fails then print that
    {
        cout << "SDL failed: " << SDL_GetError() << endl;
        return -1;
    }
   
    // Create window (SDL3 version)
    SDL_Window* window = SDL_CreateWindow("Platformer", 1200, 1000, 0);

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

    if (!surfaceKnight)
    {
        cout << "Image failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (!surfacePlatform)
    {
        cout << "Image failed: " << SDL_GetError() << endl;
        return -1;
    }

    
    SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, surfaceKnight);
    SDL_SetTextureScaleMode(knightTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfaceKnight);


    SDL_Texture* platformTexture = SDL_CreateTextureFromSurface(renderer, surfacePlatform);
    SDL_SetTextureScaleMode(platformTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surfacePlatform);


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


    // player position
    float xPosition = 350.f;
    float yPosition = 5.f;

    float platform1XPosition = 450.f;
    float platform1YPosition = 900.f;

    float yVelocity = 0.f;
    float gravity = 0.003f;
    float jumpStrength = -0.90f;


    int playerHeight = 120;
    int playerWidth = 100;
    int currentFrame = 0;




    Uint64 lastFrameTime = SDL_GetTicks();
    int frameDelay = 100; // milliseconds

    bool facingRight = true;
    bool gameLoop = true;
    SDL_Event event;

    bool bottomReached = false;
    while (gameLoop)
    {

        float playerLeft = xPosition;
        float playerRight = xPosition + playerWidth;
        float playerBottom = yPosition + playerHeight;

        float platformLeft = platform1XPosition;
        float platformRight = platform1XPosition + 100;
        float platformTop = platform1YPosition;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_SPACE && bottomReached == true)
                {
                    yVelocity = jumpStrength;
                    bottomReached = false;
                }
            }
        }

        float speed = 0.5f;

        bool moving = false;

        if (keys[SDL_SCANCODE_D])
        {
            xPosition += speed;
            facingRight = true;
            moving = true;
        }

        if (keys[SDL_SCANCODE_A])
        {
            xPosition -= speed;
            facingRight = false;
            moving = true;
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
            yVelocity += gravity; //meaning every frame gravity is stronger as the yvelocity adds more gravity onto it EVERY frame, e.g. frame 1 its 0.2 then frame 2 its 0.4 etc
            yPosition += yVelocity;
        }
        if (yPosition + playerHeight >= 1000  )
        {
            yPosition = 1000 - playerHeight;
            yVelocity = 0.0f;
            bottomReached = true;
        }
        else
        {
            bottomReached = false;
        }

        //collision
        if (playerBottom >= platformTop &&
            playerBottom <= platformTop + 20 &&
            playerRight > platformLeft &&
            playerLeft < platformRight &&
            yVelocity >= 0)
        {
            yPosition = platformTop - playerHeight;
            yVelocity = 0.0f;
            bottomReached = true;
        }
        else if (yPosition + playerHeight >= 1000)
        {
            yPosition = 1000 - playerHeight;
            yVelocity = 0.0f;
            bottomReached = true;
        }
        else
        {
            bottomReached = false;
        }


        // Clear screen
      
        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        SDL_RenderClear(renderer);

        float width;
        float height;

        //knight
        SDL_GetTextureSize(knightTexture, &width, &height);
        float scaleHeight = 1.2f; //just to stretch the image out vertically a bit
        SDL_FRect rectKnight = { xPosition, yPosition, 100.0f, 100.0f * scaleHeight};
        
        //platform
        SDL_GetTextureSize(platformTexture, &width, &height);
        SDL_FRect rectPlatform = { platform1XPosition, platform1YPosition, 100.0f, 50.0f };        
        SDL_RenderTexture(renderer, platformTexture, NULL, &rectPlatform);


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
    SDL_DestroyTexture(platformTexture);
    for (int i = 0; i < 8; i++)
    {
        SDL_DestroyTexture(runFramesForwards[i]);
    }
    for (int i = 0; i < 8; i++)
    {
        SDL_DestroyTexture(runFramesBackwards[i]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}