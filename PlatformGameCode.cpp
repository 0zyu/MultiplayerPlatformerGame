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

    SDL_Surface* surface = IMG_Load("assets/knightSprite1.png");
    
    if (!surface)
    {
        cout << "Image failed: " << SDL_GetError() << endl;
        return -1;
    }

    
    SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(knightTexture, SDL_SCALEMODE_NEAREST); //unblurs the image and doesnt do any filtering etc
    SDL_DestroySurface(surface);

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

    float gravity = 0.2f;

    int playerHeight = 120;
    int currentFrame = 0;

    Uint64 lastFrameTime = SDL_GetTicks();
    int frameDelay = 100; // milliseconds

    bool facingRight = true;
    bool gameLoop = true;
    SDL_Event event;

    bool bottomReached = false;
    while (gameLoop)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
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


        yPosition += gravity; //makes the player fall
        if (bottomReached == false)
        {
            gravity += 0.002f; //increases its value so that it accelerates down and doesnt fall at a constant speed
        }
        if (yPosition + playerHeight >= 1000)
        {
            gravity = 0.f;
            bottomReached = true;
        }




        // Clear screen
      
        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        SDL_RenderClear(renderer);

        float width;
        float height;

        SDL_GetTextureSize(knightTexture, &width, &height);
        float scaleHeight = 1.2f; //just to stretch the image out vertically a bit

        SDL_FRect rect = { xPosition, yPosition, 100.0f, 100.0f * scaleHeight};
      
        if (facingRight)
        {
            SDL_RenderTexture(renderer, runFramesForwards[currentFrame], NULL, &rect);
        }
        else
        {
            SDL_RenderTexture(renderer, runFramesBackwards[currentFrame], NULL, &rect);
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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}