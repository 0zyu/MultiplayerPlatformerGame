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

	ENetAddress address; //address struct to hold the address and port we want to host on
	address.host = ENET_HOST_ANY; //ENET_HOST_ANY is a constant that tells ENet to listen on all available network interfaces
	address.port = 1234; //the port we want to host on, this can be any port number that is not being used by another application

    host = enet_host_create(
        &address,
        2,
        2,
        0,
        0
    );

	if (host == nullptr) //if host doesnt point to anything then it failed to create a host
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

	// ENet creates the networking object somewhere in memory and gives you back its address
    host = enet_host_create(
        nullptr,
        1,
        2,
        0,
        0
    );

    //Therefore now we have the objects address host now pointing to a real ENet networking object
    if (host == nullptr)
    {
        std::cout << "Failed to create client\n";
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, ipAddress);
    address.port = 1234;

    peer = enet_host_connect(host, &address, 2, 0);

    if (peer == nullptr)
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
    if (host == nullptr)
    {
        return;
    }

    ENetEvent event;

    while (enet_host_service(host, &event, 0) > 0)
    {
        if (event.type == ENET_EVENT_TYPE_CONNECT)
        {
            if (isServer == true)
            {
                std::cout << "Client connected to server\n";
            }
            else if (isClient == true)
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
    if (host != nullptr)
    {
        enet_host_destroy(host);
        host = nullptr;
        peer = nullptr;
    }

    if (enetStarted == true)
    {
        enet_deinitialize();
        enetStarted = false;
        std::cout << "ENet shut down\n";
    }
}