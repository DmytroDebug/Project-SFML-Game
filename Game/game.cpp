#include "game.h"
#include "../GameObject/Player/player_.h"

#include <iostream>
#include <memory>

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Robo Cleaner: Lab Escape"),
      gameState(GameState::MainMenu)
{
    window.setFramerateLimit(60);

    if (!menuBackgroundTexture.loadFromFile("Attachments/textures/menu_background.png"))
    {
        std::cout << "Failed to load menu background" << std::endl;
    }

    if (!labBackgroundTexture.loadFromFile("Attachments/textures/lab_background.png"))
    {
        std::cout << "Failed to load lab background" << std::endl;
    }

    playButtonPosition = {60.f, 377.f};
    playButtonSize = {350.f, 50.f};

    exitButtonPosition = {60.f, 465.f};
    exitButtonSize = {350.f, 50.f};
}

void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (auto key = event->getIf<sf::Event::KeyPressed>())
        {
            if (gameState == GameState::MainMenu)
            {
                if (key->code == sf::Keyboard::Key::Enter)
                {
                    startGame();
                }

                if (key->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }

            if (gameState == GameState::Playing)
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    gameState = GameState::MainMenu;
                }
            }
        }

        if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left &&
                gameState == GameState::MainMenu)
            {
                if (isMouseHere(playButtonPosition, playButtonSize))
                {
                    startGame();
                }

                if (isMouseHere(exitButtonPosition, exitButtonSize))
                {
                    window.close();
                }
            }
        }
    }
}

void Game::update(float deltaTime)
{
    if (gameState != GameState::Playing)
    {
        return;
    }

    for (auto& object : objects)
    {
        object->update(deltaTime);
    }
}

void Game::render()
{
    window.clear();

    if (gameState == GameState::MainMenu)
    {
        renderMainMenu();
    }

    if (gameState == GameState::Playing)
    {
        renderPlaying();
    }

    window.display();
}

void Game::renderMainMenu()
{
    drawBackground(menuBackgroundTexture);

    if (isMouseHere(playButtonPosition, playButtonSize))
    {
        sf::RectangleShape hover(playButtonSize);
        hover.setPosition(playButtonPosition);
        hover.setFillColor(sf::Color{255, 255, 255, 20});
        window.draw(hover);
    }

    if (isMouseHere(exitButtonPosition, exitButtonSize))
    {
        sf::RectangleShape hover(exitButtonSize);
        hover.setPosition(exitButtonPosition);
        hover.setFillColor(sf::Color{255, 255, 255, 20});
        window.draw(hover);
    }
}

void Game::renderPlaying()
{
    drawBackground(labBackgroundTexture);

    for (auto& object : objects)
    {
        object->draw(window);
    }
}

void Game::drawBackground(const sf::Texture& texture)
{
    sf::Sprite background(texture);

    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

    background.setScale({scaleX, scaleY});
    window.draw(background);
}

void Game::startGame()
{
    objects.clear();

    objects.push_back(
        std::make_unique<PlayerRobot>(
            sf::Vector2f{640.f, 360.f},
            230.f,
            sf::Vector2f{1280.f, 720.f}
        )
    );

    gameState = GameState::Playing;
}

bool Game::isMouseHere(sf::Vector2f position, sf::Vector2f size) const
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    return mousePosition.x >= position.x &&
           mousePosition.x <= position.x + size.x &&
           mousePosition.y >= position.y &&
           mousePosition.y <= position.y + size.y;
}
