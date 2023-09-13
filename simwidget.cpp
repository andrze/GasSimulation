#include "simwidget.h"
#include "ui_simwidget.h"
#include <QPainter>
#include <memory>
#include <cmath>
#include <iostream>
#include <QPainterPath>

QPoint calc_parameters(double unit, QPoint center, Vector2d location){
    int x = center.x() + location.x*unit;
    int y = center.y() + location.y*unit;
    return QPoint(x,y);
}

SimWidget::SimWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimWidget)
{
    ui->setupUi(this);
    timer = std::make_unique<QTimer>(this);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(sim_move()));
}

SimWidget::~SimWidget()
{
    delete ui;
}

void SimWidget::init(){
    QPalette p = palette();
    QColor color(255,255,255);
    p.setColor(backgroundRole(), color);
    setPalette(p);
    this->thermostat = data.temperature()*temp_unit;
    this->update();
}

void SimWidget::sim_move(){
    int non_drawn_moves=50;
    QPoint center(width()/2, height()/2);
    for (int i=0; i < non_drawn_moves; i++){
        data.move(thermostat/temp_unit);
    }
    for (auto &&p: data.planets){
        QPoint end = calc_parameters(unit, center, p.location);
        float c = std::max(0, std::min(int(255*p.velocity.length()/5), 255));
        QColor color = QColor(c,0,255-c);
        points.push_back(ColorPoint({end, color}));
    }
    phase++;
    this->update();
}

void SimWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    for(auto&& p: points){
        QPainterPath path;
        path.addEllipse(p.point, 4, 4);
        //painter.fillPath(path, QColor(255,255,255));
        painter.fillPath(path, p.color);
    }
    points.clear();
    QPoint center(width()/2, height()/2);
    for(auto&& w:data.walls){
        QPoint start, end;
        if(w.horizontal){
            start = calc_parameters(unit, center, Vector2d(w.a1,w.b));
            end = calc_parameters(unit, center, Vector2d(w.a2,w.b));
        } else {
            start = calc_parameters(unit, center, Vector2d(w.b,w.a1));
            end = calc_parameters(unit, center, Vector2d(w.b,w.a2));
        }
        painter.drawLine(start, end);
    }
}

void SimWidget::start(){
    this->timer->start(5);
}

void SimWidget::stop(){
    this->timer->stop();
}

bool SimWidget::isActive(){
    return this->timer->isActive();
}
