#pragma once

#include <enet/enet.h>
#include <iostream>

class NetworkManager
{
public:
    bool init();
    void clean();

    bool hostServer();
    bool connectToServer(const char* ipAddress);
    void pollEvents();

private:
    bool enetStarted = false;

    ENetHost* host = nullptr;
    ENetPeer* peer = nullptr;

    bool isServer = false;
    bool isClient = false;
};