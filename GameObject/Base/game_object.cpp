#include "game_object.h"

GameObject::GameObject(sf::Vector2f startPosition)
    : position(startPosition), alive(true)
{
}

GameObject::~GameObject()
{
}

bool GameObject::isAlive() const
{
    return alive;
}

void GameObject::destroy()
{
    alive = false;
}
sf::Vector2f GameObject::getPosition() const
{
    return position;
}