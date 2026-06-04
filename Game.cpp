#include "Game.h"
#include <SDL3/SDL.h>

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
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event event;

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