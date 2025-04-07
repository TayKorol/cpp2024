#include "src/game_hw2.h"


int main() {
    int energy = ENERGY_MAX;

    srand(time(0));

    Cell cells[CELLSIZE_M][CELLSIZE_N];
    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {
            cells[x][y].isHidden = true;
            cells[x][y].isFood = rand() % 100 < CHANCE_FOOD;

            cells[x][y].cellType = static_cast<CellType>
            (rand() % (CellType::Type_End - 1));
        }
    }

    sf::RenderWindow window(
            sf::VideoMode({static_cast <unsigned int> (CELLSIZE_SCREEN * CELLSIZE_M),
                           static_cast <unsigned int> (CELLSIZE_SCREEN * CELLSIZE_N)}),
            "NOT MINESWEEPER GAME", sf::State::Windowed);
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
    sf::Texture textures[TextureType::Texture_End];
    textures[TextureType::Texture_Grass] = sf::Texture("assets/grass.png");
    textures[TextureType::Texture_Hill] = sf::Texture("assets/hill.png");
    textures[TextureType::Texture_Forest] = sf::Texture("assets/architecture.png");
    textures[TextureType::Texture_Stone] = sf::Texture("assets/stone.png");
    textures[TextureType::Texture_Sand] = sf::Texture("assets/sand.png");
    textures[TextureType::Texture_Snow] = sf::Texture("assets/snow.png");
    textures[TextureType::Texture_Water] = sf::Texture("assets/water.png");
    textures[TextureType::Texture_Hide] = sf::Texture("assets/hide.png");
    textures[TextureType::Texture_Soup] = sf::Texture("assets/soup.png");


    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {

            // TODO функцию а не напрямую
            if (cells[x][y].isHidden)
                shapes[x][y].setTexture(&textures[TextureType::Texture_Hide]);
            else
                shapes[x][y].setTexture(&textures[cells[x][y].cellType]);

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

                clickTile(energy, mouseCoord, cells, shapes, textures);
                energy--;
                textEnergy.setString(std::to_wstring(energy));

                if (isWinOfGame(cells))
                    textCondition.setString("WINNER!!!");
                if (isLoseOfGame(energy))
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