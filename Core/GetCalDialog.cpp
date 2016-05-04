#include "GetCalDialog.h"
#include "ui_GetCalDialog.h"


// RsaToolbox
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
            item->setText(1, "Summary");
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
            item->setText(1, "Summary");
        }
    }
}

GetCalDialog::~GetCalDialog()
{
    delete ui;
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

QVector<uint> GetCalDialog::calibratedChannels(Vna *vna) {
    QVector<uint> result;
    foreach (uint c, vna->channels()) {
        if (vna->channel(c).isCalibrated())
            result << c;
    }
    return result;
}
