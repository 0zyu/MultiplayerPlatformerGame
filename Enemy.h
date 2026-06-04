#pragma once

#include <SDL3/SDL.h>

struct Enemy
{
    float xPosition = 1800.f;
    float yPosition = 575.f;

    float speed = 100.f;
    int direction = 1;

    int width = 80;
    int height = 80;

    bool killed = false;
    Uint64 timer = 0;

    int currentFrame = 0;
    int frameDelay = 100;
    Uint64 lastFrameTime = 0;
};