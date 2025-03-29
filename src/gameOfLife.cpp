#include "../include/gameOfLife.hpp"

#include <array>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

// #define DEBUG

using game_of_life::Cell;
using game_of_life::CellList;

namespace game_of_life {

enum class CellStatus : std::uint8_t {
    LIVE = 1,
    DEAD,
    PROCESSED  
};

GameOfLifeModel::GameOfLifeModel() : 
    gameField_(Field(100, Row(100, CellStatus::LIVE)))
{};

GameOfLifeModel::GameOfLifeModel(std::size_t width, std::size_t height) :
    gameField_(Field(height, Row(width, CellStatus::DEAD)))
{
    if (width < 3 || height < 3) {
        throw std::invalid_argument("height < 3 or width < 3");
    }
}

std::size_t GameOfLifeModel::height() const {
    return gameField_.size();
}

std::size_t GameOfLifeModel::width() const {
    return gameField_[0].size();
}

bool GameOfLifeModel::isAlive(std::size_t x, std::size_t y) const {
    return gameField_[y][x] == CellStatus::LIVE;
}

void GameOfLifeModel::kill(std::size_t x, std::size_t y) {
    std::erase_if(
        livingCells_, 
        [x, y] (auto&& cell) { return cell == Cell{x, y}; }
    );
    gameField_[y][x] = CellStatus::DEAD;
}

void GameOfLifeModel::revive(std::size_t x, std::size_t y) {
    if (std::find(livingCells_.begin(), livingCells_.end(), Cell(x, y)) 
            == livingCells_.end()) 
    {
        livingCells_.emplace_back(x, y);
        gameField_[y][x] = CellStatus::LIVE;
    }
}

void GameOfLifeModel::clear() {
    gameField_ = Field(height(), Row(width(), CellStatus::DEAD));
    livingCells_.clear();
}

void GameOfLifeModel::update() {
    Field newGameFild(height(), Row(width(), CellStatus::DEAD));
    for (auto it = livingCells_.begin(); it != livingCells_.end(); ) {
        // просчитать liveStatus для живой занесенной в livingCell клетки
        bool liveStatus = computeLiveStatus_(countNeighbors_(it->first, it->second), true);
        if (liveStatus) {
            newGameFild[it->second][it->first] = CellStatus::LIVE;
            ++it;
        } else {
            newGameFild[it->second][it->first] = CellStatus::PROCESSED;
            it = livingCells_.erase(it);
        }

        // просчитать liveStatus  для соседей livingCell клетки
        for (const auto& n : neighbors) {
            auto [n_x, n_y] 
                = clampToSphere_(n.first + it->first, n.second + it->second);
            // если клетка еще не обработана
            if (newGameFild[n_y][n_x] == CellStatus::DEAD) {
                auto neCount = countNeighbors_(n_x, n_y);
                liveStatus = computeLiveStatus_(neCount, false);
                if (liveStatus) { 
                    newGameFild[n_y][n_x] = CellStatus::LIVE;
                    livingCells_.emplace_back(n_x, n_y);
                } else {
                    newGameFild[n_y][n_x] = CellStatus::PROCESSED;
                }
            }
        }
    }    
    
    gameField_.swap(newGameFild);
}

bool GameOfLifeModel::computeLiveStatus_(std::size_t neighborsCount, bool liveStatus) const {
    return (neighborsCount == 2 && liveStatus) || neighborsCount == 3;
}

const std::array<const std::pair<int, int>, 8> GameOfLifeModel::neighbors {{
    {-1,  -1}, {0, -1}, {1, -1},
    {-1,   0},          {1,  0},
    {-1,   1}, {0,  1}, {1,  1}  
}};

std::size_t GameOfLifeModel::countNeighbors_(std::size_t x, std::size_t y) const {
    auto isAliveWrapper = [this, x, y](auto&& ne) -> bool {
        auto [n_x, n_y] 
            = clampToSphere_(ne.first + x, ne.second + y);
        return isAlive(n_x, n_y);
    };
    auto count = std::count_if(neighbors.begin(), neighbors.end(), isAliveWrapper);
    return count;
}

std::pair<int, int> GameOfLifeModel::clampToSphere_(int x, int y) const {
    int n_x = x;
    int n_y = y;

    if (x < 0) n_x = width() - 1;
    else if (x >= width()) n_x = 0;

    if (y < 0) n_y = height() - 1;
    else if (y >= height()) n_y = 0;

    return {n_x, n_y};
}

} // namespace gamme_of_life
