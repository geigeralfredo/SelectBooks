#include "test03Proj.h"

#include "elapsedtime_Lib.h"
#include "logger_lib.h"

int main()
{
  const QString logType {"RELEASE"};
  const QString proj {"test03"};

    Logger_Lib::init(proj, logType );

    ElapsedTime_Lib timeSpent;

    const QVector<QString> lineVec
        = {{"0001448 0005130725 Biochemistry_Demystified(12).pdf"},
           {"0001448 0005130725 Biochemistry_Demystified (12).pdf"},
           {"0001448 0005130725 Biochemistry_Demystified _(12).pdf"},
           {"0001448 0005130725 Biochemistry_Demystified __(12).pdf"},
           {"0001448 0005130725 Biochemistry_Demystified ___(12).pdf"},
           {"0001448 0005130725 Biochemistry_Demystified ____(12).pdf"},
           {"0001448 0005130725 Biochemistry_Demystified _____(12).pdf"}};

    static const QRegularExpression re(R"((_{0,5}\(\d+\)))");

    int startOffset{0};
    int endOffset{0};
    qsizetype lastPos{0};

    for (int i{0}; i < lineVec.size(); ++i) {
        QRegularExpressionMatch match = re.match(lineVec[i]);

        if (match.hasMatch()) {
            startOffset = match.capturedStart(1);
            endOffset = match.capturedEnd(1);
        }

        qDebug() << "lineVec[" << i << "] = " << lineVec[i] << Qt::endl;
        qDebug() << "startOffset = " << startOffset << Qt::endl;
        qDebug() << "endOffset   = " << endOffset << Qt::endl;

        if (startOffset != 0) {
            QString line = lineVec[i];
            line.remove(startOffset, endOffset - startOffset);
            qInfo() << "Proj("
                    << proj
                    << ")"
                    << "line = "
                    << line
                    << Qt::endl;
        }
    }

    qWarning() << "Warning msg  " << Qt::endl;
    qCritical() << "Critical msg" << Qt::endl;
//    qFatal("ABORTING PROGRAM ");

    Logger_Lib::clean();
}
