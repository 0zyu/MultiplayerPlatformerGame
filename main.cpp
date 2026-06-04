#include "Game.h"




int main(int argc, char* argv[])
{
    Game game;

    if (!game.init())
    {
        return -1;
    }

    game.run();
    game.clean();

    return 0;
}