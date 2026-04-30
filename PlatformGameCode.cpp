#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>

using namespace std;

int main(int argc, char* argv[])
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

    bool gameLoop = true;
    SDL_Event event;

    // Draw rectangle
    float rectX = 350.f;
    float rectY = 250.f;
    
    
    SDL_Surface* surface = IMG_Load("assets/knight1.png");

    if (!surface)
    {
        cout << "Image failed: " << SDL_GetError() << endl;
        return -1;
    }
    SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    const bool* keys = SDL_GetKeyboardState(NULL);

    while (gameLoop)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
            }
        }

        float speed = 1.5f;

        if (keys[SDL_SCANCODE_D])
        {
            rectX += speed;
        }

        if (keys[SDL_SCANCODE_A])
        {
            rectX -= speed;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        SDL_RenderClear(renderer);

        float width;
        float height;

        SDL_GetTextureSize(knightTexture, &width, &height);
        float scale = 4.0f;

        SDL_FRect rect = { rectX, rectY, 100.0f * scale, 100.0f * scale * 1.5};
       // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
       // SDL_RenderFillRect(renderer, &rect);
        
        SDL_RenderTexture(renderer, knightTexture, NULL, &rect);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(knightTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}