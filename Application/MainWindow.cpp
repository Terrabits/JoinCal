#include "MainWindow.h"
#include "ui_MainWindow.h"


// Project
#include "Calibration.h"
#include "Corrections.h"
#include "JoinCalibrations.h"
#include "Settings.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDir>


MainWindow::MainWindow(Vna *vna, RsaToolbox::Keys *keys, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vna(vna),
    _keys(keys),
    _lastPath(_keys, SAVE_PATH_KEY, QDir::homePath())
{
    ui->setupUi(this);

    ui->calibration1->setVna(_vna);
    ui->calibration2->setVna(_vna);

    connect(ui->close, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(ui->generate, SIGNAL(clicked()),
            this, SLOT(generate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate() {
    double crossover_Hz = ui->crossover->frequency_Hz();

    Calibration cal1;
    cal1.source() = ui->calibration1->source();
    cal1.range().setStop(crossover_Hz);

    Calibration cal2;
    cal2.source() = ui->calibration2->source();
    cal2.range().setStart(crossover_Hz);

    QVector<Calibration> calibrations;
    calibrations << cal1 << cal2;

    QVector<uint> ports = Corrections(cal1, _vna).ports();

    QString filename = ui->filename->text();
    JoinCalibrations join(calibrations,ports, _vna, filename);

    if (ui->load->isChecked()) {
        uint c = _vna->createChannel();
        Calibration cal;
        cal.source().setCalGroup(filename);
        _vna->channel(c).setFrequencies(Corrections(cal, _vna).frequencies_Hz());
        _vna->channel(c).setCalGroup(filename);
        uint d = _vna->createDiagram();
        QString t = _vna->createTrace(c);
        _vna->trace(t).setDiagram(d);
    }
    close();
}
