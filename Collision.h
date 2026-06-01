#pragma once

#include <SDL3/SDL.h>

bool collisionWithFlag(float xPosition, float yPosition, int playerWidth, int playerHeight,
    float flagX, float flagY, int flagWidth, int flagHeight);

bool collisionWithCoin(float& xPosition, float& yPosition, int& playerWidth, int& playerHeight,
    float& xCoinPosition, float& yCoinPosition,
    float& xVelocity, float& yVelocity, bool& bottomReached,
    bool& coinCollected, int& coinWidth, int& coinHeight, int& coinCount);

bool collisionWithEnemy(float& xPosition, float& yPosition, int& playerWidth, int& playerHeight,
    float& xEnemyPosition, float& yEnemyPosition,
    float& xVelocity, float& yVelocity, bool& bottomReached,
    bool& enemyKilled, Uint64& enemyTimer,
    int& enemyWidth, int& enemyHeight);

bool collision(float& xPosition, float& yPosition,
    int& playerWidth, int& playerHeight,
    float& platformXPosition, float& platformYPosition,
    float& yVelocity, bool& bottomReached,
    float screenWidth, float screenHeight,
    float platformWidth, float platformHeight,
    float xVelocity, float yMovingPlatformDifference,
    float xMovingPlatformDifference,
    int i, int xMovingPlatformDirection);