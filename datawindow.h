#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "datawidget.h"
#include "wallwidget.h"
#include "data.h"

namespace Ui {
class DataWindow;
}

class DataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataWindow(QMainWindow *parent = 0);
    explicit DataWindow(Data data);
    explicit DataWindow(size_t n_planets);

    ~DataWindow();

    void connectSignals(int index=-1);

    std::vector< std::unique_ptr<DataWidget> > input;
    std::vector< std::unique_ptr<WallWidget> > wall_input;
    int margin=30;
    int minimumHeight();
    Data extract_data();
    void mousePressEvent(QMouseEvent *e);
signals:
    void confirmed();

private slots:
    void on_simulateButton_clicked();
    void on_searchButton_clicked();

    void on_addPlanetButton_clicked();
    void on_addWallButton_clicked();

    void deleteWidget(size_t index);
    void deleteWallWidget(size_t index);

    void on_returnButton_clicked();
    void checkData();

    void on_horizontalScrollBar_valueChanged(int);
    void on_horizontalScrollBar2_valueChanged(int);


private:
    Ui::DataWindow *ui;
    void setupPlanetWidgets();
    void setupWallWidgets();
};

#endif // DATAWINDOW_H
