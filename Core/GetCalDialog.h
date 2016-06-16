#ifndef GETCALDIALOG_H
#define GETCALDIALOG_H


// Project
#include "CalibrationSource.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QDialog>
#include <QStringList>
#include <QVector>


namespace Ui {
class GetCalDialog;
}

class GetCalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetCalDialog(RsaToolbox::Vna *vna, QWidget *parent = 0);
    ~GetCalDialog();

    void setDefault(CalibrationSource source);
    CalibrationSource selectedCal() const;

public slots:
    virtual void accept();

private slots:
    void shake();

private:
    Ui::GetCalDialog *ui;

    static QVector<uint> calibratedChannels(RsaToolbox::Vna *vna);
    QStringList _calGroups;
    QVector<uint> _channels;
};

#endif // GETCALDIALOG_H
