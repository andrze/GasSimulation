#ifndef SIMWINDOW_H
#define SIMWINDOW_H

#include <QMainWindow>
#include "data.h"

namespace Ui {
class SimWindow;
}

class SimWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimWindow(QWidget *parent = nullptr);
    explicit SimWindow(Data data);

    ~SimWindow();

    Data data;
private slots:
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_restartButton_clicked();

    void on_speedSlider_valueChanged(int value);

    void on_dataEditButton_clicked();

    void on_gravitySlider_valueChanged(int value);

    void on_statisticsButton_clicked();

    void on_tempSlider_valueChanged(int position);

signals:
    void editData();

private:
    Ui::SimWindow *ui;
};

#endif // SIMWINDOW_H
