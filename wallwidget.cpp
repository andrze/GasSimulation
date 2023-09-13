#include "wallwidget.h"
#include "ui_wallwidget.h"

WallWidget::WallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WallWidget)
{
    ui->setupUi(this);
}

WallWidget::~WallWidget()
{
    delete ui;
}

WallWidget::WallWidget(size_t index, Wall wall) :
    WallWidget(index)
{
    if(wall.horizontal){
        ui->horizontalButton->setChecked(true);
        ui->verticalButton->setChecked(false);
    } else {
        ui->horizontalButton->setChecked(false);
        ui->verticalButton->setChecked(true);
    }
    ui->a->setValue(wall.b);
    ui->max_b->setValue(wall.a2);
    ui->min_b->setValue(wall.a1);
}

WallWidget::WallWidget(size_t index) :
    WallWidget()
{
    reindex(index);
}

void WallWidget::reindex(size_t index){
    ui->title->setText(QString("Wall %1").arg(index+1));
    this->index=index;
}

Wall WallWidget::wall(){
    Wall wall;
    wall.horizontal = ui->horizontalButton->isChecked();
    wall.a1 = ui->min_b->value();
    wall.a2 = ui->max_b->value();
    wall.b = ui->a->value();
    return wall;
}

void WallWidget::on_removeButton_clicked()
{
    emit deleteWidget(index);
}
