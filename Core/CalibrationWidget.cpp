#include "CalibrationWidget.h"
#include "ui_CalibrationWidget.h"

CalibrationWidget::CalibrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationWidget)
{
    ui->setupUi(this);
}

CalibrationWidget::~CalibrationWidget()
{
    delete ui;
}
