#include "datawindow.h"
#include "data.h"
#include "datawidget.h"
#include "ui_datawindow.h"
#include "vector2d.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <algorithm>
#include <fstream>
#include <memory>

#include <iostream>

DataWindow::DataWindow(QMainWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::DataWindow)
{
    ui->setupUi(this);
}

DataWindow::DataWindow(Data data)
    : DataWindow()
{
    input.clear();
    for (size_t i = 0; i < data.planets.size(); i++) {
        input.push_back(std::make_unique<DataWidget>(i, data.planets[i]));
        ui->dataContainer->addWidget(input[i].get());
        input[i]->show();
    }
    for (size_t i = 0; i < data.walls.size(); i++) {
        wall_input.push_back(std::make_unique<WallWidget>(i, data.walls[i]));
        ui->wallDataContainer->addWidget(wall_input[i].get());
        wall_input[i]->show();
    }
    setupPlanetWidgets();
    setupWallWidgets();
    connectSignals();
    checkData();
}

DataWindow::DataWindow(size_t n_planets)
    : DataWindow()
{
    Data data;
    data.planets = std::vector<Planet>(n_planets);
    input.clear();
    for (size_t i = 0; i < data.planets.size(); i++) {
        input.push_back(std::make_unique<DataWidget>(i, data.planets[i]));
        ui->dataContainer->addWidget(input[i].get());
        input[i]->show();
    }
    for (size_t i = 0; i < data.walls.size(); i++) {
        wall_input.push_back(std::make_unique<WallWidget>(i, data.walls[i]));
        ui->wallDataContainer->addWidget(wall_input[i].get());
        wall_input[i]->show();
    }
    setupPlanetWidgets();
    setupWallWidgets();
    connectSignals();
    checkData();
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::connectSignals(int)
{
    for (auto&& w : input) {
        connect(w.get(), SIGNAL(deleteWidget(size_t)), this, SLOT(deleteWidget(size_t)));
        connect(w.get(), SIGNAL(locChanged()), this, SLOT(checkData()));
    }
    for (auto&& w : wall_input) {
        connect(w.get(), SIGNAL(deleteWidget(size_t)), this, SLOT(deleteWallWidget(size_t)));
    }
}

Data DataWindow::extract_data()
{
    std::vector<Planet> planets;
    for (auto&& widget : input) {
        planets.push_back(widget->planet());
    }
    std::vector<Wall> walls;
    for (auto&& widget : wall_input) {
        walls.push_back(widget->wall());
    }
    return Data(planets, walls);
}

void DataWindow::on_searchButton_clicked()
{
    std::string filename = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Data files (*.dat)")).toStdString();
    if (filename.size() == 0) {
        return;
    }
    if (filename.size() < 4 || filename.substr(filename.size() - 4, 4) != ".dat") {
        filename += ".dat";
    }
    std::ofstream file(filename);
    file << extract_data();
    file.close();
}

void DataWindow::setupPlanetWidgets()
{
    size_t n_planets = input.size();
    DataWidget dummy;
    int planet_widget_width = dummy.width(), planet_layout_width = (planet_widget_width + 5) * n_planets - 5,
        window_width = this->width(), planet_hidden_width = std::max(0, planet_layout_width - window_width + 5);
    int scroll_position = 0;
    if (planet_hidden_width == 0) {
        ui->horizontalScrollBar->setHidden(true);
    } else {
        ui->horizontalScrollBar->setHidden(false);
        ui->horizontalScrollBar->setMaximum(planet_hidden_width / 10);
        scroll_position = planet_hidden_width * ui->horizontalScrollBar->value() / ui->horizontalScrollBar->maximum();
    }
    ui->horizontalScrollBar->setValue(scroll_position / 10);
    ui->horizontalLayoutWidget->setGeometry(-scroll_position, margin, planet_layout_width, dummy.height());
}

void DataWindow::setupWallWidgets()
{
    size_t n_walls = wall_input.size();
    WallWidget dummy;
    int walls_widget_width = dummy.width(), walls_layout_width = (walls_widget_width + 5) * n_walls - 5,
        window_width = this->width(), walls_hidden_width = std::max(0, walls_layout_width - window_width + 5);
    int scroll_position = 0;
    if (walls_hidden_width == 0) {
        ui->horizontalScrollBar2->setHidden(true);
    } else {
        ui->horizontalScrollBar2->setHidden(false);
        ui->horizontalScrollBar2->setMaximum(walls_hidden_width / 10);
        scroll_position = walls_hidden_width * ui->horizontalScrollBar2->value() / ui->horizontalScrollBar2->maximum();
    }
    ui->horizontalScrollBar2->setValue(scroll_position / 10);
    ui->horizontalLayoutWidget_2->setGeometry(-scroll_position, ui->horizontalLayoutWidget->y() + ui->horizontalLayoutWidget->height(), walls_layout_width, dummy.height());
}

void DataWindow::on_addPlanetButton_clicked()
{
    size_t size = input.size();
    input.push_back(std::make_unique<DataWidget>(size));
    ui->dataContainer->addWidget(input[size].get());
    setupPlanetWidgets();
    connectSignals(size);
    checkData();
}

void DataWindow::on_addWallButton_clicked()
{
    size_t size = wall_input.size();
    wall_input.push_back(std::make_unique<WallWidget>(size));
    ui->wallDataContainer->addWidget(wall_input[size].get());
    setupWallWidgets();
    connectSignals(size);
    checkData();
}

void DataWindow::deleteWidget(size_t index)
{
    if (input.size() <= 2) {
        return;
    }
    input.erase(input.begin() + index);

    for (size_t i = index; i < input.size(); i++) {
        input[i]->reindex(i);
    }

    setupPlanetWidgets();
    checkData();
}

void DataWindow::deleteWallWidget(size_t index)
{
    wall_input.erase(wall_input.begin() + index);

    for (size_t i = index; i < wall_input.size(); i++) {
        wall_input[i]->reindex(i);
    }

    setupWallWidgets();
    checkData();
}

void DataWindow::on_returnButton_clicked()
{
    this->hide();
}

void DataWindow::checkData()
{
    std::vector<bool> problems(input.size());
    bool problem = false;
    for (size_t j = 0; j < input.size(); j++) {
        for (size_t i = j + 1; i < input.size(); i++) {
            if (input[i]->location() == input[j]->location()) {
                input[i]->paint(true);
                input[j]->paint(true);
                problems[i] = true;
                problems[j] = true;
                problem = true;
            }
        }
    }
    for (size_t i = 0; i < problems.size(); i++) {
        if (!problems[i]) {
            input[i]->paint(false);
        }
    }
    if (problem) {
        ui->simulateButton->setEnabled(false);
    } else {
        ui->simulateButton->setEnabled(true);
    }
}

void DataWindow::on_simulateButton_clicked()
{
    emit confirmed();
}

void DataWindow::mousePressEvent(QMouseEvent* e)
{
    QRect rect = ui->simulateButton->geometry();
    QPoint p = e->pos();
    if (p.x() > rect.x() && p.x() < rect.x() + rect.width() && p.y() > rect.y() && p.y() < rect.y() + rect.height()) {
        ui->simulateButton->click();
    }
}

void DataWindow::on_horizontalScrollBar_valueChanged(int)
{
    setupPlanetWidgets();
}

void DataWindow::on_horizontalScrollBar2_valueChanged(int)
{
    setupWallWidgets();
}
