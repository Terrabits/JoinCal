#include "CalibrationWidget.h"
#include "ui_CalibrationWidget.h"


// RsaToolbox
using namespace RsaToolbox;


CalibrationWidget::CalibrationWidget(Vna *vna, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationWidget),
    _vna(vna)
{
    ui->setupUi(this);

    CalWidgetRow *row = new CalWidgetRow(vna, this);

    _layout = new QGridLayout(this);
    this->setLayout(_layout);
    _layout->addWidget(row->sourceWidget, 0, 0);
    _layout->addWidget(row->startEdit, 0, 1);
    _layout->addWidget(row->stopEdit, 0, 2);
    _layout->addWidget(row->summaryLabel, 0, 3);
    _layout->setRowStretch(1, 1);
}

CalibrationWidget::~CalibrationWidget()
{
    delete ui;
}
