/**
 * @file gameOfLife.h
 * @author Matveev Stepan (mset321@gmail.com).
 * @brief Header file containing the GameOfLifeModel and GameOfLifePainter classes.
 * @version 1.0
 * @date 2024-07-25
 */

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <array>
#include <cstddef>
#include <functional>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace game_of_life {
enum class CellStatus : std::uint8_t;
using Row = typename  std::vector<CellStatus>; ///< Designation of a row of the game field.
using Field = typename std::vector<Row>; ///< Designation of the game field.
using Cell = std::pair<std::size_t, std::size_t>;///< Designation of cell
using CellList = std::list<Cell>; ///< Designation of list of cells

/**
 * @class GameOfLifeModel
 * @brief Class for creating and processing a single session of the cellular automaton "Game of Life".
 * 
 * @note 1. In the methods of the class, cells are universally referred to as point (x, y).
 *          Note that the origin is considered the point (0, 0), located in the upper left
 *          corner of the game field.
 * 
 * @note 2. The field has clear vertical boundaries: the upper and lower boundaries are not connected in pairs.
 */
class GameOfLifeModel {
public:
    /**
     * @brief Construct a new Game Of Life object
     */
    GameOfLifeModel();

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
    GameOfLifeModel(std::size_t width, std::size_t height);
    

    /**
     * @brief Copy construct a new Game Of Life object
     * 
     * @param[in] other Another GameOfLifeModel object. 
     */
    GameOfLifeModel(const GameOfLifeModel& other);

    /**
     * @brief Move construct a new Game Of Life object.
     * 
     * @param[in] other Another GameOfLifeModel object.
     */
    GameOfLifeModel(GameOfLifeModel&& other) noexcept;

    /**
     * @brief Copy assignment operator.
     * 
     * @param[in] other Another GameOfLifeModel object.
     * @return GameOfLifeModel& Reference to this object.
     */
    GameOfLifeModel& operator=(const GameOfLifeModel& other);

    /**
     * @brief Move assignment operator.
     * 
     * @param[in] other Another GameOfLifeModel object.
     * @return GameOfLifeModel& Reference to this object.
     */
    GameOfLifeModel& operator=(GameOfLifeModel&& other) noexcept;

    /**
     * @brief Get the height of the game field.
     * 
     * @return std::size_t The height of the game field.
     */
    std::size_t height() const;

    /**
     * @brief Get the width of the game field.
     * 
     * @return std::size_t The width of the game field.
     */
    std::size_t width() const;

    /**
     * @brief Check if a cell on the game field is alive.
     * 
     * @param[in] x The x-coordinate.
     * @param[in] y The y-coordinate.
     * @return true If the cell is alive.
     * @return false If the cell is dead.
     */
    bool isAlive(std::size_t x, std::size_t y) const; 

    /**
     * @brief Kill a cell on the game field.
     * 
     * @param[in] x The x-coordinate.
     * @param[in] y The y-coordinate.
     */
    void kill(std::size_t x, std::size_t y);
        
    /**
     * @brief Revive a cell on the game field.
     * 
     * @param[in] x The x-coordinate.
     * @param[in] y The y-coordinate.
     */
    void revive(std::size_t x, std::size_t y);

    /**
     * @brief Clear the game field.
     */
    void clear();

    /**
     * @brief Render the next state of the game field. 
     */
    void update();

private:
    Field gameField;
    CellList livingCells;

    static const std::array<const std::pair<int, int>, 8> neighbors;

    bool computeLiveStatus(std::size_t neighborsCount, bool isLive);
    std::size_t countNeighbors(std::size_t x, std::size_t y) const;
};


} // namespace game_of_life

#endif
