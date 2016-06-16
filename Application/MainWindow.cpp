#include "MainWindow.h"
#include "ui_MainWindow.h"


// Project
#include "Calibration.h"
#include "CalSort.h"
#include "Corrections.h"
#include "Settings.h"

// RsaToolbox
#include <Shake.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>


MainWindow::MainWindow(Vna *vna, RsaToolbox::Keys *keys, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vna(vna),
    _pause(vna),
    _keys(keys)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);

    ui->calibration1->setVna(_vna);
    ui->calibration2->setVna(_vna);

    QRegExp regex("^[\\w\\{\\}\\[\\]\\(\\)]+([\\w\\{\\}\\[\\]\\(\\)\\s]*[\\w\\{\\}\\[\\]\\(\\)]+)?(\\.cal)?$", Qt::CaseInsensitive);
    ui->filename->setValidator(new QRegExpValidator(regex));

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

    // Keys
    loadKeys();

    // Error messages
    connect(ui->crossover, SIGNAL(outOfRange(QString)),
            ui->error, SLOT(showMessage(QString)));
    connect(ui->crossover, SIGNAL(outOfRange(QString)),
            this, SLOT(shake()));
}

MainWindow::~MainWindow()
{
    if (_thread.isRunning())
        _thread.terminate();
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
    this->setDisabled(true);

    bool isCrossover = ui->crossover->isEnabled();
    double crossover_Hz;
    if (isCrossover)
        crossover_Hz = ui->crossover->frequency_Hz();

    Calibration cal1;
    cal1.source() = ui->calibration1->source();
    if (isCrossover)
        cal1.range().setStop(crossover_Hz);
    _corr1.reset(new Corrections(cal1, _vna));

    Calibration cal2;
    cal2.source() = ui->calibration2->source();
    if (isCrossover)
        cal2.range().setStart(crossover_Hz);
    _corr2.reset(new Corrections(cal2, _vna));

    if (!isValidInput()) {
        this->setEnabled(true);
        return;
    }

    QString filename = ui->filename->text();

    _join.reset(new JoinCalibrations(_corr1.data(), _corr2.data(), _vna, filename));
    JoinError error;
    if (!_join->isValid(error)) {
        _join.reset();
        displayError(error);
        return;
    }

    saveKeys();
    _join->moveToThread(&_thread);
    connect(_join.data(), SIGNAL(starting()),
            this, SLOT(generateStarted()));
    connect(_join.data(), SIGNAL(progress(int)),
            ui->progressBar, SLOT(setValue(int)));
    connect(_join.data(), SIGNAL(finished()),
            this, SLOT(generateFinished()));
    QMetaObject::invokeMethod(_join.data(),
                              "generate",
                              Qt::QueuedConnection);
    _thread.start();
}
void MainWindow::generateStarted() {
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
}

void MainWindow::generateFinished() {
    _thread.quit();
    _join.reset();
    _corr1.reset();
    _corr2.reset();
    _pause.resume();
    if (ui->load->isChecked()) {
        uint c = _vna->createChannel();
        uint d = _vna->createDiagram();
        QString t = _vna->createTrace(c);
        _vna->trace(t).setDiagram(d);

        const QString filename = ui->filename->text();
        CalibrationSource source(filename);
        _vna->channel(c).setFrequencies(Corrections(source, _vna).frequencies_Hz());
        _vna->channel(c).setCalGroup(filename);
    }

    ui->progressBar->setValue(100);
    this->setEnabled(true);
    QString msg = "\'%1\' generated successfully!";
    msg = msg.arg(ui->filename->text());
    QMessageBox::information(this,
                             APP_NAME,
                             msg);
    ui->progressBar->setVisible(false);

    close();
}

void MainWindow::shake() {
    RsaToolbox::shake(this);
}

void MainWindow::loadKeys() {
    CalibrationSource source;
    if (_keys->exists(CALSOURCE1_KEY)) {
        _keys->get(CALSOURCE1_KEY, source);
        if (Corrections(source, _vna).isReady()) {
            ui->calibration1->setSource(source);
        }
    }
    if (_keys->exists(CALSOURCE2_KEY)) {
        _keys->get(CALSOURCE2_KEY, source);
        if (Corrections(source, _vna).isReady()) {
            ui->calibration2->setSource(source);
        }
    }
    if (_keys->exists(CROSSOVER_KEY)) {
        double crossover;
        _keys->get(CROSSOVER_KEY, crossover);
        ui->crossover->setFrequency(crossover);
    }
    if (_keys->exists(FILENAME_KEY)) {
        QString filename;
        _keys->get(FILENAME_KEY, filename);
        ui->filename->setText(filename);
    }
    if (_keys->exists(LOAD_CAL_KEY)) {
        bool loadCal;
        _keys->get(LOAD_CAL_KEY, loadCal);
        ui->load->setChecked(loadCal);
    }
}
void MainWindow::saveKeys() {
    _keys->set(CALSOURCE1_KEY, ui->calibration1->source());
    _keys->set(CALSOURCE2_KEY, ui->calibration2->source());
    _keys->set(CROSSOVER_KEY, ui->crossover->frequency_Hz());
    _keys->set(FILENAME_KEY, ui->filename->text());
    _keys->set(LOAD_CAL_KEY, ui->load->isChecked());
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
bool MainWindow::isValidInput() {
    if (ui->calibration1->source().isEmpty()) {
        JoinError error;
        error.code = JoinError::Code::Calibration1;
        error.msg = "*Choose first calibration";
        displayError(error);
        return false;
    }
    if (ui->calibration2->source().isEmpty()) {
        JoinError error;
        error.code = JoinError::Code::Calibration2;
        error.msg = "*Choose second calibration";
        displayError(error);
        return false;
    }
    const bool isCrossover = ui->crossover->isEnabled();
    if (isCrossover && ui->crossover->text().isEmpty()) {
        JoinError error;
        error.code = JoinError::Code::FrequencyRange;
        error.msg = "*Choose crossover frequency";
        displayError(error);
        return false;
    }
    if (!ui->filename->hasAcceptableInput()) {
        JoinError error;
        error.code = JoinError::Code::Filename;
        error.msg = "*Choose valid filename";
        displayError(error);
        return false;
        return false;
    }

    return true;
}
void MainWindow::displayError(JoinError error) {
    switch(error.code) {
    case JoinError::Code::Calibration1:
        ui->calibration1->setFocus();
        break;
    case JoinError::Code::Calibration2:
        ui->calibration2->setFocus();
        break;
    case JoinError::Code::FrequencyRange:
        if (ui->crossover->isEnabled()) {
            ui->crossover->setFocus();
            ui->crossover->selectAll();
        }
        break;
    case JoinError::Code::Filename:
        ui->filename->setFocus();
        ui->filename->selectAll();
        break;
    default:
        break;
    }
    ui->error->showMessage(error.msg);
    shake();
}
