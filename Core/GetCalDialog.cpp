#include "GetCalDialog.h"
#include "ui_GetCalDialog.h"


// Project
#include "Corrections.h"

// RsaToolbox
#include <Shake.h>
using namespace RsaToolbox;

// Qt
#include <QModelIndexList>
#include <QTreeWidgetItem>


GetCalDialog::GetCalDialog(Vna *vna, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetCalDialog)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    _calGroups = vna->calGroups();
    if (_calGroups.isEmpty()) {
        const int i = ui->tabWidget->indexOf(ui->calGroupTab);
        ui->tabWidget->removeTab(i);
    }
    else {
        ui->calGroupTreeWidget->clear();
        for (int i = 0; i < _calGroups.size(); i++) {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            ui->calGroupTreeWidget->addTopLevelItem(item);
            item->setText(0, _calGroups[i]);
            Calibration source;
            source.source().setCalGroup(_calGroups[i]);
            item->setText(1, Corrections(source, vna).displayText());
        }
    }

    _channels = calibratedChannels(vna);
    if (_channels.isEmpty()) {
        const int i = ui->tabWidget->indexOf(ui->channelTab);
        ui->tabWidget->removeTab(i);
    }
    else {
        ui->channelTreeWidget->clear();
        for (int i = 0; i < _channels.size(); i++) {
            const uint c = _channels[i];
            QTreeWidgetItem *item = new QTreeWidgetItem;
            ui->channelTreeWidget->addTopLevelItem(item);
            item->setText(0, vna->channel(c).name());
            Calibration source;
            source.source().setChannel(c);
            item->setText(1, Corrections(source, vna).displayText());
        }
    }
}

GetCalDialog::~GetCalDialog()
{
    delete ui;
}

void GetCalDialog::setDefault(CalibrationSource source) {
    if (source.isEmpty())
        return;

    if (source.isChannel() && _channels.contains(source.channel())) {
        ui->tabWidget->setCurrentWidget(ui->channelTab);
        int row = _channels.indexOf(source.channel());
        QModelIndex index = ui->channelTreeWidget->model()->index(row,0);
        ui->channelTreeWidget->setCurrentIndex(index);
        ui->channelTreeWidget->setFocus();
    }
    else if (source.isCalGroup() && _calGroups.contains(source.calGroup())) {
        ui->tabWidget->setCurrentWidget(ui->calGroupTab);
        int row = _calGroups.indexOf(source.calGroup());
        QModelIndex index = ui->calGroupTreeWidget->model()->index(row, 0);
        ui->calGroupTreeWidget->setCurrentIndex(index);
        ui->calGroupTreeWidget->setFocus();
    }
}

CalibrationSource GetCalDialog::selectedCal() const {
    if (result() == QDialog::Rejected)
        return CalibrationSource();

    if (ui->tabWidget->currentWidget() == ui->calGroupTab) {

        QModelIndexList rows = ui->calGroupTreeWidget->selectionModel()->selectedRows(0);
        if (rows.isEmpty()) {
            return CalibrationSource();
        }
        else {
            const int i = rows.first().row();
            return CalibrationSource(_calGroups[i]);
        }
    }
    else {
        QModelIndexList rows = ui->channelTreeWidget->selectionModel()->selectedRows(0);
        if (rows.isEmpty()) {
            return CalibrationSource();
        }
        else {
            const int i = rows.first().row();
            return CalibrationSource(_channels[i]);
        }
    }
}

void GetCalDialog::accept() {
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QModelIndexList rows;
    if (currentWidget == ui->calGroupTab) {
        rows = ui->calGroupTreeWidget->selectionModel()->selectedRows(0);

    }
    else {
        rows = ui->channelTreeWidget->selectionModel()->selectedRows(0);
    }
    if (rows.isEmpty()) {
        shake();
        return;
    }
    else {
        QDialog::accept();
    }
}

void GetCalDialog::shake() {
    RsaToolbox::shake(this);
}

QVector<uint> GetCalDialog::calibratedChannels(Vna *vna) {
    QVector<uint> result;
    foreach (uint c, vna->channels()) {
        if (vna->channel(c).isCalibrated())
            result << c;
    }
    return result;
}
