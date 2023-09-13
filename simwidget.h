#ifndef SIMWIDGET_H
#define SIMWIDGET_H

#include <QWidget>
#include <QTimer>
#include <memory>
#include "data.h"

struct ColorPoint{
    QPoint point;
    QColor color;
};

namespace Ui {
class SimWidget;
}

class SimWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimWidget(QWidget *parent = 0);
    ~SimWidget();

    Data data;
    double unit=100;
    double temp_unit=100;
    int phase=0;
    double thermostat;

    void init();
    void paintEvent(QPaintEvent*);
    void start();
    void stop();
    bool isActive();

private slots:
    void sim_move();

private:
    std::vector<ColorPoint> points;
    std::unique_ptr<QTimer> timer;
    Ui::SimWidget *ui;
};


#endif // SIMWIDGET_H
