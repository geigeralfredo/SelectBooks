#include "selectBooks.h"
#include "openFile_Lib.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                   argumentsParsing
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

QVector < QString >
SelectBooks::argumentsParsing( int argc, char *argv [] )
  {
  Constants constructedLiteral;
  QString   functionName =
    constructedLiteral.timeStamp()
    +
    " - "
    +
    __FUNCTION__
    +
    " "
    +
    QString::number( __LINE__ );

  QCoreApplication      app( argc, argv );
  QCoreApplication::setApplicationName( "SelectBooks" );
  QCoreApplication::setApplicationVersion( "1.0" );

  QCommandLineParser    parser;

  parser.setApplicationDescription( constructedLiteral.helpText() );
  parser.addHelpOption();
  parser.addVersionOption();

  /*--------------------------------------------------------------------
   *               Options with their values
   *--------------------------------------------------------------------*/

  QCommandLineOption    allBooksOption( QStringList() << "a" << "create-let-allbooks",
                                     QCoreApplication::     translate(
                                          "main", "Command to Create the AllBooks file or Let it." ),
                                     QCoreApplication::     translate(
                                          "main", "Create the AllBooks file or Let it." ) );

  QCommandLineOption    bookColletionOption( QStringList() << "b" << "book-collection",
                                          QCoreApplication::translate(
                                               "main", "The directory where all the proper books reside." ),
                                          QCoreApplication::translate(
                                               "main", "File of your book collection." ) );

  QCommandLineOption    listsOfBooksOption( QStringList() << "l" << "lists-of-books",
                                         QCoreApplication:: translate(
                                              "main", "The directory where the lists of books will be written." ),
                                         QCoreApplication:: translate(
                                              "main", "Files of the lists of books found." ) );

  QCommandLineOption    subjectsOption( QStringList() << "s" << "subjects-file",
                                     QCoreApplication:: translate(
                                          "main", "Where the subjects file reside." ),
                                     QCoreApplication:: translate(
                                          "main", "File containing the subject words." ) );

  /*--------------------------------------------------------------------
   *                   Add the options
   *--------------------------------------------------------------------*/

  parser.   addOption(  allBooksOption );
  parser.   addOption(  bookColletionOption );
  parser.   addOption(  listsOfBooksOption );
  parser.   addOption(  subjectsOption );

  /*--------------------------------------------------------------------
   *     process the actual cmdToExecute line arguments given by the user
   *--------------------------------------------------------------------*/

  parser.process( app );

  QVector < QString > returnValuesVec( 5 );

  /*--------------------------------------------------------------------
   *     Verify if parameters were informed
   *--------------------------------------------------------------------*/

  if ( ! parser.parse( QCoreApplication::arguments() ) )
     {
     parser.showHelp();

     qCritical()    << functionName
                    << " -  Error Message = "
                    << constructedLiteral.errorCodeAndErrorMsg( "allParametersMissing_Error" ).getMessage();
     returnValuesVec[ 4 ] = QString::number( constructedLiteral.errorCodeAndErrorMsg( "allParametersMissing_Error" ).getErrorCode() );

     return returnValuesVec;
     }

  /*--------------------------------------------------------------------
   *     Variables to store parameters
   *--------------------------------------------------------------------*/

  QString   createLetAllBooks;
  QString   bookCollectionDir;
  QString   listsOfBooksDir;
  QString   subjectsFileName;

  /*--------------------------------------------------------------------
   *     It is only OK if all 3 parameters were informed
   *--------------------------------------------------------------------*/

  if ( parser.isSet( bookColletionOption ) &&
       parser.isSet( listsOfBooksOption ) &&
       parser.isSet( subjectsOption ) )
     {
     /*--------------------------------------------------------------------
      *     Save parameters
      *--------------------------------------------------------------------*/

     bookCollectionDir  = parser.value( bookColletionOption );
     listsOfBooksDir    = parser.value( listsOfBooksOption );
     subjectsFileName   = parser.value( subjectsOption );
     }
  else
      {
        qCritical() << functionName
                    << " -  You must inform at least 3 parameters.";

        qCritical() << functionName
                    << " -     1            ) [-b] or [--book-collection] - Directory name.";
        qCritical() << functionName
                    << " -     2            ) [-l] or [--lists-of-books]  - Directory name.";
        qCritical() << functionName
                    << " -     3            ) [-s] or [--subjects-file]   - The file name and path.";
        qCritical() << functionName
                    << " -     Optional Parm) [-a] or [--create-let-allbooks] - 'c' or 'l'";
        qCritical() << functionName
                    << " -     program will terminate. ";

      parser.showHelp();

      qCritical()   << functionName
                    << " - Error Message = " << constructedLiteral.errorCodeAndErrorMsg( "allParametersMissing_Error" ).getMessage();
      returnValuesVec[ 4 ] = QString::number( constructedLiteral.errorCodeAndErrorMsg( "allParametersMissing_Error" ).getErrorCode() );

      return returnValuesVec;
      }

  if ( parser.isSet( allBooksOption ) )
     {
     createLetAllBooks = parser.value( allBooksOption );
     qInfo() << functionName << " - createLetAllBooks = " << createLetAllBooks;
     }

  /*--------------------------------------------------------------------
   *                Just for debugging
   *--------------------------------------------------------------------*/

    qInfo() << functionName << " - bookCollectionDir = " << bookCollectionDir;
    qInfo() << functionName << " - listsOfBooksDir   = " << listsOfBooksDir;
    qInfo() << functionName << " - subjectsFile      = " << subjectsFileName;

  returnValuesVec[ 0 ]  = createLetAllBooks;
  returnValuesVec[ 1 ]  = bookCollectionDir;
  returnValuesVec[ 2 ]  = listsOfBooksDir;
  returnValuesVec[ 3 ]  = subjectsFileName;
  returnValuesVec[ 4 ]  = "0";

  return returnValuesVec;
  }     // SelectBooks::argumentsParsing()

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  removingFiles
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ErrorCodeAndMsg_Lib
SelectBooks::removingFiles( const QString & fileNameToBeRemoved )
  {
  Constants constructedLiteral;
  QString   functionName =
    constructedLiteral.timeStamp()
    +
    " - "
    +
    __FUNCTION__
    +
    " "
    +
    QString::number( __LINE__ );

  qInfo() << functionName << "fileNameToBeRemoved = " << fileNameToBeRemoved << Qt::endl;

  ErrorCodeAndMsg_Lib retCodeAndMsgObj;

  if ( fileNameToBeRemoved.isEmpty() )
     {
     qCritical()    << functionName
                    << " - Error Message = "
                    << constructedLiteral.errorCodeAndErrorMsg( "fileNameEmpty_Error" ).getMessage();
     retCodeAndMsgObj = constructedLiteral.errorCodeAndErrorMsg( "fileNameEmpty_Error" );

     return retCodeAndMsgObj;
     }
  else
      {
      QFile file;

      if ( file.exists( fileNameToBeRemoved ) )
         {
         if ( ! file.remove( fileNameToBeRemoved ) )
            {
            qCritical() << functionName
                        << " - Error Message = "
                        << constructedLiteral.errorCodeAndErrorMsg( "fileNotRemoved_Error" ).getMessage();
            retCodeAndMsgObj = constructedLiteral.errorCodeAndErrorMsg( "fileNotRemoved_Error" );

            return retCodeAndMsgObj;
            }
         }
      else
          {
          qCritical()   << functionName
                        << " - Error Message = "
                        << constructedLiteral.errorCodeAndErrorMsg( "file2BeRemovedDoesNotExist_Error" ).getMessage();
          retCodeAndMsgObj = constructedLiteral.errorCodeAndErrorMsg( "file2BeRemovedDoesNotExist_Error" );

          return retCodeAndMsgObj;
          }
      }

  retCodeAndMsgObj.setErrorCode( EXIT_SUCCESS );

  QString msg = functionName + " - The file has been removed. - All is well!!";
  retCodeAndMsgObj.setMessage( msg );
  retCodeAndMsgObj.display();

  return retCodeAndMsgObj;
  }     // SelectBooks::removingFiles()

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  startTask
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ErrorCodeAndMsg_Lib
SelectBooks::startTask( const QString       & cmdToExecute,
                        const QStringList   & arguments )
  {
  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;
  QString               myMessage;
  QString               funcInfo        = Q_FUNC_INFO;

  Constants             constructedLiteral;

  QString               functionName    =
    constructedLiteral.timeStamp()
    +
    " - "
    +
    __FUNCTION__
    +
    " "
    +
    QString::number( __LINE__ );

  /*--------------------------------------------------------------------
   *              All parameters must be informed
   *--------------------------------------------------------------------*/

  if ( cmdToExecute.isEmpty() )
     {
     myMessage = functionName +
                 " - Error: " +
                 funcInfo +
                 " Got empty cmd = " +
                 cmdToExecute +
                 " to execute.";

     retCodeAndMsgObj.setErrorCode( EXIT_FAILURE );
     retCodeAndMsgObj.setMessage( myMessage );
     retCodeAndMsgObj.display();

     return retCodeAndMsgObj;
     }

  if ( arguments.isEmpty() )
     {
     myMessage = functionName +
                 " - Error: " +
                 funcInfo +
                 " All parameters must be informed.";

     retCodeAndMsgObj.setErrorCode( EXIT_FAILURE );
     retCodeAndMsgObj.setMessage( myMessage );
     retCodeAndMsgObj.display();

     return retCodeAndMsgObj;
     }

  /*--------------------------------------------------------------------
   *              Just for debugging
   *--------------------------------------------------------------------*/

    qInfo() << functionName << "cmdToExecute     = " << cmdToExecute << Qt::endl;
    qInfo() << functionName << "arguments        = " << arguments << Qt::endl;
    qInfo() << functionName << "outputFileName   = " << m_OutputFile << Qt::endl;
    qInfo() << functionName << "workingDirectory = " << m_WorkingDirectory << Qt::endl;

  /*--------------------------------------------------------------------
   *              Create QProcess
   *--------------------------------------------------------------------*/

  QProcess process;

  /*--------------------------------------------------------------------
   *       set the Output File and the Working Directory
   *--------------------------------------------------------------------*/

  process.setStandardOutputFile( m_OutputFile, QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate );
  process.setWorkingDirectory( m_WorkingDirectory );

  /*--------------------------------------------------------------------
   *  Make sure we use the system environment
   *--------------------------------------------------------------------*/

  process.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );

  qDebug()  << functionName
            << "==============Before starting process==========="
            << Qt::endl;

  qInfo()   << functionName
            << " - Command to be executed: "
            << cmdToExecute
            << "\n - Arguments: "
            << arguments
            << Qt::endl;

  qDebug() << "process.workingDirectory = " << process.workingDirectory();

  /*--------------------------------------------------------------------
   *            Starting Process
   *--------------------------------------------------------------------*/

  process.setProcessChannelMode( QProcess::MergedChannels );

  process.start( cmdToExecute, arguments );

  if ( process.waitForStarted( -1 ) )
     {
     qDebug()   << functionName
                << "======= After waitForStarted() and Before waitForFinished() ========"
                << Qt::endl;

     if ( process.waitForFinished( -1 ) &&
          process.exitStatus()  == QProcess::NormalExit &&
          process.exitCode()    == 0 &&
          process.state() == QProcess::NotRunning
        )
        {
        qDebug()    << functionName
                    << "=========== After waitForFinished() (Here we are OK) ============="
                    << Qt::endl;

        // =========================================> Here we are OK

        retCodeAndMsgObj.setErrorCode( EXIT_SUCCESS );
        retCodeAndMsgObj.setMessage( functionName + " - Command executed successfully - All is well!!" );
        retCodeAndMsgObj.display();

        return retCodeAndMsgObj;
        }
     }

  // ===============================================> Here we're in error

  qDebug()  << functionName
            << "===================== (Here we're in error) ========================"
            << Qt::endl;

  QString                   exitStatus  =
    ( process.exitStatus() == QProcess::NormalExit ) ? "Normal Exit" : "Crash Exit";

  QString                   processError;

  QProcess::ProcessError    qPError     = process.error();

  switch ( qPError )
         {
             case QProcess::FailedToStart:
             processError   = "\nQProcess::FailedToStart";
             break;

             case QProcess::Crashed:
             processError   = "\nQProcess::Crashed";
             break;

             case QProcess::Timedout:
             processError   = "\nQProcess::Timedout";
             break;

             case QProcess::WriteError:
             processError   = "\nQProcess::WriteError";
             break;

             case QProcess::ReadError:
             processError   = "\nQProcess::ReadError";
             break;

             case QProcess::UnknownError:
             processError   = "\nQProcess::UnknownError";
             break;
         }

  qInfo()   << functionName
            << Qt::endl
            << "=============== Process Output =================="
            << Qt::endl
            << process.readAllStandardOutput().trimmed()
            << Qt::endl
            << "================================================="
            << Qt::endl;

  myMessage = functionName
              + " ============= ERROR - CMD failed to Start ============="
              + "\nCommand = "
              + cmdToExecute
              + "\nProcess errorString = "
              + process.errorString()
              + "\nProcess exitCode = "
              + QString::number( process.exitCode() )
              + "\nProcess errorOccurred = "
              + processError
              + "\nProcess exitStatus = "
              + exitStatus;

  retCodeAndMsgObj.setErrorCode( EXIT_FAILURE );
  retCodeAndMsgObj.setMessage( myMessage );
  retCodeAndMsgObj.display();

  return retCodeAndMsgObj;

  }     // SelectBooks::startTask()

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  realWork Uses:
 * BOOK_LISTS_DIR     -> File to put the lists of books found
 * SUBJECTS_FILE      -> file with subject records
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ErrorCodeAndMsg_Lib
SelectBooks::realWork( const QString    & bookListsDir,
                       const QString    & subjectsFile )
  {
  Constants constructedLiteral;
  QString   functionName =
    constructedLiteral.timeStamp()
    +
    " - "
    +
    __FUNCTION__
    +
    " "
    +
    QString::number( __LINE__ );

    qInfo() << functionName << "bookListsDir = " << bookListsDir << Qt::endl;
    qInfo() << functionName << "subjectsFile = " << subjectsFile << Qt::endl;

  QString               cmdToExecute;
  QStringList           arguments;
  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;

  OpenFile_Lib          mySubjectsFile( subjectsFile,
                                        __FUNCTION__,
                                        "ROTX" );

  QTextStream           mySubjects( mySubjectsFile.getMp_File() );

  while ( ! mySubjects.atEnd() )
        {
        QString line = mySubjects.readLine();

        if ( line.at( 0 ) == constructedLiteral.retrieveLiteral( "numberSign" ) )     // 1st position == "#" means a comment, so discard it
          continue;

        /*--------------------------------------------------------------------
         *              Prepare arguments to 'grep' Process
         *--------------------------------------------------------------------*/

        retCodeAndMsgObj    = constructGrepCommand( line,
                                                    bookListsDir );

        if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
           {
           retCodeAndMsgObj.display();                            // Here we have an error message
           return retCodeAndMsgObj;
           }

        cmdToExecute        = constructedLiteral.retrieveLiteral( "bashProgam" );
        arguments.clear();
        arguments   << constructedLiteral.retrieveLiteral( "argument2Bash" )
                    << getCmdConstructed();             // Here we have the constructed cmdToExecute

        /*--------------------------------------------------------------------
         *              Start 'grep' Process
         *--------------------------------------------------------------------*/

        retCodeAndMsgObj    = startTask( cmdToExecute,
                                         arguments );

        if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
           {
           retCodeAndMsgObj.display();

           return retCodeAndMsgObj;
           }

        /*--------------------------------------------------------------------
         *        Treats the output of GREP process
         *--------------------------------------------------------------------
         *        Open Temp file
         *--------------------------------------------------------------------*/

        QString         tempFile    = constructedLiteral.retrieveLiteral( "tmpFileName" );

        OpenFile_Lib    myTempFile( tempFile,
                                    __FUNCTION__,
                                    "ROTX" );

        QTextStream     myTemp( myTempFile.getMp_File() );

        /*--------------------------------------------------------------------
         *        Open Temp2 file
         *--------------------------------------------------------------------*/

        QString         tempFile2   = constructedLiteral.retrieveLiteral( "tmpFileName2" );

        OpenFile_Lib    myTempFile2( tempFile2,
                                     __FUNCTION__,
                                     "WOTX" );

        QTextStream     myTemp2( myTempFile2.getMp_File() );

        /*--------------------------------------------------------------------
         *    Add double quotes at beginning and at the end of each line
         *--------------------------------------------------------------------*/

        while ( ! myTemp.atEnd() )
              {
              myTemp2 << "\"" << myTemp.readLine() << "\"" << Qt::endl;
              }

        /*--------------------------------------------------------------------
         *           Construct bookListFileName using
         *              - bookListsDir
         *              - MiddleName plus
         *              - Suffix and Extension
         *--------------------------------------------------------------------*/

        QString MiddleName                  = constructedLiteral.retrieveLiteral( "emptyString" );

        /*--------------------------------------------------------------------
         *            Construct the MiddleName variable
         *--------------------------------------------------------------------*/

        for ( int i = 0; i < line.size(); ++i )
            {
            if ( line.at( i ) == constructedLiteral.retrieveLiteral( "singleSpace" ) )
              continue;

            MiddleName += line.at( i );
            }

        QString         bookListFileName    = bookListsDir + MiddleName + constructedLiteral.retrieveLiteral( "listTxt" );

        /*--------------------------------------------------------------------
         *            Construct filename for the Totals
         *--------------------------------------------------------------------*/

        QString         FileTotals          = bookListsDir + constructedLiteral.retrieveLiteral( "selectedBooksTotalsFile" );

        OpenFile_Lib    myTotalsFile( FileTotals,
                                      __FUNCTION__,
                                      "WOAPTX" );

        QTextStream     myFileForTheTotals( myTotalsFile.getMp_File() );

        /*--------------------------------------------------------------------
         *            Display the searched word and its size
         *--------------------------------------------------------------------*/

        myFileForTheTotals << "Line size = " << line.size() << Qt::endl;
        myFileForTheTotals << "Word Searched = ";

        for ( int i = 0; i < line.size(); ++i )
            {
            myFileForTheTotals << line.at( i );
            }

        myFileForTheTotals << Qt::endl;

        /*--------------------------------------------------------------------
         *            Display File Name For The Totals
         *--------------------------------------------------------------------*/

        myFileForTheTotals << "File Name = " << bookListFileName << Qt::endl;

        /*--------------------------------------------------------------------
         *              Prepare 'SORT' cmd
         *--------------------------------------------------------------------*/

        QString wholeSortCmd = constructedLiteral.retrieveLiteral( "sortCmd" ) +
                               " " +
                               constructedLiteral.retrieveLiteral( "tmpFileName2" );

        cmdToExecute        = constructedLiteral.retrieveLiteral( "bashProgam" );
        arguments.clear();
        arguments   << constructedLiteral.retrieveLiteral( "argument2Bash" )
                    << wholeSortCmd;

        setOutputFile( bookListFileName );

        /*--------------------------------------------------------------------
         *              Start 'SORT' Process
         *--------------------------------------------------------------------*/

        retCodeAndMsgObj    = SelectBooks::startTask( cmdToExecute,
                                                      arguments );

        if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
           {
           retCodeAndMsgObj.display();

           return retCodeAndMsgObj;
           }

        /*--------------------------------------------------------------------
         *              Line full of '='
         *--------------------------------------------------------------------*/

        myFileForTheTotals << constructedLiteral.retrieveLiteral( "equalSignLine" ) << Qt::endl;

        /*--------------------------------------------------------------------
         *        Display books found Totals of a single selection
         *--------------------------------------------------------------------*/

        OpenFile_Lib    listOfBooks( bookListFileName,
                                     __FUNCTION__,
                                     "ROTX" );

        QTextStream     myListOfBooks( listOfBooks.getMp_File() );
        int             lineCounter = 0;

        while ( ! myListOfBooks.atEnd() )
              {
              myListOfBooks.readLine();
              ++lineCounter;
              }

        myFileForTheTotals  << "Number of Books found = "
                            << lineCounter
                            << Qt::endl;

        /*--------------------------------------------------------------------
         *              Prepare another Line full of '='
         *--------------------------------------------------------------------*/

        myFileForTheTotals << constructedLiteral.retrieveLiteral( "equalSignLine" ) << Qt::endl;

        /*--------------------------------------------------------------------
         *              Removing temp and temp2 files
         *--------------------------------------------------------------------*/

        retCodeAndMsgObj    = SelectBooks::removingFiles( constructedLiteral.retrieveLiteral( "tmpFileName" ) );

        if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
          return retCodeAndMsgObj;

        retCodeAndMsgObj    = SelectBooks::removingFiles( constructedLiteral.retrieveLiteral( "tmpFileName2" ) );

        if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
          return retCodeAndMsgObj;
        }

  retCodeAndMsgObj.setErrorCode( EXIT_SUCCESS );
  retCodeAndMsgObj.setMessage( functionName + " - All is well!!" );

  return retCodeAndMsgObj;
  }         // SelectBooks::realWork()

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  constructGrepCommand
 * Concatenate grep + pipes + file writing cmdToExecutes conditioned by
 * the words that appear in the subjects file records
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ErrorCodeAndMsg_Lib
SelectBooks::constructGrepCommand( const QString    & line,
                                   const QString    & bookListsDir )
  {
  Constants constructedLiteral;
  QString   functionName =
    constructedLiteral.timeStamp()
    +
    " - "
    +
    __FUNCTION__
    +
    " "
    +
    QString::number( __LINE__ );

    qInfo() << functionName << "line = " << line << Qt::endl;
    qInfo() << functionName << "bookListsDir = " << bookListsDir << Qt::endl;

  QString   grepCommand = constructedLiteral.retrieveLiteral( "emptyString" );
  QString   makeName    = constructedLiteral.retrieveLiteral( "emptyString" );

  /*--------------------------------------------------------------------
   *              EgrepType Enum Type
   *              eGrepiv: Invert the sense of the matching
   *              eGrepEi: Select whole words only
   *--------------------------------------------------------------------*/

  enum class EgrepType
    {
    eGrepiv,                // (-v) Invert the sense of the matching
    eGrepEi,                // Select WHOLE WORDs only
    eGrepi                  // normal selection
    };

  EgrepType eGrepCmd;
  bool      wasGrepCmdSet = false;

  for ( int i = 0; i < line.size(); ++i )
      {
      if ( line.at( i ) != constructedLiteral.retrieveLiteral( "singleSpace" ) )
         {
         qInfo() << "constructGrepCommand - CHAR = " << line.at( i ) << Qt::endl;

         /*--------------------------------------------------------------------
          * Select only the lines that DO NOT contain the word that follows it
          *--------------------------------------------------------------------*/

         if ( line.at( i ) == constructedLiteral.retrieveLiteral( "invertedMatchSymbol" ) )
            {
            qInfo() << "constructGrepCommand - (~~~~~~~~) Inside (Invert the sense of matching) ==============="
                    << Qt::endl;

            eGrepCmd        = EgrepType::eGrepiv;                       // (-v) Invert the sense of matching
            wasGrepCmdSet   = true;
            }
         else
             {
             /*--------------------------------------------------------------------
              * Select only WHOLE WORDs as the one that follows the symbol
              *--------------------------------------------------------------------*/

             if ( line.at( i ) == constructedLiteral.retrieveLiteral( "wholeWordSymbol" ) )
                {
                qInfo() << "constructGrepCommand - (^^^^^^^^) Inside (WHOLE WORDs only) ==============="
                        << Qt::endl;
                eGrepCmd        = EgrepType::eGrepEi;
                wasGrepCmdSet   = true;
                }
             else
                 {
                 makeName += line.at( i );

                 if ( ! wasGrepCmdSet )
                    {
                    qInfo() << "constructGrepCommand - ========= Inside (NORMAL SELECTION) ==============="
                            << Qt::endl;
                    eGrepCmd = EgrepType::eGrepi;
                    }
                 }
             }
         }
      else
          {
          wasGrepCmdSet = false;

          qInfo()   << "constructGrepCommand - WORD(" << makeName << ")"
                    << Qt::endl;

          switch ( eGrepCmd )
                 {
                     case EgrepType::eGrepiv:                               // (-v) Invert the sense of the matching
                     grepCommand    += constructedLiteral.retrieveLiteral( "bashCmdPipe" ) +
                                       constructedLiteral.retrieveLiteral( "grepivCmd" ) +
                                       " " +
                                       makeName;
                     makeName.clear();
                     qInfo() << "constructGrepCommand - Inverted grep cmd         = " << grepCommand << Qt::endl;
                     break;

                     case EgrepType::eGrepEi:                               // Select whole words only
                     grepCommand    += constructedLiteral.retrieveLiteral( "bashCmdPipe" ) +
                                       constructedLiteral.retrieveLiteral( "grepEiStartCmd" ) +
                                       makeName +
                                       constructedLiteral.retrieveLiteral( "grepEiEndCmd" );
                     makeName.clear();
                     qInfo() << "constructGrepCommand - whole words only grep cmd = " << grepCommand << Qt::endl;
                     break;

                     case EgrepType::eGrepi:                                // Normal selection by default
                     default:
                     grepCommand    += constructedLiteral.retrieveLiteral( "bashCmdPipe" ) +
                                       constructedLiteral.retrieveLiteral( "grepiCmd" ) +
                                       " " +
                                       makeName;
                     makeName.clear();
                     qInfo() << "constructGrepCommand - Normal selection grep cmd = " << grepCommand << Qt::endl;
                 }
          }
      }

  qInfo()   << "constructGrepCommand - WORD(" << makeName << ")"
            << Qt::endl;

  switch ( eGrepCmd )
         {
             case EgrepType::eGrepiv:                    // (-v) Invert the sense of the matching
             grepCommand    += constructedLiteral.retrieveLiteral( "bashCmdPipe" ) +
                               constructedLiteral.retrieveLiteral( "grepivCmd" ) +
                               " " +
                               makeName;
             makeName.clear();
             qInfo() << "constructGrepCommand - Inverted grep cmd         = " << grepCommand << Qt::endl;
             break;

             case EgrepType::eGrepEi:                    // Select whole words only
             grepCommand    += constructedLiteral.retrieveLiteral( "bashCmdPipe" ) +
                               constructedLiteral.retrieveLiteral( "grepEiStartCmd" ) +
                               makeName +
                               constructedLiteral.retrieveLiteral( "grepEiEndCmd" );
             makeName.clear();
             qInfo() << "constructGrepCommand - whole words only grep cmd = " << grepCommand << Qt::endl;
             break;

             case EgrepType::eGrepi:                     // Normal selection by default
             default:
             grepCommand    += constructedLiteral.retrieveLiteral( "bashCmdPipe" ) +
                               constructedLiteral.retrieveLiteral( "grepiCmd" ) +
                               " " +
                               makeName;
             makeName.clear();
             qInfo() << "constructGrepCommand - Normal selection grep cmd = " << grepCommand << Qt::endl;
         }

  m_OutputFile = constructedLiteral.retrieveLiteral( "tmpFileName" );

  QString               allBooksFileName = bookListsDir + constructedLiteral.retrieveLiteral( "allBooksTXTFile" );
  QFile                 allBooksTxtFile( allBooksFileName );

  QString               message;
  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;

  if ( ! allBooksTxtFile.exists() )
     {
     message = functionName + " - The file " + constructedLiteral.retrieveLiteral( "allBooksTXTFile" ) + " does not exist";
     retCodeAndMsgObj.setErrorCode( EXIT_FAILURE );
     retCodeAndMsgObj.setMessage( message );

     return retCodeAndMsgObj;                             // Here we have an error
     }          // if ( ! allBooksTxtFile.exists())

  grepCommand = constructedLiteral.retrieveLiteral( "catCmd" ) + allBooksFileName + grepCommand;

  qInfo()   << functionName
            << " - Constructed grepCommand = "
            << grepCommand
            << Qt::endl;

  SelectBooks::setCmdConstructed( grepCommand );

  retCodeAndMsgObj.setErrorCode( EXIT_SUCCESS );
  retCodeAndMsgObj.setMessage( grepCommand );

  return retCodeAndMsgObj;                                // Here we're OK
  }             // SelectBooks::constructGrepCommand()

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  equalSignLine
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

