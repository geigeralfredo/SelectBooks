#ifndef LOGGER_LIB_H
#define LOGGER_LIB_H

#include "Logger_Lib_global.h"

#pragma once

#include <QDebug>
#include <QFile>
#include <QHash>

class Logger_Lib {

public:
    // Initializes the logger.
    static void init(const QString & projName, const QString & logType = "DEBUG");

    // Cleans up the logger.
    static void clean();

    // The function which handles the logging of text.
    static void messageOutput(QtMsgType type, const QMessageLogContext& context,
                              const QString& msg);

private:
    // The file object where logs are written to.
    static QFile* logFile;

    // Whether the logger has being initialized.
    static bool isInit;

    // The different type of contexts.
    static QHash<QtMsgType, QString> contextNames;

};

#endif // LOGGER_LIB_H
