#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "planet.h"
#include <QWidget>

namespace Ui {
class DataWidget;
}

class DataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataWidget(QWidget *parent = 0);
    explicit DataWidget(size_t number);
    explicit DataWidget(size_t number, Planet planet);

    ~DataWidget();

    Planet planet();
    size_t index=0;
    void reindex(size_t);
    Vector2d location();
    void paint(bool color);

signals:
    void deleteWidget(size_t);
    void locChanged();

private slots:
    void on_deleteButton_clicked();

    void on_pos_y_valueChanged(double);

    void on_pos_x_valueChanged(double);

private:
    Ui::DataWidget *ui;
};

#endif // DATAWIDGET_H
