#include "vector2d.h"
#include "planet.h"
#include "datawidget.h"
#include "ui_datawidget.h"
#include <QPalette>

DataWidget::DataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);
}

DataWidget::DataWidget(size_t number) :
    DataWidget()
{
    index=number;
    ui->title->setText(QString("Particle %1").arg(number+1));
}

DataWidget::DataWidget(size_t number, Planet planet) :
    DataWidget()
{
    index=number;
    ui->title->setText(QString("Particle %1").arg(number+1));
    ui->pos_x->setValue(planet.location.x);
    ui->pos_y->setValue(planet.location.y);
    ui->vel_x->setValue(planet.velocity.x);
    ui->vel_y->setValue(planet.velocity.y);
    ui->mass->setValue(planet.mass);
}

DataWidget::~DataWidget()
{
    delete ui;
}

Planet DataWidget::planet(){
    Planet planet;
    planet.location=Vector2d(ui->pos_x->value(), ui->pos_y->value());
    planet.velocity=Vector2d(ui->vel_x->value(), ui->vel_y->value());
    planet.mass = ui->mass->value();
    return planet;
}

void DataWidget::reindex(size_t index){
    this->index = index;
    ui->title->setText(QString("Particle %1").arg(index+1));
}

Vector2d DataWidget::location(){
    return Vector2d(ui->pos_x->value(),ui->pos_y->value());
}

void DataWidget::paint(bool color){
    if(color){
        this->ui->pos_x->setStyleSheet("QDoubleSpinBox { background-color: red; }");
        this->ui->pos_y->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    } else {
        this->ui->pos_x->setStyleSheet("QDoubleSpinBox { background-color: white; }");
        this->ui->pos_y->setStyleSheet("QDoubleSpinBox { background-color: white; }");
    }
}

void DataWidget::on_deleteButton_clicked(){
    emit deleteWidget(index);
}

void DataWidget::on_pos_y_valueChanged(double){
    emit locChanged();
}

void DataWidget::on_pos_x_valueChanged(double){
    emit locChanged();
}
