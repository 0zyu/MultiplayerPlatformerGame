#pragma once

struct Flag
{
    float xPosition = 5400.f;
    float yPosition = 450.f;

    int width = 150;
    int height = 300;

    bool levelCompleted = false;

    int currentFrame = 0;
    int frameDelay = 250;
};