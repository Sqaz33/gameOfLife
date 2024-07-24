#include "include/gameOfLife.h"

#include <array>
#include <stdexcept>
#include <utility>

using game::GameOfLife;


void GameOfLife::setLiveStatus(size_t x, size_t y, bool status) {
    if (x >= gameField.size() || y >= gameField[0].size()) {
        throw std::out_of_range("x >= width or y >= high");
    }
    gameField[y][x] = status;
}

const GameOfLife::Field& GameOfLife::renderNextStage()  {
    bool status;
    int countN;
    for (size_t y = 0; y < gameField.size(); ++y) {
        for (size_t x = 0; x < gameField[0].size(); ++x) {
            countN = countNeighbors(x, y);
            status = countN == 2 || countN == 3;
            gameField[x][y] = status;
        }
    }
    return gameField;
}

bool GameOfLife::isAlive(size_t x, size_t y) const noexcept{
    return gameField[y][x];
}

int GameOfLife::countNeighbors(size_t x, size_t y) const {
    static const std::array<std::pair<int, int>, 8> neighbors {{
        {-1, -1}, {-1, 0}, {-1, 1},
        {0,  -1},          {0,  1},
        {1,   1}, {1,  0}, {1,  1}  
    }};

    int count = 0;
    int n_x;
    int n_y;
    for (auto& n : neighbors) {
        n_x = n.first + x;
        n_y = n.second + y;

        if (n_y < gameField.size() && n_x < gameField[0].size()) {
            count += gameField[n_y][n_x] ? 1 : 0;
        }
    }
    return count;
}
