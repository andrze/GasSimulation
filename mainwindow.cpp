#include <stdexcept>
#include <fstream>
#include <QObject>
#include <QFileDialog>
#include <QCloseEvent>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data.h"
#include "simwindow.h"
#include "datawindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_simulation(Data data){
    try{
        simwindow = std::make_unique<SimWindow>(data);
    }
    catch (std::invalid_argument&){
        ui->errorLabel->setText(tr("Invalid input data"));
        return;
    }
    ui->reloadButton->setEnabled(true);
    simwindow->show();
    connect(simwindow.get(), SIGNAL(editData()), this, SLOT(editData()));
}

void MainWindow::open_file(QString filename){
    ui->filenameEdit->setText(filename);
    ui->errorLabel->clear();
    std::ifstream stream;
    stream.open(filename.toStdString());
    if(stream.fail()){
        ui->errorLabel->setText(tr("Error while opening the file"));
        return;
    }
    datawindow = std::make_unique<DataWindow>(Data(stream));
    datawindow->show();
    connect(datawindow.get(),SIGNAL(confirmed()),this,SLOT(get_data()));
    ui->reloadButton->setEnabled(true);
}

void MainWindow::on_filenameButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Pliki danych (*.dat)"));
    if( !filename.isEmpty() ){
        open_file(filename);
    }
}

void MainWindow::on_reloadButton_clicked()
{
    QString filename = ui->filenameEdit->text();
    open_file(filename);
}


void MainWindow::on_dataButton_clicked()
{
    size_t n_planets = ui->dataBox->value();
    if( n_planets != 0 ){
        datawindow = std::make_unique<DataWindow>(n_planets);
        datawindow->show();
        connect(datawindow.get(),SIGNAL(confirmed()),this,SLOT(get_data()));
    }
}

void MainWindow::get_data(){
    Data data(datawindow->extract_data());
    datawindow = nullptr;
    start_simulation(data);
}

void MainWindow::closeEvent (QCloseEvent *event){
    datawindow = nullptr;
    simwindow = nullptr;
    event->accept();
}

void MainWindow::editData(){
    datawindow = std::make_unique<DataWindow>(simwindow->data);
    simwindow = nullptr;
    connect(datawindow.get(),SIGNAL(confirmed()),this,SLOT(get_data()));
    datawindow->show();
}

void MainWindow::on_dataButton_2_clicked()
{
    Data data(ui->dataBox->value(), ui->wallNumberBox->value());
    start_simulation(data);
}

