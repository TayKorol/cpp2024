
#include "src/Game.h"
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


int main() {
    const std::map<Textures::ID, std::string> resourcePaths{
            {Textures::ID::Texture_Grass, "assets/grass.png"},
            {Textures::ID::Texture_Hide, "assets/hide.png"},
            {Textures::ID::Texture_Soup, "assets/soup.png"},
            {Textures::ID::Texture_Forest, "assets/forest.png"},
            {Textures::ID::Texture_Hill, "assets/hill.png"},
            {Textures::ID::Texture_Sand, "assets/sand.png"},
            {Textures::ID::Texture_Snow, "assets/snow.png"},
            {Textures::ID::Texture_Stone, "assets/stone.png"},
            {Textures::ID::Texture_Water, "assets/water.png"},
    };

    Game game(resourcePaths, CELLSIZE_M, CELLSIZE_N);
    game.run();
}