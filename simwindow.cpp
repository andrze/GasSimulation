#include "simwindow.h"
#include "ui_simwindow.h"
#include <QTimer>
#include <QFileDialog>
#include <iostream>

SimWindow::SimWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimWindow)
{
    ui->setupUi(this);
}

SimWindow::SimWindow(Data data) :
    SimWindow()
{
    this->data = data;
    ui->widget->data = data;
    ui->widget->init();
    int start_temp = int(ui->widget->thermostat);
    ui->tempSlider->setValue(start_temp);
}

SimWindow::~SimWindow()
{
    delete ui;
}

void SimWindow::on_pauseButton_clicked()
{
    if(ui->widget->isActive()){
        ui->pauseButton->setText(tr("Resume"));
        ui->widget->stop();
    }
    else{
        ui->pauseButton->setText(tr("Pause"));
        ui->widget->start();
    }
}

void SimWindow::on_stopButton_clicked(){
    this->ui->widget->stop();
    this->close();
}

void SimWindow::on_restartButton_clicked(){
    ui->widget->stop();
    ui->widget->data = data;
    ui->widget->init();
    ui->pauseButton->setText(tr("Start"));
}

void SimWindow::on_speedSlider_valueChanged(int value)
{
    double step = 0.000002*value;
    data.step = step;
    ui->widget->data.step = step;
}

void SimWindow::on_dataEditButton_clicked()
{
    emit editData();
}

void SimWindow::on_gravitySlider_valueChanged(int value)
{
    data.gravity = value;
    ui->widget->data.gravity = value;
}

void SimWindow::on_statisticsButton_clicked()
{
    std::string filename = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Comma seperated values (*.csv)")).toStdString();
    if( filename.size() == 0 ){
        return;
    }
    if( filename.size() < 4 || filename.substr(filename.size()-4,4) != ".csv" ){
        filename += ".csv";
    }
    std::ofstream file(filename);
    file << ui->widget->data.statistics();
    file.close();
}

void SimWindow::on_tempSlider_valueChanged(int position)
{
    ui->widget->thermostat = position;
    ui->tempLabel->setText(QString::fromStdString(std::to_string(position)));
}
