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

    while (gameLoop)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameLoop = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw rectangle
        SDL_FRect rect = { 350.0f, 250.0f, 100.0f, 100.0f };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}