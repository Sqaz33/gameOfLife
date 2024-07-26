#include <iostream>
#include <thread>
#include <chrono>

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QPoint>
#include <QComboBox>
#include <QTimer>
#include <QSpinBox>
#include <QMouseEvent>

#include "ui_form.h"
#include "include/gameOfLife.h"

#include "windows.h"

using game_of_life::GameOfLife;
using game_of_life::GameOfLifePainter;

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {}

signals:
    void cellClicked(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit cellClicked(event->x(), event->y());
        }
    }
};

class MyWidget : public QWidget {
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr) : 
        QWidget(parent)
    {
        ui->setupUi(this);

        game_field = new ClickableLabel(this);
        game_field->setObjectName(QString::fromUtf8("game_field"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(game_field->sizePolicy().hasHeightForWidth());
        game_field->setSizePolicy(sizePolicy);
        ui->verticalLayout_3->addWidget(game_field);
        
        QPixmap field = GameOfLifePainter::paintGameOfLifeFieldOnQPixMap(game, 10);
        game_field->setPixmap(field);
        
        connect(
            ui->mode_selection_comboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MyWidget::changeMode   
        );

        connect(
            game_field, 
            &ClickableLabel::cellClicked, 
            this, 
            &MyWidget::onCellClicked
        );

        connect(
            ui->clear_field_button,
            &QPushButton::clicked,
            this,
            &MyWidget::clearGameField
        );

        connect(
            ui->do_step_button,
            &QPushButton::clicked,
            this,
            &MyWidget::doStep
        );

        connect(
            ui->per_second_spinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &MyWidget::perSecondValueChanged
        );

        connect(
            ui->start_button,
            &QPushButton::clicked,
            this,
            &MyWidget::start
        );
        
        connect(
            ui->stop_button,
            &QPushButton::clicked,
            this,
            &MyWidget::stop
        );

        timer = new QTimer(this);
        connect(
            timer, 
            &QTimer::timeout, 
            this, 
            &MyWidget::moveToNextStage
        );
    }

private slots:
    void changeMode(int index) {
        if (index == 0) {
            timer->stop();
        }
        ui->stackedWidget->setCurrentIndex(index);
        
    }

    void start() {
        timer->start(timerTime); 
    }

    void stop() {
        timer->stop(); 
    }

    void doStep() {
        moveToNextStage();
    }

    void clearGameField() {
        game.clearField();
        updateFieldOnLabel();
    }

    void perSecondValueChanged(int perSecond) {
        perSecond = perSecond ? perSecond : 1;
        timerTime = 1000 / perSecond;
        timer->setInterval(timerTime);
    }

    void onCellClicked(int x, int y) {
        int cellX = x / 10; 
        int cellY = y / 10;
        if (game.isAlive(cellX, cellY)) {
            game.kill(cellX, cellY);
        } else {
            game.revive(cellX, cellY);
        }
        updateFieldOnLabel();
    }

private:    
    Ui_Form* ui = new Ui_Form;
    GameOfLife game = GameOfLife(100, 100);
    QTimer* timer;
    ClickableLabel* game_field;
    size_t timerTime = 500; 

    void moveToNextStage() {
        game.renderNextGameFieldState();
        updateFieldOnLabel();
    }

    void updateFieldOnLabel() {
        QPixmap field = GameOfLifePainter::paintGameOfLifeFieldOnQPixMap(game, 10);
        game_field->setPixmap(field);
    }
};

#include "main.moc"


#include <list>
#include <utility>
int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8); 

    QApplication app(argc, argv);

    MyWidget window;
    window.resize(400, 300);
    window.show();
    return app.exec();
}
