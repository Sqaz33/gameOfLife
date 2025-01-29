#ifndef INCLUDE_VIEW_HPP
#define INCLUDE_VIEW_HPP

#include <QPixmap>
#include "gameOfLife.hpp"

namespace view {

/**
 * @brief A set of tools for drawing the game field of a GameOfLifeModel object.
 * 
 */
class GameOfLifePainter {
public:
    /**
     * @brief Draw the game field of a GameOfLifeModel object on a QPixmap object. 
     * 
     * @param[in] game The GameOfLifeModel object.
     * @param squareSideLen The side length of the squares.
     * @return QPixmap The resulting QPixmap object.
     */
    static QPixmap paintGameOfLifeFieldOnQPixMap(const game_of_life::GameOfLifeModel& game, size_t squareSideLen);
};

} // namespace view


#endif // INCLUDE_VIEW_HPP