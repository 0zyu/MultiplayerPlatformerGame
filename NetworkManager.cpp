#include "NetworkManager.h"

bool NetworkManager::init()
{
    if (enet_initialize() != 0)
    {
        std::cout << "ENet failed to initialise\n";
        return false;
    }

    enetStarted = true;
    std::cout << "ENet initialised\n";

    return true;
}

void NetworkManager::clean()
{
    if (enetStarted)
    {
        enet_deinitialize();
        enetStarted = false;
        std::cout << "ENet shut down\n";
    }
}