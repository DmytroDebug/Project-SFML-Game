#include "game.h"
#include "player_.h"

#include <iostream>
#include <memory>

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Robo Cleaner: Lab Escape"),
      gameState(GameState::MainMenu)
{
    window.setFramerateLimit(60);

    if (!menuBackgroundTexture.loadFromFile("attachments/textures/menu_background.png"))
    {
        std::cout << "Fail loading image" << std::endl;
    }
    if (!font.openFromFile("attachments/type/menu.ttf"))
    {
        std::cout << "Fail loading font" << std::endl;
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
                    startGame();
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
}

void Game::update(float deltaTime)
{
    if (gameState == GameState::Playing)
    {
        for (auto &object : objects)
        {
            object->update(deltaTime);
        }
    }
}

void Game::render()
{
    window.clear(sf::Color{20, 20, 25});

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
    sf::RectangleShape background({1280.f, 720.f});
    background.setFillColor(sf::Color{22, 25, 30});
    background.setPosition({0.f, 0.f});
    window.draw(background);

    sf::RectangleShape floorLine({1280.f, 20.f});
    floorLine.setPosition({0.f, 670.f});
    floorLine.setFillColor(sf::Color{70, 80, 90});
    window.draw(floorLine);

    sf::RectangleShape UpLine({1280.f, 20.f});
    UpLine.setPosition({0.f, 45.f});
    UpLine.setFillColor(sf::Color{70, 80, 90});
    window.draw(UpLine);

    for (auto &object : objects)
    {
        object->draw(window);
    }

    sf::Text text(font, "WASD / Arrows - move      Mouse - Aim            Shoot - LMB             ESC - menu", 22);
    text.setFillColor(sf::Color{180, 185, 195});
    text.setPosition({30.f, 680.f});
    window.draw(text);
}

bool Game::isMouseHere(sf::Vector2f position, sf::Vector2f size) const
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    return mousePosition.x >= position.x &&
           mousePosition.x <= position.x + size.x &&
           mousePosition.y >= position.y &&
           mousePosition.y <= position.y + size.y;
}
void Game::startGame()
{
    objects.clear();

    objects.push_back(
        std::make_unique<PlayerRobot>(
            sf::Vector2f{640.f, 360.f},
            280.f,
            sf::Vector2f{1280.f, 720.f}));

    gameState = GameState::Playing;
}
