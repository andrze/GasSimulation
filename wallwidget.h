#ifndef WALLWIDGET_H
#define WALLWIDGET_H

#include <QWidget>
#include "wall.h"

namespace Ui {
class WallWidget;
}

class WallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WallWidget(QWidget *parent = 0);
    ~WallWidget();
    WallWidget(size_t index, Wall wall);
    WallWidget(size_t index);

    size_t index;
    void reindex(size_t);
    Wall wall();

signals:
    void deleteWidget(size_t);

private slots:
    void on_removeButton_clicked();

private:
    Ui::WallWidget *ui;
};

#endif // WALLWIDGET_H
