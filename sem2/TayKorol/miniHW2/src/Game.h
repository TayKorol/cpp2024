//
// Created by user on 07.04.2025.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <ctime>
#include "resource_management/ResourceManagement.h"

// (V) 1. M x N двумерное поле с тайлами(в примере 10 x 10)
// (V) 2. Скрытые тайлы -> открытые с эвентами
// (X) 3. эвенты должны отображаться, + Логика
// (X) 4. Под каждым тайлом с вероятность в 10% есть консервы,
// при вскрытии тайла -> восст. сытость
// (V) 5. Открыть M x N тайлов (все тайлы) для победы
// (V) 6. Экран победы + экран поражения
// (V) 7. За каждое открытие тайла теряем 1 сытость. Сытость равно 0,
// тогда поражение, + отображать сытость
// (V) 8. Начальное значени сытости - 25

#define CHANCE_FOOD 10
#define CHANCE_ENEMY 10
#define HP_START 10
#define ENERGY_START 40

#define CELLSIZE_SCREEN 100.f


class Game {

public:
    Game(const std::map<Textures::ID, std::string> &resourcePaths,
         const std::map<Fonts::ID, std::string> &fontPaths, int width, int length);

    void run();

private:

    struct Cell {
        bool isHidden;
        bool isFood;
        bool hasEnemy = false;
        Textures::ID cellType;
    };

private:


    size_t width, length;
    sf::RenderWindow window;
    int energy = ENERGY_START;
    int hp = HP_START;
    std::vector<std::vector<Cell>> cells{width, std::vector<Cell>(length)};
    std::vector<std::vector<sf::RectangleShape>> tiles{width, std::vector<sf::RectangleShape>(length)};

    sf::Font arial_font;
    ResourceHolder<sf::Texture, Textures::ID> textures;
    ResourceHolder<sf::Font, Fonts::ID> fonts;
    sf::Text textEnergy;
    sf::Text textCondition;
    sf::Text textHP;

    bool isWon() const;

    bool isLost() const;

    void clickTile(const sf::Vector2i &mouseCoord);

    void set_text_style();

    void generate_cells(std::mt19937 &gen);

    void make_shapes();

    void update_txt();
};

