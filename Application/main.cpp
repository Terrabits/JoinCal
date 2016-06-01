

// Project
#include "MainWindow.h"
#include "Settings.h"

// RsaToolbox
#include <About.h>
#include <Keys.h>
#include <Log.h>
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QMessageBox>
#include <QScopedPointer>


// About menu
bool isAboutMenu(int argc, char *argv[]);

// Connection
bool isNoConnection(Vna &vna);
bool isUnknownModel(Vna &vna);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (isAboutMenu(argc, argv))
        return 0;

    Log log(LOG_FILENAME, APP_NAME, APP_VERSION);
    log.printHeader();

    Vna vna(ConnectionType::VisaTcpSocketConnection, "127.0.0.1::5025");
    if (isNoConnection(vna)) {
        vna.useLog(&log);
        vna.printInfo();
        return 0;
    }

    vna.settings().setEmulationOff();
    vna.useLog(&log);
    vna.printInfo();
    if (isUnknownModel(vna))
        return 0;

    Keys keys(KEY_PATH);

    QString windowTitle = "%1 %2";
    windowTitle         = windowTitle.arg(APP_NAME);
    windowTitle         = windowTitle.arg(APP_VERSION);

    QScopedPointer<MainWindow> w(new MainWindow(&vna, &keys));
    w->setWindowTitle(windowTitle);
    w->show();
    int result = a.exec();

    w.reset();
    return result;
}


bool isAboutMenu(int argc, char *argv[]) {
    if (argc != 2)
        return false;

    QString arg(argv[1]);
    arg = arg.trimmed().toUpper();
    if (arg == "-ABOUT" || arg == "--ABOUT") {
        Q_INIT_RESOURCE(AboutResources);
        About about;
        about.setAppName(APP_NAME);
        about.setVersion(APP_VERSION);
        about.setDescription(APP_DESCRIPTION);
        about.setContactInfo(CONTACT_INFO);
        about.exec();
        return true;
    }

    return false;
}
bool isNoConnection(Vna &vna) {
    if (vna.isConnected() && !vna.idString().isEmpty())
        return false;

    QString msg = "Instrument not found.\n";
    msg += "Please run this application on the instrument.";
    QMessageBox::critical(NULL,
                          APP_NAME,
                          msg);
    vna.print(msg);
    return true;
}
bool isUnknownModel(Vna &vna) {
    if (vna.properties().isKnownModel())
        return false;

    QString msg = "Instrument not recognized.\n";
    msg += "Please use %1 with a Rohde & Schwarz VNA";
    msg = msg.arg(APP_NAME);
    QMessageBox::critical(NULL,
                          APP_NAME,
                          msg);
    vna.print(msg);
    return true;
}
