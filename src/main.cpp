#include <chrono>
#include <iostream>
#include <thread>

#include <QApplication>
#include <QComboBox>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>

#include "../include/gameOfLife.hpp"
#include "../include/view.hpp"
#include "ui_form.h"
#include "windows.h"

using game_of_life::GameOfLifeModel;
using view::GameOfLifePainter;

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
        ui_->setupUi(this);

        gameField_ = new ClickableLabel(this);
        gameField_->setObjectName(QString::fromUtf8("gameField_"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gameField_->sizePolicy().hasHeightForWidth());
        gameField_->setSizePolicy(sizePolicy);
        ui_->verticalLayout_3->addWidget(gameField_);
        
        QPixmap field = GameOfLifePainter::paintGameOfLifeFieldOnQPixMap(game_, 10);
        gameField_->setPixmap(field);
        
        connect(
            ui_->mode_selection_comboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MyWidget::changeMode   
        );

        connect(
            gameField_, 
            &ClickableLabel::cellClicked, 
            this, 
            &MyWidget::onCellClicked
        );

        connect(
            ui_->clear_field_button,
            &QPushButton::clicked,
            this,
            &MyWidget::clearGameField
        );

        connect(
            ui_->do_step_button,
            &QPushButton::clicked,
            this,
            &MyWidget::doStep
        );

        connect(
            ui_->per_second_spinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &MyWidget::perSecondValueChanged
        );

        connect(
            ui_->start_button,
            &QPushButton::clicked,
            this,
            &MyWidget::start
        );
        
        connect(
            ui_->stop_button,
            &QPushButton::clicked,
            this,
            &MyWidget::stop
        );

        timer_ = new QTimer(this);
        connect(
            timer_, 
            &QTimer::timeout, 
            this, 
            &MyWidget::moveToNextStage
        );
    }

private slots:
    void changeMode(int index) {
        if (index == 0) {
            timer_->stop();
        }
        ui_->stackedWidget->setCurrentIndex(index);
        
    }

    void start() {
        timer_->start(timerTime_); 
    }

    void stop() {
        timer_->stop(); 
    }

    void doStep() {
        moveToNextStage();
    }

    void clearGameField() {
        game_.clear();
        updateFieldOnLabel();
    }

    void perSecondValueChanged(int perSecond) {
        perSecond = perSecond ? perSecond : 1;
        timerTime_ = 1000 / perSecond;
        timer_->setInterval(timerTime_);
    }

    void onCellClicked(int x, int y) {
        int cellX = x / 10; 
        int cellY = y / 10;
        if (game_.isAlive(cellX, cellY)) {
            game_.kill(cellX, cellY);
        } else {
            game_.revive(cellX, cellY);
        }
        updateFieldOnLabel();
    }

private:    
    Ui_Form* ui_ = new Ui_Form;
    GameOfLifeModel game_ = GameOfLifeModel(100, 100);
    QTimer* timer_;
    ClickableLabel* gameField_;
    size_t timerTime_ = 500; 

    void moveToNextStage() {
        game_.update();
        updateFieldOnLabel();
    }

    void updateFieldOnLabel() {
        QPixmap field = GameOfLifePainter::paintGameOfLifeFieldOnQPixMap(game_, 10);
        gameField_->setPixmap(field);
    }
};

#include "main.moc"


int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8); 

    QApplication app(argc, argv);

    MyWidget window;
    window.resize(400, 300);
    window.show();
    return app.exec();
}
