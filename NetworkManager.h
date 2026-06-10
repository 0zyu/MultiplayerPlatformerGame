#pragma once

#include <enet/enet.h>
#include <iostream>


struct PlayerInputPacket
{
    bool left = false;
    bool right = false;
    bool jump = false;
    bool roll = false;
};


class NetworkManager
{
public:
    void sendInput(bool left, bool right, bool jump, bool roll);

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