#include "Collision.h"
#include <SDL3/SDL.h>
#include <iostream>

using namespace std;


bool collisionWithFlag(float xPosition, float yPosition, int playerWidth, int playerHeight,
    float flagX, float flagY, int flagWidth, int flagHeight)
{
    float playerLeft = xPosition;
    float playerRight = xPosition + playerWidth;
    float playerTop = yPosition;
    float playerBottom = yPosition + playerHeight;

    float flagLeft = flagX;
    float flagRight = flagX + flagWidth;
    float flagTop = flagY;
    float flagBottom = flagY + flagHeight;

    if (playerRight > flagLeft &&
        playerLeft < flagRight &&
        playerBottom > flagTop &&
        playerTop < flagBottom)
    {

        return true;
    }

    return false;
}



bool collisionWithCoin(float& xPosition, float& yPosition, int& playerWidth, int& playerHeight, float& xCoinPosition, float& yCoinPosition,
    float& xVelocity, float& yVelocity, bool& bottomReached, bool& coinCollected, int& coinWidth, int& coinHeight, int& coinCount)
{
    float playerLeft = xPosition;
    float playerRight = xPosition + playerWidth;
    float playerBottom = yPosition + playerHeight;
    float playerTop = yPosition;

    float coinLeft = xCoinPosition;
    float coinRight = xCoinPosition + coinWidth;
    float coinTop = yCoinPosition;
    float coinBottom = yCoinPosition + coinHeight;

    int paddingValue = 5;
    //Left side of coin
    if (playerRight > coinLeft &&
        playerLeft < coinLeft &&
        playerTop <= coinBottom &&
        playerBottom >= coinTop)
    {
        coinCount += 1;
        coinCollected = true;
        return coinCollected;
    }
    //Right side of coin
    else if (playerRight > coinRight &&
        playerLeft < coinRight &&
        playerTop <= coinBottom &&
        playerBottom >= coinTop)
    {
        coinCount += 1;
        coinCollected = true;
        return coinCollected;
    }
    //Top of coin
    else if (playerBottom >= coinTop &&
        playerBottom <= coinTop + paddingValue &&
        playerRight > coinLeft + paddingValue &&
        playerLeft < coinRight - paddingValue)
    {
        coinCount += 1;
        coinCollected = true;
        return coinCollected;
    }

    return false;
}

bool collisionWithEnemy(float& xPosition, float& yPosition, int& playerWidth, int& playerHeight, float& xEnemyPosition, float& yEnemyPosition,
    float& xVelocity, float& yVelocity, bool& bottomReached, bool& enemyKilled, Uint64& enemyTimer, int& enemyWidth, int& enemyHeight)
{
    float playerLeft = xPosition;
    float playerRight = xPosition + playerWidth;
    float playerBottom = yPosition + playerHeight;
    float playerTop = yPosition;

    float enemyLeft = xEnemyPosition;
    float enemyRight = xEnemyPosition + enemyWidth;
    float enemyTop = yEnemyPosition;
    float enemyBottom = yEnemyPosition + enemyHeight;

    int paddingValue = 20; //Makes the collision feel more fair as sprites have empty space around them
    //need to do top, right, and left of the enemy, exact same as platform logic but without the bottom

    //landing on top of enemy
    if (playerBottom >= enemyTop &&
        playerBottom <= enemyTop + paddingValue &&
        playerRight > enemyLeft + paddingValue &&
        playerLeft < enemyRight - paddingValue &&
        yVelocity >= 0
        )
    {
        //yPosition = enemyTop - playerHeight;
        yEnemyPosition = 2000.f; //moves enemy off screen instead of deleting it, because deleting it would cause problems with the animation frames and texture rendering etc

        enemyTimer = SDL_GetTicks(); //starts the timer for when the enemy is killed, so that after a certain time it can respawn
        enemyKilled = true;
        bottomReached = true;
        return bottomReached;
    }

    //left side of enemy hit
    else if (playerRight > enemyLeft &&
        playerLeft < enemyLeft &&
        playerTop <= enemyBottom &&
        playerBottom >= enemyTop &&
        xVelocity >= 0)
    {
        xPosition = 500.f;
        xVelocity = 0;
        enemyKilled = false;
        bottomReached = false;
        return true;
    }

    //right side of enemy hit
    else if (playerLeft < enemyRight &&
        playerRight > enemyRight &&
        playerTop <= enemyBottom &&
        playerBottom >= enemyTop &&
        xVelocity <= 0)
    {
        xPosition = 500.f;
        xVelocity = 0;
        enemyKilled = false;
        bottomReached = false;
        return true;
    }
    return false;

}

bool collision(float& xPosition, float& yPosition, int& playerWidth, int& playerHeight, float& platformXPosition,
    float& platformYPosition, float& yVelocity, bool& bottomReached, float screenWidth, float screenHeight,
    float platformWidth, float platformHeight, float xVelocity, float yMovingPlatformDifference,
    float xMovingPlatformDifference, int i, int xMovingPlatformDirection)
{

    float playerLeft = xPosition;
    float playerRight = xPosition + playerWidth;
    float playerBottom = yPosition + playerHeight;
    float playerTop = yPosition;

    float platformLeft = platformXPosition;
    float platformRight = platformXPosition + platformWidth;
    float platformTop = platformYPosition;
    float platformBottom = platformYPosition + platformHeight;

    int paddingValue = 20;

    //Landing on platform
    if (playerBottom >= platformTop &&
        playerBottom <= platformTop + paddingValue &&
        playerRight > platformLeft &&
        playerLeft < platformRight &&
        yVelocity >= 0)
    {
        if (i != 6)
        {

            yPosition = platformTop - playerHeight;
            yVelocity = 0;
        }
        if (i == 1)
        {
            xPosition += xMovingPlatformDifference; //Adds the difference between the two platform positions to the player position so it can move with the platform
        }

        if (i == 6)
        {
            yPosition += yMovingPlatformDifference;
        }
        bottomReached = true;
        return bottomReached;
    }

    //Hitting bottom of platform
    else if (playerTop <= platformBottom &&
        playerTop >= platformBottom - paddingValue &&
        playerRight > platformLeft &&
        playerLeft < platformRight &&
        yVelocity <= 0)
    {
        yPosition = platformBottom + playerHeight;
        yVelocity = 0.0f;
        bottomReached = true;
        return bottomReached;
    }
    //Hitting left side of platform
    else if (xVelocity > 0 &&
        playerRight > platformLeft &&
        playerLeft < platformLeft &&
        playerTop < platformBottom &&
        playerBottom > platformTop
        )
    {
        xPosition = platformLeft - playerWidth;
        xVelocity = 0;
        bottomReached = false;
        return true;
    }
    //Hitting right side of platform
    else if (xVelocity < 0 &&
        playerRight > platformRight &&
        playerLeft < platformRight &&
        playerTop < platformBottom &&
        playerBottom > platformTop
        )
    {
        xPosition = platformRight;
        xVelocity = 0;
        bottomReached = false;
        return true;
    }

    else
    {
        bottomReached = false;
        return bottomReached;
    }
    i++;
}
