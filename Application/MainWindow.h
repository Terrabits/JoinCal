#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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
    void generate();

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna     *_vna;

    RsaToolbox::Keys    *_keys;
    RsaToolbox::LastPath _lastPath;
};

#endif // MAINWINDOW_H
