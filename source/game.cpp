#include "game.h"

Game::Game()
    : window(sf::VideoMode({1200, 800}), "Robo Cleaner: Lab Escape"){}

void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        eventsHandeling();
        update(deltaTime);
        render();
    }
}

void Game::eventsHandeling()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }
}

void Game::update(float deltaTime)
{
    (void)deltaTime;
}

void Game::render()
{
    window.clear(sf::Color{30, 30, 30});
    window.display();
}