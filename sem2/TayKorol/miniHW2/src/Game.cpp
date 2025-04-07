//
// Created by user on 07.04.2025.
//

#include "Game.h"


Game::Game(const std::map<Textures::ID, std::string> &resourcePaths,
           const std::map<Fonts::ID, std::string> &fontPaths, int width, int length) :
        width(width), length(length),
        window(
                sf::VideoMode({static_cast <unsigned int> (CELLSIZE_SCREEN * width),
                               static_cast <unsigned int> (CELLSIZE_SCREEN * length)}),
                "NOT MINESWEEPER GAME", sf::State::Windowed),
        textures(resourcePaths),
        fonts(fontPaths),
        textEnergy(fonts.get(Fonts::ID::Arial)),
        textCondition(fonts.get(Fonts::ID::Arial)),
        textHP(fonts.get(Fonts::ID::Arial)) {

    energy = ENERGY_START;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    generate_cells(gen);
    make_shapes();
    set_text_style();
}

void Game::make_shapes() {
    for (int x = 0; x < cells.size(); x++) {
        for (int y = 0; y < cells[x].size(); y++) {
            if (cells[x][y].isHidden)
                tiles[x][y].setTexture(&textures.get(Textures::ID::Texture_Hide));
            else {
                tiles[x][y].setTexture(&textures.get(cells[x][y].cellType));
            }


            tiles[x][y].setPosition(
                    sf::Vector2f(x * CELLSIZE_SCREEN, y * CELLSIZE_SCREEN));
            tiles[x][y].setSize({CELLSIZE_SCREEN, CELLSIZE_SCREEN});
        }
    }
}

void Game::generate_cells(std::mt19937 &gen) {
    std::uniform_int_distribution<size_t> dist(0, 100);
    for (auto &cell: cells) {
        for (auto &y: cell) {
            y.isHidden = true;
            y.isFood = dist(gen) < CHANCE_FOOD;
            if (!y.isFood) { // не совсем правильные шансы ну и лан
                y.hasEnemy = dist(gen) < CHANCE_ENEMY;
            }
            y.cellType = Textures::getTileTexture(gen);
        }
    }
}

void Game::run() {

    sf::Vector2i mouseCoord;

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
            if (event->getIf<sf::Event::MouseButtonPressed>()) {
                mouseCoord = sf::Mouse::getPosition(window);
                clickTile(mouseCoord);
                update_txt();
                if (isWon())
                    textCondition.setString("WINNER!!!");
                if (isLost())
                    textCondition.setString("LOSER!!!");
            }
        }

        window.clear();
        for (int x = 0; x < cells.size(); x++) {
            for (int y = 0; y < cells[x].size(); y++) {
                window.draw(tiles[x][y]);
            }
        }
        window.draw(textEnergy);
        window.draw(textCondition);
        window.draw(textHP);
        window.display();
    }
}

void Game::set_text_style() {
    textEnergy.setCharacterSize(CELLSIZE_SCREEN / 2);
    textEnergy.setFillColor(sf::Color::Red);
    textEnergy.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textCondition.setCharacterSize(CELLSIZE_SCREEN);
    textCondition.setFillColor(sf::Color::Red);
    textCondition.setStyle(sf::Text::Bold | sf::Text::Underlined);
    update_txt();
}

void Game::update_txt() {
    textHP.setString(L"HP: " + std::to_wstring(hp));
    textEnergy.setString(L"ENG: " + std::to_wstring(energy));

    sf::FloatRect textBounds = textHP.getLocalBounds();
    float padding = 50.f;
    textHP.setPosition(
            {window.getSize().x - padding - textBounds.size.x,
             padding}
    );
    textHP.setCharacterSize(CELLSIZE_SCREEN / 2);
    textHP.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textEnergy.setPosition(
            {padding,
             padding}
    );
    textCondition.setPosition(
            {(window.getSize().x - padding) / 2,
             (window.getSize().y - padding) /2});
}

void Game::clickTile(const sf::Vector2i &mouseCoord) {
    const int x = mouseCoord.x < 0 ?
                  0 :
                  mouseCoord.x > CELLSIZE_SCREEN * width ?
                  -1 :
                  mouseCoord.x / CELLSIZE_SCREEN;

    const int y = mouseCoord.y < 0 ?
                  0 :
                  mouseCoord.y > CELLSIZE_SCREEN * length ?
                  -1 :
                  mouseCoord.y / CELLSIZE_SCREEN;

    if ((x == -1 || y == -1) || !cells[x][y].isHidden) {
        return;
    }
    energy--;
    cells[x][y].isHidden = false;

    if (cells[x][y].isFood) {
        tiles[x][y].setTexture(&textures.get(Textures::ID::Texture_Soup));
        energy++;
    } else if (cells[x][y].hasEnemy) {
        tiles[x][y].setTexture(&textures.get(Textures::ID::Texture_EvilGuy));
        hp--;
    } else
        tiles[x][y].setTexture(&textures.get(cells[x][y].cellType));
}

bool Game::isWon() const {
    for (auto &cell: cells) {
        for (auto &y: cell) {
            if (y.isHidden) {
                return false;
            }
        }
    }
    return true;
}

bool Game::isLost() const {
    return energy <= 0 || hp <= 0;
}
