#include "game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Robo Cleaner: Lab Escape"),
      gameState(GameState::MainMenu)
{
    window.setFramerateLimit(60);
    if (!font.openFromFile("attachments/type/menu.ttf"))
    {
        std::cout << "Fail loading font" << std::endl;
    }
    if (!menuBackgroundTexture.loadFromFile("attachments/textures/menu_background.png"))
    {
        std::cout << "Fail loading image" << std::endl;
    }
    playButtonPosition = {70.f, 374.f};
    playButtonSize = {330.f, 58.f};

    exitButtonPosition = {70.f, 462.f};
    exitButtonSize = {330.f, 58.f};
}

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
        if (auto keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (gameState == GameState::MainMenu)
            {
                if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    gameState = GameState::Playing;
                }

                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
            else if (gameState == GameState::Playing)
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    gameState = GameState::MainMenu;
                }
            }
        }
        if (auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                if (gameState == GameState::MainMenu)
                {
                    if (isMouseHere(playButtonPosition, playButtonSize))
                    {
                        gameState = GameState::Playing;
                    }

                    if (isMouseHere(exitButtonPosition, exitButtonSize))
                    {
                        window.close();
                    }
                }
            }
        }
    }
}

void Game::update(float deltaTime)
{
    (void)deltaTime;
}

void Game::render()
{
    window.clear();

    if (gameState == GameState::MainMenu)
    {
        renderMainMenu();
    }
    else if (gameState == GameState::Playing)
    {
        renderPlaying();
    }

    window.display();
}
void Game::renderMainMenu()
{
    sf::Sprite menuBackground(menuBackgroundTexture);

    sf::Vector2u textureSize = menuBackgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

    menuBackground.setScale({scaleX, scaleY});

    window.draw(menuBackground);

    if (isMouseHere(playButtonPosition, playButtonSize))
    {
        sf::RectangleShape hover(playButtonSize);
        hover.setPosition(playButtonPosition);
        hover.setFillColor(sf::Color{255, 255, 255, 0});
        window.draw(hover);
    }

    if (isMouseHere(exitButtonPosition, exitButtonSize))
    {
        sf::RectangleShape hover(exitButtonSize);
        hover.setPosition(exitButtonPosition);
        hover.setFillColor(sf::Color{255, 255, 255, 0});
        window.draw(hover);
    }
}
void Game::renderPlaying()
{}

bool Game::isMouseHere(sf::Vector2f position, sf::Vector2f size) const
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    return mousePosition.x >= position.x &&
           mousePosition.x <= position.x + size.x &&
           mousePosition.y >= position.y &&
           mousePosition.y <= position.y + size.y;
}