#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
#include "Corrections.h"

// RsaToolbox
#include <Keys.h>
#include <LastPath.h>
#include <Vna.h>

// Qt
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(RsaToolbox::Vna *vna, RsaToolbox::Keys *keys, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sourceChanged();
    void checkFilename();
    void generate();

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna     *_vna;

    RsaToolbox::Keys    *_keys;
    RsaToolbox::LastPath _lastPath;

    bool isTwoCalibrations() const;
    void sortCalibrations();
    void checkFrequencyOverlap();

    void blockInputs();
    void unblockInputs();

    bool isValid(Corrections &c1, Corrections &c2);
};

#endif // MAINWINDOW_H
