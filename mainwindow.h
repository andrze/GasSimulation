#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "data.h"
#include "simwindow.h"
#include "datawindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    std::unique_ptr<SimWindow> simwindow;
    std::unique_ptr<DataWindow> datawindow;

private slots:
    void on_filenameButton_clicked();
    void on_reloadButton_clicked();
    void on_dataButton_clicked();
    void get_data();
    void editData();

    void on_dataButton_2_clicked();

private:
    Ui::MainWindow *ui;
    void open_file(QString filename);
    void start_simulation(Data data);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
