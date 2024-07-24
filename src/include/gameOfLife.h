#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>
#include <stdexcept>

//TODO: задокументировать

namespace game {

class GameOfLife {
    using Row = std::vector<int>;
    using Field = std::vector<Row>;

    GameOfLife(size_t width, size_t height) :
        gameField(Field(height, Row(width, 0)))
    {
        if (width < 3 || height < 3) {
            throw std::invalid_argument("high < 8 or width < 8");
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

    void setLiveStatus(size_t x, size_t y, bool status) inline;
    void renderNextStage();
    inline bool isAlive(size_t x, size_t y) const; 

private:
    Field gameField;

    inline int countNeighbors(size_t x, size_t y) const;
};


}


#endif