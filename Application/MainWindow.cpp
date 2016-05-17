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
#include <QDebug>
#include <QDir>
#include <QRegExp>
#include <QRegExpValidator>


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

    QRegExp regex("^[\\w\\{\\}\\[\\]\\(\\)]+([\\w\\{\\}\\[\\]\\(\\)\\s]*[\\w\\{\\}\\[\\]\\(\\)]+)?(\\.cal)?$", Qt::CaseInsensitive);
    ui->filename->setValidator(new QRegExpValidator(regex));

    connect(ui->filename, SIGNAL(editingFinished()),
            this, SLOT(checkFilename()));
    connect(ui->close, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(ui->generate, SIGNAL(clicked()),
            this, SLOT(generate()));
}

MainWindow::~MainWindow()
{
    _vna->local();
    delete ui;
}

void MainWindow::generate() {
    double crossover_Hz = ui->crossover->frequency_Hz();
    if (ui->calibration1->source().isEmpty()) {
        ui->error->showMessage("*Choose first calibration");
        ui->calibration1->setFocus();
        return;
    }
    if (ui->calibration2->source().isEmpty()) {
        ui->error->showMessage("*Choose second calibration");
        ui->calibration2->setFocus();
        return;
    }
    if (ui->crossover->text().isEmpty()) {
        ui->error->showMessage("*Choose crossover frequency");
        ui->crossover->setFocus();
        return;
    }
    if (!ui->filename->hasAcceptableInput()) {
        ui->error->showMessage("*Choose valid filename");
        ui->filename->selectAll();
        ui->filename->setFocus();
        return;
    }

    Calibration cal1;
    cal1.source() = ui->calibration1->source();
    cal1.range().setStop(crossover_Hz);
    Corrections corr1(cal1, _vna);
    if (corr1.frequencies_Hz().isEmpty()) {
        ui->error->showMessage("*Crossover too low");
        ui->crossover->selectAll();
        ui->crossover->setFocus();
        return;
    }

    Calibration cal2;
    cal2.source() = ui->calibration2->source();
    cal2.range().setStart(crossover_Hz);
    Corrections corr2(cal2, _vna);
    if (corr2.frequencies_Hz().isEmpty()) {
        ui->error->showMessage("*Crossover too high");
        ui->crossover->selectAll();
        ui->crossover->setFocus();
        return;
    }

    QVector<Corrections*> corrections;
    corrections << &corr1 << &corr2;

    QString filename = ui->filename->text();
    JoinCalibrations join(corrections, corr1.ports(), _vna, filename);

    if (ui->load->isChecked()) {
        uint c = _vna->createChannel();
        uint d = _vna->createDiagram();
        QString t = _vna->createTrace(c);
        _vna->trace(t).setDiagram(d);

        Calibration cal;
        cal.source().setCalGroup(filename);
        _vna->channel(c).setFrequencies(Corrections(cal, _vna).frequencies_Hz());
        _vna->channel(c).setCalGroup(filename);
    }
    close();
}
void MainWindow::checkFilename() {
    QString filename = ui->filename->text();
    if (!filename.endsWith(".cal", Qt::CaseInsensitive)) {
        ui->filename->setText(filename + ".cal");
    }
}

bool MainWindow::isValid(Corrections &c1, Corrections &c2, double crossover_Hz) {
    if (!c1.isReady()) {
        QString msg = "*Can\'t load cal from %1";
        msg = msg.arg(ui->calibration1->source().displayText());
        ui->error->showMessage(msg);
        ui->calibration1->setFocus();
        return false;
    }
    if (!c2.isReady()) {
        QString msg = "*Can\'t load cal from %1";
        msg = msg.arg(ui->calibration2->source().displayText());
        ui->error->showMessage(msg);
        ui->calibration2->setFocus();
        return false;
    }
    // Prevent 0 point cutoff frequency in validator

}
