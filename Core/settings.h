#ifndef SETTINGS_H
#define SETTINGS_H


// RsaToolbox
#include <Definitions.h>
#include <General.h>
#include <GenericBus.h>


// Application settings
const QString APP_NAME            = "R&S Join Cal";
const QString APP_VERSION         = "1.1";
const QString MANUFACTURER_FOLDER = "Rohde-Schwarz";
const QString APP_FOLDER          = "Join Cal";
const QString APP_DESCRIPTION     = "Combine corrections from two calibrations with different frequency ranges";
const QString CONTACT_INFO        = "<html><head/><body><p>Nick Lalic<br/>VNA Software Developer<br/>Half Moon Bay, CA USA<br/>+1 424 200 2846<br/>nick.lalic@rsa.rohde-schwarz.com<br/><a href=\"http://vna.rs-us.net\"><span style=\"text-decoration: underline; color:#0000ff;\">http://vna.rs-us.net</span></a></p></body></html>";
const QString JSON_UPDATE_URL     = "https://vna.rs-us.net/applications/join_cal.json";

#ifdef DEBUG_MODE
// Debug program data
const QDir    dataDir(SOURCE_DIR);
const QString LOG_FILENAME = dataDir.filePath("_Debug/Log.txt");
const QString KEY_PATH     = dataDir.filePath("_Debug/Keys");

// Debug Connection settings
const RsaToolbox::ConnectionType CONNECTION_TYPE = RsaToolbox::ConnectionType::VisaTcpSocketConnection;
const QString                    ADDRESS         = "127.0.0.1::5025";

#else
// Release program data
const QDir    dataDir(RsaToolbox::GetAppDataPath(MANUFACTURER_FOLDER, APP_FOLDER));
const QString LOG_FILENAME = dataDir.filePath("R&S Join Cal Log.txt");
const QString KEY_PATH     = dataDir.filePath("Settings");

// Debug Connection settings
const RsaToolbox::ConnectionType CONNECTION_TYPE = RsaToolbox::ConnectionType::VisaTcpSocketConnection;
const QString                    ADDRESS         = "127.0.0.1::5025";
#endif

// Keys
const QString CALSOURCE1_KEY = "CALSOURCE1";
const QString CALSOURCE2_KEY = "CALSOURCE2";
const QString CROSSOVER_KEY  = "CROSSOVER";
const QString FILENAME_KEY   = "FILENAME";
const QString LOAD_CAL_KEY   = "LOADCAL";

#endif // SETTINGS_H

