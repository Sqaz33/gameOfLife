#include "../include/view.hpp"

#include <QPainter>
#include <QBrush>

struct FieldKey {
    size_t w, h, s;
    auto operator<=>(const FieldKey&) const = default;
};

namespace std {
    template<> struct hash<FieldKey> {
        size_t operator()(const FieldKey& key) const {
            std::hash<size_t> h;
            return h(key.w) ^ h(key.h << 1) ^ h(key.s << 2);
        }
    };
}


static void drawFieldSquares(QPixmap& field, size_t sideLen, Qt::GlobalColor color) {
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


QPixmap view::GameOfLifePainter::paintGameOfLifeFieldOnQPixMap(const game_of_life::GameOfLifeModel& game, size_t squareSideLen) {
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