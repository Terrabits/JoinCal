#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
#include "Corrections.h"
#include "JoinCalibrations.h"
#include "JoinError.h"

// RsaToolbox
#include <Keys.h>
#include <VnaPauseSweeps.h>
#include <Vna.h>

// Qt
#include <QMainWindow>
#include <QScopedPointer>
#include <QThread>


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
    void generateStarted();
    void generateFinished();

    void shake();

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna     *_vna;
    RsaToolbox::VnaPauseSweeps _pause;

    RsaToolbox::Keys    *_keys;
    void loadKeys();
    void saveKeys();

    bool isCal1() const;
    bool isCal2() const;
    bool isTwoCalibrations() const;
    void sortCalibrations();
    void checkFrequencyOverlap();
    void updateCal1Summary();
    void updateCal2Summary();
    bool isValidInput();
    void displayError(JoinError error);

    QThread _thread;
    QScopedPointer<Corrections> _corr1;
    QScopedPointer<Corrections> _corr2;
    QScopedPointer<JoinCalibrations> _join;
};

#endif // MAINWINDOW_H
