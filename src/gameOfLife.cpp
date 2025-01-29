#include "../include/gameOfLife.hpp"

#include <array>
#include <cstddef>
#include <functional>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

// #define DEBUG

#include <QPixMap>

using game_of_life::Cell;
using game_of_life::CellList;

namespace game_of_life {

enum class CellStatus : std::uint8_t {
    LIVE = 1,
    DEAD,
    PROCESSED  
};

GameOfLifeModel::GameOfLifeModel() : 
    gameField(Field(100, Row(100, CellStatus::LIVE)))
{};

GameOfLifeModel::GameOfLifeModel(std::size_t width, std::size_t height) :
    gameField(Field(height, Row(width, CellStatus::DEAD)))
{
    if (width < 3 || height < 3) {
        throw std::invalid_argument("height < 3 or width < 3");
    }
}

GameOfLifeModel::GameOfLifeModel(const GameOfLifeModel& other) : 
    gameField(other.gameField) 
{}

GameOfLifeModel::GameOfLifeModel(GameOfLifeModel&& other) noexcept :
    gameField(std::move(other.gameField)) 
{}

GameOfLifeModel& GameOfLifeModel::operator=(const GameOfLifeModel& other) {
    if (this != &other) {
        gameField = other.gameField;
    }
    return *this;
}

GameOfLifeModel& GameOfLifeModel::operator=(GameOfLifeModel&& other) noexcept {
    gameField = std::move(other.gameField);
    return *this;
}

std::size_t GameOfLifeModel::height() const {
    return gameField.size();
}

std::size_t GameOfLifeModel::width() const {
    return gameField[0].size();
}

bool GameOfLifeModel::isAlive(std::size_t x, std::size_t y) const {
    return gameField[y][x] == CellStatus::LIVE;
}

void GameOfLifeModel::kill(std::size_t x, std::size_t y) {
    std::erase_if(
        livingCells, 
        [x, y] (auto cell) { return cell == Cell{x, y}; }
    );
    gameField[y][x] = CellStatus::DEAD;

}

void GameOfLifeModel::revive(std::size_t x, std::size_t y) {
    if (std::find(livingCells.begin(), livingCells.end(), Cell(x, y)) 
            == livingCells.end()) 
    {
        livingCells.emplace_back(x, y);
        gameField[y][x] = CellStatus::LIVE;
    }
}

void GameOfLifeModel::clear() {
    gameField = Field(height(), Row(width(), CellStatus::DEAD));
    livingCells.clear();
}


void GameOfLifeModel::update() {
    Field newGameFild(height(), Row(width(), CellStatus::DEAD));
    int n_x;
    int n_y;
    bool liveStatus; 
    Cell cell;
    for (auto it = livingCells.begin(); it != livingCells.end(); ) {
        cell = *it;
        // просчитать liveStatus для живой занесенной в livingCell клетки
        liveStatus = computeLiveStatus(countNeighbors(cell.first, cell.second), true);
        if (liveStatus) {
            newGameFild[cell.second][cell.first] = CellStatus::LIVE;
            ++it;
        } else {
            newGameFild[cell.second][cell.first] = CellStatus::PROCESSED;
            it = livingCells.erase(it);
        }

        // просчитать liveStatus  для соседей livingCell клетки
        for (const auto& n : neighbors) {
            n_x = n.first + cell.first;
            n_y = n.second + cell.second;

            if (n_x < 0) {
                n_x = gameField[0].size() - 1;
            } else if (n_x >= gameField[0].size()) {
                n_x = 0;
            }
            if (n_y < 0) {
                n_y = gameField.size() - 1;
            } else if (n_y >= gameField.size()) {
                n_y = 0;
            }

            // если клетка еще не обработана
            if (newGameFild[n_y][n_x] == CellStatus::DEAD) {
                liveStatus = computeLiveStatus(countNeighbors(n_x, n_y), false);
                if (liveStatus) { 
                    newGameFild[n_y][n_x] = CellStatus::LIVE;
                    livingCells.push_front(Cell(n_x, n_y));
                } else {
                    newGameFild[n_y][n_x] = CellStatus::PROCESSED;
                }
            }
        }
    }    
    gameField = std::move(newGameFild);
}

bool GameOfLifeModel::computeLiveStatus(std::size_t neighborsCount, bool liveStatus) {
    return (neighborsCount == 2 && liveStatus) || neighborsCount == 3;
}

const std::array<const std::pair<int, int>, 8> GameOfLifeModel::neighbors {{
    {-1,  -1}, {0, -1}, {1, -1},
    {-1,   0},          {1,  0},
    {-1,   1}, {0,  1}, {1,  1}  
}};


std::size_t GameOfLifeModel::countNeighbors(std::size_t x, std::size_t y) const {
    std::size_t count = 0;
    int n_x;
    int n_y;
    for (const auto& n : neighbors) {
        n_x = n.first + x;
        n_y = n.second + y;

        if (n_x < 0) {
            n_x = gameField[0].size() - 1;
        } else if (n_x >= gameField[0].size()) {
            n_x = 0;
        }

        if (n_y < 0) {
            n_y = gameField.size() - 1;
        } else if (n_y >= gameField.size()) {
            n_y = 0;
        }
    
        count += gameField[n_y][n_x] == CellStatus::LIVE ? 1 : 0;
    }
    return count;
}


} // namespace gamme_of_life
