#include <SDL3/SDL.h>
#include <iostream>

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
    SDL_Window* window = SDL_CreateWindow("Platformer", 800, 600, 0);

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

    while (gameLoop)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_D)
                {
                    rectX += 15.f;
                }
                else if (event.key.key == SDLK_A)
                {
                    rectX -= 15.f;
                }
            }

        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0,0);
        SDL_RenderClear(renderer);

        
        SDL_FRect rect = { rectX, rectY, 100.0f, 100.0f };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    
       
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}