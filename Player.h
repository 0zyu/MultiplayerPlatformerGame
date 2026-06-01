#pragma once

struct Player
{
    float xPosition = 5.f;
    float yPosition = 5.f;

    float xVelocity = 0.f;
    float yVelocity = 0.f;

    int width = 100;
    int height = 120;

    bool rolling = false;
    bool onGround = false;
    bool facingRight = true;

    int currentFrame = 0;
    int currentFrameRolling = 0;
};