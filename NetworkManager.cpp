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

bool NetworkManager::hostServer()
{
    if (host != nullptr)
    {
        std::cout << "Network already running\n";
        return false;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    host = enet_host_create(
        &address,
        2,
        2,
        0,
        0
    );

    if (!host)
    {
        std::cout << "Failed to host server\n";
        return false;
    }

    isServer = true;
    isClient = false;

    std::cout << "Server hosted on port 1234\n";

    return true;
}

bool NetworkManager::connectToServer(const char* ipAddress)
{
    if (host != nullptr)
    {
        std::cout << "Network already running\n";
        return false;
    }

    host = enet_host_create(
        nullptr,
        1,
        2,
        0,
        0
    );

    if (!host)
    {
        std::cout << "Failed to create client\n";
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, ipAddress);
    address.port = 1234;

    peer = enet_host_connect(host, &address, 2, 0);

    if (!peer)
    {
        std::cout << "Failed to create connection peer\n";
        enet_host_destroy(host);
        host = nullptr;
        return false;
    }

    isClient = true;
    isServer = false;

    std::cout << "Trying to connect to server...\n";

    return true;
}

void NetworkManager::pollEvents()
{
    if (!host)
    {
        return;
    }

    ENetEvent event;

    while (enet_host_service(host, &event, 0) > 0)
    {
        if (event.type == ENET_EVENT_TYPE_CONNECT)
        {
            if (isServer)
            {
                std::cout << "Client connected to server\n";
            }
            else if (isClient)
            {
                std::cout << "Connected to server\n";
            }
        }
        else if (event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            std::cout << "Packet received\n";
            enet_packet_destroy(event.packet);
        }
        else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
        {
            std::cout << "Disconnected\n";
        }
    }
}

void NetworkManager::clean()
{
    if (host)
    {
        enet_host_destroy(host);
        host = nullptr;
        peer = nullptr;
    }

    if (enetStarted)
    {
        enet_deinitialize();
        enetStarted = false;
        std::cout << "ENet shut down\n";
    }
}