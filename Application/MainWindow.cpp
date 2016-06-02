#include "MainWindow.h"
#include "ui_MainWindow.h"


// Project
#include "Calibration.h"
#include "CalSort.h"
#include "Corrections.h"
#include "JoinCalibrations.h"
#include "Settings.h"

// RsaToolbox
#include <Shake.h>
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
    _pause(vna),
    _keys(keys),
    _lastPath(_keys, SAVE_PATH_KEY, QDir::homePath())
{
    ui->setupUi(this);

    ui->calibration1->setVna(_vna);
    ui->calibration2->setVna(_vna);

    QRegExp regex("^[\\w\\{\\}\\[\\]\\(\\)]+([\\w\\{\\}\\[\\]\\(\\)\\s]*[\\w\\{\\}\\[\\]\\(\\)]+)?(\\.cal)?$", Qt::CaseInsensitive);
    ui->filename->setValidator(new QRegExpValidator(regex));

    // Error messages
    connect(ui->crossover, SIGNAL(outOfRange(QString)),
            ui->error, SLOT(showMessage(QString)));
    connect(ui->crossover, SIGNAL(outOfRange(QString)),
            this, SLOT(shake()));

    // Process inputs
    connect(ui->calibration1, SIGNAL(sourceChanged(CalibrationSource)),
            this, SLOT(sourceChanged()));
    connect(ui->calibration2, SIGNAL(sourceChanged(CalibrationSource)),
            this, SLOT(sourceChanged()));
    connect(ui->filename, SIGNAL(editingFinished()),
            this, SLOT(checkFilename()));
    connect(ui->close, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(ui->generate, SIGNAL(clicked()),
            this, SLOT(generate()));
}

MainWindow::~MainWindow()
{
    _pause.resume();
    _vna->local();
    delete ui;
}

void MainWindow::sourceChanged() {
    if (isTwoCalibrations()) {
        sortCalibrations();
        checkFrequencyOverlap();
    }
    else {
        ui->crossover->clearLimits();
        ui->crossover->setEnabled(true);
    }

    if (isCal1())
        updateCal1Summary();
    if (isCal2())
        updateCal2Summary();
}

void MainWindow::generate() {
    if (ui->calibration1->source().isEmpty()) {
        ui->error->showMessage("*Choose first calibration");
        ui->calibration1->setFocus();
        shake();
        return;
    }
    if (ui->calibration2->source().isEmpty()) {
        ui->error->showMessage("*Choose second calibration");
        ui->calibration2->setFocus();
        shake();
        return;
    }
    const bool isCrossover = ui->crossover->isEnabled();
    if (isCrossover && ui->crossover->text().isEmpty()) {
        ui->error->showMessage("*Choose crossover frequency");
        ui->crossover->setFocus();
        shake();
        return;
    }
    if (!ui->filename->hasAcceptableInput()) {
        ui->error->showMessage("*Choose valid filename");
        ui->filename->selectAll();
        ui->filename->setFocus();
        shake();
        return;
    }

    double crossover_Hz;
    if (isCrossover)
        crossover_Hz = ui->crossover->frequency_Hz();

    Calibration cal1;
    cal1.source() = ui->calibration1->source();
    if (isCrossover)
        cal1.range().setStop(crossover_Hz);
    Corrections corr1(cal1, _vna);

    Calibration cal2;
    cal2.source() = ui->calibration2->source();
    if (isCrossover)
        cal2.range().setStart(crossover_Hz);
    Corrections corr2(cal2, _vna);

    if (!isValid(corr1, corr2))
        return;

    QVector<Corrections*> corrections;
    corrections << &corr1 << &corr2;

    QString filename = ui->filename->text();
    QVector<uint> ports = commonPorts(corr1, corr2);
    JoinCalibrations join(corrections, ports, _vna, filename);

    if (ui->load->isChecked()) {
        uint c = _vna->createChannel();
        uint d = _vna->createDiagram();
        QString t = _vna->createTrace(c);
        _vna->trace(t).setDiagram(d);

        CalibrationSource source(filename);
        _vna->channel(c).setFrequencies(Corrections(source, _vna).frequencies_Hz());
        _vna->channel(c).setCalGroup(filename);
    }
    close();
}
void MainWindow::shake() {
    RsaToolbox::shake(this);
}

void MainWindow::checkFilename() {
    QString filename = ui->filename->text();
    if (!filename.endsWith(".cal", Qt::CaseInsensitive)) {
        ui->filename->setText(filename + ".cal");
    }
}

bool MainWindow::isCal1() const {
     return !ui->calibration1->source().isEmpty();
}
bool MainWindow::isCal2() const {
    return !ui->calibration2->source().isEmpty();
}
bool MainWindow::isTwoCalibrations() const {
    return isCal1() && isCal2();
}
void MainWindow::sortCalibrations() {
    // Assumes two valid calibrations
    bool isCal1Blocked = ui->calibration1->blockSignals(true);
    bool isCal2Blocked = ui->calibration2->blockSignals(true);

    CalibrationSource cal1 = ui->calibration1->source();
    CalibrationSource cal2 = ui->calibration2->source();
    CalSort(cal1, cal2, _vna);
    ui->calibration1->setSource(cal1);
    ui->calibration2->setSource(cal2);

    ui->calibration1->blockSignals(isCal1Blocked);
    ui->calibration2->blockSignals(isCal2Blocked);
}
void MainWindow::checkFrequencyOverlap() {
    // Assumes calibrations are sorted
    Corrections c1(ui->calibration1->source(), _vna);
    Corrections c2(ui->calibration2->source(), _vna);
    if (c1.frequencies_Hz().last() < c2.frequencies_Hz().first()) {
        // No overlap
        ui->crossover->setDisabled(true);
        ui->crossover->clearLimits();
        ui->crossover->clear();
        return;
    }
    else {
        double _min = std::max(c1.frequencies_Hz().first(), c2.frequencies_Hz().first());
        double _max = std::min(c1.frequencies_Hz().last(),  c2.frequencies_Hz().last());
        ui->crossover->setMinimum(_min);
        ui->crossover->setMaximum(_max);
        ui->crossover->setEnabled(true);
    }
}
void MainWindow::updateCal1Summary() {
    Corrections corr(ui->calibration1->source(), _vna);
    ui->summary1->setText(corr.displayText());
}
void MainWindow::updateCal2Summary() {
    Corrections corr(ui->calibration2->source(), _vna);
    ui->summary2->setText(corr.displayText());
}

QVector<uint> MainWindow::commonPorts(Corrections &c1, Corrections &c2) {
    QVector<uint> ports1 = c1.ports();
    QVector<uint> ports2 = c2.ports();
    QVector<uint> shared;
    foreach (uint p, ports1) {
        if (ports2.contains(p))
            shared.append(p);
    }
    return shared;
}

bool MainWindow::isValid(Corrections &c1, Corrections &c2) {
    if (!c1.isReady()) {
        QString msg = "*Can\'t load cal from %1";
        msg = msg.arg(ui->calibration1->source().displayText());
        ui->error->showMessage(msg);
        ui->calibration1->setFocus();
        shake();
        return false;
    }
    if (c1.frequencies_Hz().isEmpty()) {
        QString msg = "*No points from cal %1";
        msg = msg.arg(ui->calibration1->source().displayText());
        ui->error->showMessage(msg);
        ui->calibration1->setFocus();
        shake();
        return false;
    }
    if (!c2.isReady()) {
        QString msg = "*Can\'t load cal from %1";
        msg = msg.arg(ui->calibration2->source().displayText());
        ui->error->showMessage(msg);
        ui->calibration2->setFocus();
        shake();
        return false;
    }
    if (c2.frequencies_Hz().isEmpty()) {
        QString msg = "*No points from cal %1";
        msg = msg.arg(ui->calibration2->source().displayText());
        ui->error->showMessage(msg);
        ui->calibration2->setFocus();
        shake();
        return false;
    }
    if (commonPorts(c1, c2).isEmpty()) {
        QString msg = "*Ports do not overlap";
        ui->error->showMessage(msg);
        ui->calibration1->setFocus();
        shake();
        return false;
    }

    return true;
}
