//
// Created by user on 07.04.2025.
//

#include "Game.h"


Game::Game(const std::map<Textures::ID, std::string> &resourcePaths, int width, int length) :
        width(width), length(length),
        window(
                sf::VideoMode({static_cast <unsigned int> (CELLSIZE_SCREEN * CELLSIZE_M),
                               static_cast <unsigned int> (CELLSIZE_SCREEN * CELLSIZE_N)}),
                "NOT MINESWEEPER GAME", sf::State::Windowed),
        textures(resourcePaths) {

    energy = ENERGY_MAX;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, 100);

    for (int x = 0; x < cells.size(); x++) {
        for (int y = 0; y < cells[x].size(); y++) {
            cells[x][y].isHidden = true;
            cells[x][y].isFood = dist(gen) < CHANCE_FOOD;
            cells[x][y].cellType = Textures::getTileTexture(gen);
        }
    }


}

void Game::run() {

    sf::Vector2i mouseCoord;

    sf::Font font("assets/arial.ttf");
    sf::Text textEnergy(font);
    sf::Text textCondition(font);
    textEnergy.setCharacterSize(CELLSIZE_SCREEN / 2);
    textEnergy.setFillColor(sf::Color::Red);
    textEnergy.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textCondition.setCharacterSize(CELLSIZE_SCREEN);
    textCondition.setFillColor(sf::Color::Red);
    textCondition.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::RectangleShape shapes[CELLSIZE_M][CELLSIZE_N];


    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {

            // TODO функцию а не напрямую
            if (cells[x][y].isHidden)
                shapes[x][y].setTexture(&textures.get(Textures::ID::Texture_Hide));
            else
                shapes[x][y].setTexture(&textures.get(cells[x][y].cellType));

            shapes[x][y].setPosition(
                    sf::Vector2f(x * CELLSIZE_SCREEN, y * CELLSIZE_SCREEN));
            shapes[x][y].setSize({CELLSIZE_SCREEN, CELLSIZE_SCREEN});
        }
    }

    bool mousepressed = false;
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                mousepressed = false;
            }

            if (!mousepressed && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                mousepressed = true;
                mouseCoord = sf::Mouse::getPosition(window);

                clickTile(mouseCoord, shapes);
                energy--;
                textEnergy.setString(std::to_wstring(energy));

                if (isWinOfGame())
                    textCondition.setString("WINNER!!!");
                if (isLoseOfGame())
                    textCondition.setString("LOSER!!!");
            }
        }

        window.clear();
        for (int x = 0; x < CELLSIZE_M; x++) {
            for (int y = 0; y < CELLSIZE_N; y++) {
                window.draw(shapes[x][y]);
            }
        }
        window.draw(textEnergy);
        window.draw(textCondition);
        window.display();
    }
}
