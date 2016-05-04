#include "CalSourceWidget.h"
#include "ui_CalSourceWidget.h"

CalSourceWidget::CalSourceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalSourceWidget)
{
    ui->setupUi(this);
}

CalSourceWidget::~CalSourceWidget()
{
    delete ui;
}
