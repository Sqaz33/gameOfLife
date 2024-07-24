#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <array>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <functional>

#include <QPixmap>

namespace game_of_life {
    using Row = std::vector<int>;
    using Field = std::vector<Row>;
class GameOfLife {
public:
    GameOfLife(size_t width, size_t height) :
        gameField(Field(height, Row(width, 0)))
    {
        if (width < 3 || height < 3) {
            throw std::invalid_argument("high < 3 or width < 3");
        }
    }

    GameOfLife(const GameOfLife& other) noexcept :
        gameField(other.gameField) 
    {}

    GameOfLife(GameOfLife&& other) noexcept :
        gameField(std::move(other.gameField))
    {}

    GameOfLife& operator=(const GameOfLife& other) noexcept {
        gameField = other.gameField;
        return *this;
    }

    GameOfLife& operator=(GameOfLife&& other) noexcept{
        gameField = std::move(other.gameField);
        return *this;
    }


    inline size_t height() const;
    inline size_t width() const;

    inline bool isAlive(size_t x, size_t y) const; 
    void kill(size_t x, size_t y);
    void revive(size_t x, size_t y);
    void clearField();

    static bool computeLiveStatus(size_t neighborsCount, bool liveStatus) {
        return (!liveStatus && neighborsCount == 3) || //порождение
                ((neighborsCount == 2 || neighborsCount == 3) && liveStatus); // продолжение жизни
    }           // иначе наступает/продолжается смерть

    void renderNextStage();

private:
    Field gameField;

    inline int countNeighbors(size_t x, size_t y) const;
};


class GameOfLifePainter {
public:
    static QPixmap paintGameOFLifeFieldOnQPixMap(const GameOfLife& game, size_t squareSideLen);
};


}


#endif