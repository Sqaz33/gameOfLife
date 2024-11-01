#include "include/gameOfLife.h"

#include <array>
#include <algorithm>
#include <stdexcept>
#include <list>
#include <utility>
#include <iostream>

// #define DEBUG

#include <QPixMap>
#include <QPainter>
#include <QBrush>

using game_of_life::GameOfLife;
using game_of_life::GameOfLifePainter;
using game_of_life::Cell;
using game_of_life::CellList;

namespace {
    void drawFieldSquares(QPixmap& field, size_t sideLen, Qt::GlobalColor color) {
        QPainter painter(&field);
        painter.setPen(color);
        for (size_t x = 0; x < field.width(); x+=sideLen) {
            painter.drawLine(x, 0, x, field.width() - 1);
        }
        for (size_t y = 0; y < field.height(); y+=sideLen) {
            painter.drawLine(0, y, field.width() - 1, y);
        }
        painter.drawLine(0, field.height() - 1, field.width() - 1, field.height() - 1);
        painter.drawLine(field.width() - 1, 0, field.width() - 1, field.height() - 1);
    }


    CellList::iterator getIterOnCellFromLivingCells(CellList& cells, size_t x, size_t y) {
        return std::find(
            cells.begin(), cells.end(), Cell(x, y)
        );
    }
}

size_t GameOfLife::height() const {
    return gameField.size();
}

size_t GameOfLife::width() const {
    return gameField[0].size();
}

bool GameOfLife::isAlive(size_t x, size_t y) const {
    return gameField[y][x] == 1;
}

void GameOfLife::kill(size_t x, size_t y) {
    std::remove(livingCells.begin(), livingCells.end(), Cell(x, y));
    gameField[y][x] = 0;

}

void GameOfLife::revive(size_t x, size_t y) {
    if (std::find(livingCells.begin(), livingCells.end(), Cell(x, y)) 
            == livingCells.end()) 
    {
        livingCells.push_front(Cell(x, y));
        gameField[y][x] = 1;
    }
}

void GameOfLife::clearField() {
    gameField = Field(height(), Row(width(), 0));
    livingCells = std::list<Cell>();
}


void GameOfLife::renderNextGameFieldState() {
    Field newGameFild(height(), Row(width(), 0));
    int n_x;
    int n_y;
    bool liveStatus; 
    Cell cell;
    for (auto it = livingCells.begin(); it != livingCells.end(); ) {
        cell = *it;
        // просчитать liveStatus для живой занесенной в livingCell клетки
        liveStatus = computeLiveStatus(countNeighbors(cell.first, cell.second), true);
        if (liveStatus) {
            newGameFild[cell.second][cell.first] = 1;
            ++it;
        } else {
            newGameFild[cell.second][cell.first] = 2;
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
            if (newGameFild[n_y][n_x] == 0) {
                liveStatus = computeLiveStatus(countNeighbors(n_x, n_y), false);
                if (liveStatus) { 
                    newGameFild[n_y][n_x] = 1;
                    livingCells.push_front(Cell(n_x, n_y));
                } else {
                    newGameFild[n_y][n_x] = 2;
                }
            }
        }
    }    
    gameField = std::move(newGameFild);
}

bool GameOfLife::computeLiveStatus(size_t neighborsCount, bool liveStatus) {
    return (neighborsCount == 2 && liveStatus) || neighborsCount == 3;

}

const std::array<const std::pair<int, int>, 8> GameOfLife::neighbors {{
    {-1,  -1}, {0, -1}, {1, -1},
    {-1,   0},          {1,  0},
    {-1,   1}, {0,  1}, {1,  1}  
}};


size_t GameOfLife::countNeighbors(size_t x, size_t y) const {
    size_t count = 0;
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
    
        count += gameField[n_y][n_x] == 1 ? 1 : 0;
    }
    return count;
}


struct FieldKey {
    size_t w, h, s;
    bool operator==(const FieldKey& other) const {
        return w == other.w && h == other.h && s == other.s;
    }
};

namespace std {
    template<>
    struct hash<FieldKey> {
        size_t operator()(const FieldKey& key) const {
            std::hash<size_t> h;
            return h(key.w) ^ h(key.h << 1) ^ h(key.s << 2);
        }
    };
}

QPixmap GameOfLifePainter::paintGameOfLifeFieldOnQPixMap(const GameOfLife& game, size_t squareSideLen) {
    static std::unordered_map<FieldKey, QPixmap> markedQPixmapFields; 
    FieldKey size = {game.height(), game.width(), squareSideLen};
    if (markedQPixmapFields.find(size) == markedQPixmapFields.end()) {
        QPixmap field(game.width() * squareSideLen, game.height() * squareSideLen);
        field.fill(Qt::white);
        drawFieldSquares(field, squareSideLen, Qt::red);
        markedQPixmapFields[size] = field;
    }

    QPixmap resField(markedQPixmapFields[size]);
    QPainter painter(&resField);

    for (size_t y = 0; y < game.height(); ++y) {
        for (size_t x = 0; x < game.width(); ++x) {
            if (game.isAlive(x, y)) {
                painter.fillRect(
                    x * squareSideLen,
                    y * squareSideLen,
                    squareSideLen,
                    squareSideLen,
                    Qt::black
                );
            }
        }
    }
    return resField;
}