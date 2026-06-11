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

struct PlayerStatePacket
{
    float playerX = 0.f;
    float playerY = 0.f;

    float otherPlayerX = 0.f;
    float otherPlayerY = 0.f;
};

class NetworkManager
{
public:
    
    bool init();
    void clean();

    bool getHasReceivedPlayerState();
    bool hostServer();
    bool connectToServer(const char* ipAddress);

    void pollEvents();
    void sendInput(bool left, bool right, bool jump, bool roll);
    
    PlayerInputPacket getLatestClientInput();
    

    
    void sendPlayerState(float playerX, float playerY, float otherPlayerX, float otherPlayerY);
    PlayerStatePacket getLatestPlayerState();

    bool getIsServer();
    bool getIsClient();

private:
    bool enetStarted = false;

    ENetHost* host = nullptr;
    ENetPeer* peer = nullptr;

    bool isServer = false;
    bool isClient = false;
    bool hasReceivedPlayerState = false;

    PlayerInputPacket latestClientInput;
    PlayerStatePacket latestPlayerState;
};