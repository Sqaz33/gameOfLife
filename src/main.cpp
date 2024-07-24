#include <iostream>
// Включать в следующем порядке:
// Связный заголовочный файл.
// Системные заголоыочные.
// C++ stl заголовочные.
// Заголовочные других библиотек.
// Заголовочные проекта.
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QPoint>

#include "ui_form.h"
#include "include/gameOfLife.h"


class MyWidget : public QWidget {
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr) : 
        QWidget(parent), 
        ui(new Ui_Form)
    {
        ui->setupUi(this);
    }

protected:
    void paintEvent(QPaintEvent *event) override {

        QPixmap pmap(1000, 1000);
        pmap.fill(Qt::white);

        QPainter painter(&pmap);


        for (int x = 0; x < 1000; x+=20) {
            painter.drawLine(x, 0, x, 1000);
        }

        for (int y = 0; y < 1000; y+=20) {
            painter.drawLine(0, y, 1000, y);
        }

        painter.setBrush(Qt::black);
        painter.drawRect(40, 40, 20, 20);

        

        ui->game_field->setPixmap(pmap);

        
    }
private:
    QVBoxLayout* layout;
    QLabel* label;
    Ui_Form* ui;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MyWidget window;
    window.resize(400, 300);
    window.show();

    return app.exec();
}