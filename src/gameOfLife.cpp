#include "include/gameOfLife.h"

#include <array>
#include <stdexcept>
#include <utility>

#include <QPixMap>
#include <QPainter>
#include <QBrush>

using game_of_life::GameOfLife;
using game_of_life::GameOfLifePainter;

size_t GameOfLife::height() const {
    return gameField.size();
}

size_t GameOfLife::width() const {
    return gameField[0].size();
}

bool GameOfLife::isAlive(size_t x, size_t y) const {
    return gameField[y][x];
}

void GameOfLife::kill(size_t x, size_t y) {
    gameField[y][x] = false;
}

void GameOfLife::revive(size_t x, size_t y) {
    gameField[y][x] = true;
}

void GameOfLife::clearField() {
    gameField = Field(height(), Row(width(), 0));
}

void GameOfLife::renderNextGameFieldState() {
    Field newGameFild(height(), Row(width(), 0));
    for (size_t y = 0; y < gameField.size(); ++y) {
        for (size_t x = 0; x < gameField[0].size(); ++x) {
            newGameFild[y][x] = computeLiveStatus(countNeighbors(x, y), gameField[y][x]);
        }
    }
    gameField = std::move(newGameFild);
}

bool GameOfLife::computeLiveStatus(size_t neighborsCount, bool liveStatus) {
    return (!liveStatus && neighborsCount == 3) || // Revive
            ((neighborsCount == 2 || neighborsCount == 3) && liveStatus); // Live continue 
}           // Kill/ Deth continue



int GameOfLife::countNeighbors(size_t x, size_t y) const {
    static const std::array<std::pair<int, int>, 8> neighbors {{
        {-1,  -1}, {0, -1}, {1, -1},
        {-1,   0},          {1,  0},
        {-1,   1}, {0,  1}, {1,  1}  
    }};

    int count = 0;
    int n_x;
    int n_y;
    for (auto& n : neighbors) {
        n_x = n.first + x;
        n_y = n.second + y;

        if (n_y < gameField.size()  &&
            n_x < gameField[0].size() && 
            n_y >= 0 &&
            n_x >= 0) 
        {
            count += gameField[n_y][n_x] ? 1 : 0;
        }
    }
    return count;
}

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