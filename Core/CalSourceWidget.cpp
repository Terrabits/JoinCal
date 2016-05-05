#include "CalSourceWidget.h"
#include "ui_CalSourceWidget.h"


// Project
#include "GetCalDialog.h"

// RsaToolbox
using namespace RsaToolbox;


CalSourceWidget::CalSourceWidget(Vna *vna, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalSourceWidget),
    _vna(vna)
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

void CalSourceWidget::setSource(const CalibrationSource &source) {
    if (_source == source)
        return;

    _source = source;
    updateText();
    emit sourceChanged(_source);
}
CalibrationSource CalSourceWidget::source() const {
    return _source;
}

void CalSourceWidget::showCalDialog() {
    GetCalDialog dialog(_vna);
    dialog.exec();
    CalibrationSource source = dialog.selectedCal();
    if (source.isEmpty())
        return;

    if (source != _source) {
        _source = source;
        updateText();
        emit sourceChanged(_source);
    }
}

void CalSourceWidget::updateText() {
    ui->lineEdit->setText(_source.displayText());
}
