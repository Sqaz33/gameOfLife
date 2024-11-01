/**
 * @file gameOfLife.h
 * @author Matveev Stepan (mset321@gmail.com).
 * @brief Header file containing the GameOfLife and GameOfLifePainter classes.
 * @version 1.0
 * @date 2024-07-25
 */


#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <array>
#include <functional>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#include <QPixmap>


namespace game_of_life {
using Row = typename  std::vector<int>; ///< Designation of a row of the game field.
using Field = typename std::vector<Row>; ///< Designation of the game field.
using Cell = std::pair<size_t, size_t>;///< Designation of cell
using CellList = std::list<Cell>; ///< Designation of list of cells

/**
 * @class GameOfLife
 * @brief Class for creating and processing a single session of the cellular automaton "Game of Life".
 * 
 * @note 1. In the methods of the class, cells are universally referred to as point (x, y).
 *          Note that the origin is considered the point (0, 0), located in the upper left
 *          corner of the game field.
 * 
 * @note 2. The field has clear vertical boundaries: the upper and lower boundaries are not connected in pairs.
 */
class GameOfLife {
public:
    /**
     * @brief Construct a new Game Of Life object
     */
    GameOfLife() : 
        gameField(Field(100, Row(100, 0)))
    {};

    /**
     * @brief Construct a new Game Of Life.
     * 
     * @param[in] width Width of the game field.
     * @param[in] height Height of the game field.
     * 
     * @pre width >= 3 and height >= 3.
     * 
     * @throw std::invalid_argument("height < 3 or width < 3")
     */
    GameOfLife(size_t width, size_t height) :
        gameField(Field(height, Row(width, 0)))
    {
        if (width < 3 || height < 3) {
            throw std::invalid_argument("height < 3 or width < 3");
        }
    }

    /**
     * @brief Copy construct a new Game Of Life object
     * 
     * @param[in] other Another GameOfLife object. 
     */
    GameOfLife(const GameOfLife& other) noexcept :
        gameField(other.gameField) 
    {}

    /**
     * @brief Move construct a new Game Of Life object.
     * 
     * @param[in] other Another GameOfLife object.
     */
    GameOfLife(GameOfLife&& other) noexcept :
        gameField(std::move(other.gameField)) 
    {}

    /**
     * @brief Copy assignment operator.
     * 
     * @param[in] other Another GameOfLife object.
     * @return GameOfLife& Reference to this object.
     */
    GameOfLife& operator=(const GameOfLife& other) noexcept {
        if (this != &other) {
            gameField = other.gameField;
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * 
     * @param[in] other Another GameOfLife object.
     * @return GameOfLife& Reference to this object.
     */
    GameOfLife& operator=(GameOfLife&& other) noexcept {
        gameField = std::move(other.gameField);
        return *this;
    }

    /**
     * @brief Get the height of the game field.
     * 
     * @return size_t The height of the game field.
     */
    size_t height() const;

    /**
     * @brief Get the width of the game field.
     * 
     * @return size_t The width of the game field.
     */
    size_t width() const;

    /**
     * @brief Check if a cell on the game field is alive.
     * 
     * @param[in] x The x-coordinate.
     * @param[in] y The y-coordinate.
     * @return true If the cell is alive.
     * @return false If the cell is dead.
     */
    bool isAlive(size_t x, size_t y) const; 

    /**
     * @brief Kill a cell on the game field.
     * 
     * @param[in] x The x-coordinate.
     * @param[in] y The y-coordinate.
     */
    void kill(size_t x, size_t y);
        
    /**
     * @brief Revive a cell on the game field.
     * 
     * @param[in] x The x-coordinate.
     * @param[in] y The y-coordinate.
     */
    void revive(size_t x, size_t y);

    /**
     * @brief Clear the game field.
     */
    void clearField();

    /**
     * @brief Render the next state of the game field. 
     */
    void renderNextGameFieldState();

private:
    Field gameField;
    CellList livingCells;

    static const std::array<const std::pair<int, int>, 8> neighbors;

    bool computeLiveStatus(size_t neighborsCount, bool liveStatus);
    size_t countNeighbors(size_t x, size_t y) const;
};

/**
 * @brief A set of tools for drawing the game field of a GameOfLife object.
 * 
 */
class GameOfLifePainter {
public:
    /**
     * @brief Draw the game field of a GameOfLife object on a QPixmap object. 
     * 
     * @param[in] game The GameOfLife object.
     * @param squareSideLen The side length of the squares.
     * @return QPixmap The resulting QPixmap object.
     */
    static QPixmap paintGameOfLifeFieldOnQPixMap(const GameOfLife& game, size_t squareSideLen);
};

}

#endif
