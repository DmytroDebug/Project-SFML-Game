#pragma once
#include <SFML/Graphics.hpp>

class Game
{
public:
    Game();
    void run();
private:
    void eventsHandeling();
    void update(float deltaTime);
    void render();
    sf::RenderWindow window;
    sf::Clock clock;
};