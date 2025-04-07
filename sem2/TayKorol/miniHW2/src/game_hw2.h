//
// Created by user on 07.04.2025.
//
#pragma once


class game_hw2 {

};


#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <ctime>

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

#define CELLSIZE_M 10
#define CELLSIZE_N 10
#define CHANCE_FOOD 10

#define ENERGY_MAX 100

#define CELLSIZE_SCREEN 100.f

enum CellType {
    Type_Grass,
    Type_Hill,
    Type_Forest,
    Type_Stone,
    Type_Sand,
    Type_Snow,
    Type_Water,
    Type_End
};
enum TextureType {
    Texture_Grass,
    Texture_Hill,
    Texture_Forest,
    Texture_Stone,
    Texture_Sand,
    Texture_Snow,
    Texture_Water,
    Texture_Hide,
    Texture_Soup,
    Texture_End
};

struct Cell {
    bool isHidden;
    bool isFood;
    CellType cellType;
};

bool isWinOfGame(const Cell cells[CELLSIZE_M][CELLSIZE_N]) {
    bool winCondition = true;
    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {
            if (cells[x][y].isHidden) {
                winCondition = false;
                break;
            }
        }
    }
    return winCondition;
}

bool isLoseOfGame(const int &energy) {
    return energy <= 0;
}

void clickTile(int &energy,
               const sf::Vector2i &mouseCoord,
               Cell cells[CELLSIZE_M][CELLSIZE_N],
               sf::RectangleShape shapes[CELLSIZE_M][CELLSIZE_N],
               sf::Texture textures[TextureType::Texture_End]) {

    const int x = mouseCoord.x < 0 ?
                  0 :
                  mouseCoord.x > CELLSIZE_SCREEN * CELLSIZE_M ?
                  CELLSIZE_M :
                  mouseCoord.x / CELLSIZE_SCREEN;

    const int y = mouseCoord.y < 0 ?
                  0 :
                  mouseCoord.y > CELLSIZE_SCREEN * CELLSIZE_N ?
                  CELLSIZE_N :
                  mouseCoord.y / CELLSIZE_SCREEN;

    cells[x][y].isHidden = false;

    if (cells[x][y].isFood) {
        shapes[x][y].setTexture(&textures[TextureType::Texture_Soup]);
        energy = ENERGY_MAX;
    } else
        shapes[x][y].setTexture(&textures[cells[x][y].cellType]);
}