ErrorCodeAndMsg_Lib
SelectBooks::equalSignLine( const QString & FileTotals )
  {
  Constants constructedLiteral;
  QString   functionName =
    constructedLiteral.timeStamp()
    +
    " - "
    +
    __FUNCTION__
    +
    " "
    +
    QString::number( __LINE__ );

  qInfo() << functionName << "FileTotals = " << FileTotals << Qt::endl;

  QString               cmdToExecute;
  QStringList           arguments;
  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;

  /*--------------------------------------------------------------------
   *              Prepare a Line full of '='
   *--------------------------------------------------------------------*/

  cmdToExecute      = constructedLiteral.retrieveLiteral( "bashProgam" );
  arguments.clear();
  arguments << constructedLiteral.retrieveLiteral( "argument2Bash" )
            << constructedLiteral.bashCmdAppendTo( FileTotals );

  /*--------------------------------------------------------------------
   *              Print the line
   *--------------------------------------------------------------------*/

  retCodeAndMsgObj  = SelectBooks::startTask( cmdToExecute,
                                              arguments );

  if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
    retCodeAndMsgObj.display();

  return retCodeAndMsgObj;
  }             // SelectBooks::equalSignLine()

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  setCmdConstructed
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

void
SelectBooks::setCmdConstructed( const QString & newCmdConstructed )
  {
  m_CmdConstructed = newCmdConstructed;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  getCmdConstructed
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const QString &
SelectBooks::getCmdConstructed() const
  {
  return m_CmdConstructed;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  setOutputFile
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
void
SelectBooks::setOutputFile( const QString & newOutputFile )
  {
  m_OutputFile = newOutputFile;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  getOutputFile
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const QString &
SelectBooks::getOutputFile() const
  {
  return m_OutputFile;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  setWorkingDirectory
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
void
SelectBooks::setWorkingDirectory( const QString & newWorkingDirectory )
  {
  m_WorkingDirectory = newWorkingDirectory;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                  getWorkingDirectory
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const QString &
SelectBooks::getWorkingDirectory() const
  {
  return m_WorkingDirectory;
  }
