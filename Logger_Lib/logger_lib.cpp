#include "logger_lib.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QObject>
#include <QFileInfo>
#include <QDirIterator>

QFile* Logger_Lib::logFile = Q_NULLPTR;

bool Logger_Lib::isInit = false;

QHash<QtMsgType, QString> Logger_Lib::contextNames = {
    {QtMsgType::QtDebugMsg,     " Debug  "},
    {QtMsgType::QtInfoMsg,	"==INFO=="},
    {QtMsgType::QtWarningMsg,	"Warning "},
    {QtMsgType::QtCriticalMsg,	"Critical"},
    {QtMsgType::QtFatalMsg,	" Fatal  "}
};

/******************************************************************
 *          init() function act as a constructor
 ******************************************************************/

void Logger_Lib::init(const QString & projName, const QString & logType)
{
  if (logType == "DEBUG")
    return;

    if (isInit) {
        return;
    }

/******************************************************************
 *          Variable definitions
 ******************************************************************/

    QDir myDir = QDir::home();
    QString home = myDir.homePath();
    QString pathToDir {home + "/Log/"};
    QString fileNameAndAsterisk {projName + "*.log"};

    QString logFileName{pathToDir +
                        projName +
                        QDateTime::currentDateTime().toString("_yyyy-MM-dd_hh:mm:ss") +
                        ".log"};

 /******************************************************************
 *          Deleting files with same name
 ******************************************************************/

    QDir dir( pathToDir, fileNameAndAsterisk );
    for (const QString & filename: dir.entryList())
    {
        dir.remove(filename);
    }

 /******************************************************************
 *          Creating the Log file
 ******************************************************************/

    logFile = new QFile;
    logFile->setFileName(logFileName);
    logFile->open( QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text );

 /******************************************************************
 *  Installing Message Handler to Redirect logs to messageOutput()
 ******************************************************************/

    qInstallMessageHandler(Logger_Lib::messageOutput);

    Logger_Lib::isInit = true;
}

/******************************************************************
 *          clean() function act as a destructor
 ******************************************************************/

void Logger_Lib::clean() {
    if (logFile != Q_NULLPTR) {
        logFile->close();
        delete logFile;
        logFile = Q_NULLPTR;
        qInstallMessageHandler(0);
    }
}

/******************************************************************
 *      Message Handler function that is installed
 ******************************************************************/

void Logger_Lib::messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg) {

  static bool isItFirstTime = true;

    QString log = QString(" %1 | %2 | %3 | %4 | %5 ").
                  arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss")).
                  arg(Logger_Lib::contextNames.value(type)).
                  arg(context.line).
                  arg(QString(context.file).
                      section('/', -1)).			// File name without file path
                  arg(msg);

    /****************************************************************************
     *  I don't know why in the 1st line of the LOG it prints with one byte less
     *  so I'm adding a blank byte to correct it
     ****************************************************************************/

    if (isItFirstTime)
      {
        logFile->write(" " + log.toLocal8Bit());
        isItFirstTime = false;
      }
    else
      logFile->write(log.toLocal8Bit());

    logFile->flush();

    /******************************************************************
     *      Every message goes to the LOG file but
     *      the "Critical" and " Fatal  " msgs also go to stdout
     ******************************************************************/

    if (Logger_Lib::contextNames.value(type) == "Critical" ||
        Logger_Lib::contextNames.value(type) == " Fatal  ")
      qInfo() << log << Qt::endl;
}
