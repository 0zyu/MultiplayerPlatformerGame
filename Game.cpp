#include "Game.h"


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

    return true;
}


void Game::run()
{

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

}

void Game::update(float deltaTime)
{

}

void Game::render()
{

}