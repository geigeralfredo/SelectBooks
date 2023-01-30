#ifndef SELECTBOOKS_H
#define SELECTBOOKS_H

#include <iostream>

#ifdef NDEBUG
#define QT_NO_DEBUG_OUTPUT
#define QT_NO_INFO_OUTPUT
#define QT_NO_WARNING_OUTPUT
#endif

#include "selectBooksProj.h"
#include "constants.h"

class SelectBooks
  {
  public:

    SelectBooks( )
      : m_CmdConstructed{ " " },
      m_OutputFile{ " " },
      m_WorkingDirectory{ " " }
      {}

    QVector < QString >
    argumentsParsing( int argc, char *argv [] );

    int
    removingListsOfBooks( const QString & listsOfBooksDir );

    ErrorCodeAndMsg_Lib
    removingFiles( const QString & fileNameToBeRemoved );

    ErrorCodeAndMsg_Lib
    startTask( const QString        & cmdToExecute,
               const QStringList    & arguments );

    ErrorCodeAndMsg_Lib
    realWork( const QString & bookListsDir,
              const QString & subjectsFile );

    ErrorCodeAndMsg_Lib
    constructGrepCommand( const QString & line,
                          const QString & bookListsDir );

    ErrorCodeAndMsg_Lib
    equalSignLine( const QString & FileTotals );

    void
    setCmdConstructed( const QString & newCmdConstructed );

    const QString &
    getCmdConstructed() const;

    void
    setOutputFile( const QString & newOutputFile );

    const QString &
    getOutputFile() const;

    void
    setWorkingDirectory( const QString & newWorkingDirectory );

    const QString &
    getWorkingDirectory() const;

  private:

    QString m_CmdConstructed;
    QString m_OutputFile;
    QString m_WorkingDirectory;

  };

#endif // SELECTBOOKS_H
