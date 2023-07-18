#pragma once
class Collision
{
private:
    sf::RectangleShape& collisionShape;

public:
    Collision(sf::RectangleShape& collisionShape) : collisionShape(collisionShape) {}
    bool CheckCollision(Collision& other);
};