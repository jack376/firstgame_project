#include "stdafx.h"
#include "Collision.h"

bool Collision::CheckCollision(Collision& other)
{
    sf::Vector2f thisPosition = collisionShape.getPosition();
    sf::Vector2f thisHalfSize = collisionShape.getSize() / 2.0f;
    sf::Vector2f otherPosition = other.collisionShape.getPosition();
    sf::Vector2f otherHalfSize = other.collisionShape.getSize() / 2.0f;

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.0f && intersectY < 0.0f)
    {
        return true;
    }
    return false;
}