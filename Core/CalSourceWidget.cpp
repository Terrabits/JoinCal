#include "CalSourceWidget.h"
#include "ui_CalSourceWidget.h"


// Project
#include "GetCalDialog.h"


CalSourceWidget::CalSourceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalSourceWidget)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(showCalDialog()));
    ui->lineEdit->setText(_source.displayText());
}

CalSourceWidget::~CalSourceWidget()
{
    delete ui;
}

void CalSourceWidget::showCalDialog() {
    GetCalDialog dialog(_vna);
    dialog.exec();
    CalibrationSource source = dialog.selectedCal();
    if (source.isEmpty())
        return;

    if (source != _source) {
        _source = source;
        ui->lineEdit->setText(_source.displayText());
        emit sourceChanged(_source);
    }
}
