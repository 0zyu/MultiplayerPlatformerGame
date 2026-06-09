#pragma once

#include <enet/enet.h>
#include <iostream>

class NetworkManager
{
public:
    bool init();
    void clean();

private:
    bool enetStarted = false;
};