/**************************************************************************
 * SelectBooks
 *
 *        Mandatory Parameters:
 *        '-b' (or '--book-colletion')
 *        '-s' (or '--subjects-file')
 *        '-l' (or '--lists-of-books')
 *
 *        Optional Parameter:
 *        '-c' (or '--create-allbooks')
 *
 *        This application reads just the file names of all books
 *        that are present in the directory informed by the
 *        '-b' (or '--book-colletion') parameter.
 *
 *        Then, selects the books that have in their names or in their
 *        directory names the words that are present in the subjects file.
 *        '-s' (or '--subjects-file') parameter inform the subjects file name.
 *
 *        Subjects are words or group of words that denote a subject
 *        that will be searched for in the collection of books.
 *
 *        '-l' (or '--lists-of-books') inform the lists of books directory,
 *        are the books found in the book collection that have at least one of the words
 *        present in the subjects file.
 *
 *        The file names of the lists are a concatenation of the words of subjects
 *        plus the suffix '.list' and the extension '.txt'.
 *
 *        The parameter '-c' (or '--create-allbooks') is used to speed up the book selection process
 *        the value 'Y' or 'y' is: CREATE the AllBooks file (SLOWER)
 *        the value 'N' or 'n' is: DO NOT CREATE a new version of the file, use the old one(FASTER).
 ***************************************************************************/

#include "selectBooks.h"
#include "openFile_Lib.h"
#include "elapsedtime_Lib.h"
#include "logger_lib.h"

/********************************************************
*          returns the Project Name
********************************************************/

QString
selectBooks( const QString & project )
  {
  if ( project == "main" )
    return ( __FUNCTION__ );
  else
    return project;
  }

/********************************************************
*          returns the Project Name
********************************************************/

int
main( int argc, char *argv [] )
  {
    const QString logType = "RELEASE";
    QString proj {"selectBooks"};

    Logger_Lib::init(proj, logType );

  ElapsedTime_Lib       timeIntervalObj;

  Constants             constructedLiteral;
  SelectBooks           books2Select;
  QString               functionName    = constructedLiteral.timeStamp() + " - " + selectBooks( __FUNCTION__ ) + " " + QString::number( __LINE__ );

  // QCoreApplication      app( argc, argv );

    qInfo() << functionName << "================ B E G I N N I N G =============================" << Qt::endl;

  /*--------------------------------------------------------------------
   *       Parsing received arguments
   *--------------------------------------------------------------------*/

  QVector < QString >   parmsVector     = books2Select.argumentsParsing( argc, argv );

    qInfo() << functionName << "================ A F T E R =====================================" << Qt::endl;

    qInfo() << functionName << parmsVector.length() << Qt::endl;

    qInfo() << functionName << parmsVector[ 0 ] << Qt::endl;
    qInfo() << functionName << parmsVector[ 1 ] << Qt::endl;
    qInfo() << functionName << parmsVector[ 2 ] << Qt::endl;
    qInfo() << functionName << parmsVector[ 3 ] << Qt::endl;
    qInfo() << functionName << parmsVector[ 4 ] << Qt::endl;

  if ( parmsVector[ 4 ] != "0" )
     {
     qInfo() << functionName << "============= Inside IF ========================================" << Qt::endl;

     return parmsVector[ 4 ].toInt();
     }

  /*--------------------------------------------------------------------
   *      create error code and error message object
   *--------------------------------------------------------------------*/

  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;

  /*--------------------------------------------------------------------
   *      setting variables with received parameters
   *--------------------------------------------------------------------*/

  QString               createAllBooks    ( parmsVector[ 0 ] );
  QString               BookCollectionDir ( parmsVector[ 1 ] );
  QString               ListsOfBooksDir   ( parmsVector[ 2 ] );
  QString               SubjectsFile      ( parmsVector[ 3 ] );

  QFileInfo             fi_BookCollectionDir ( BookCollectionDir );
  QFileInfo             fi_ListsOfBooksDir   ( ListsOfBooksDir );
  QFileInfo             fi_SubjectsFile      ( SubjectsFile );

  /*--------------------------------------------------------------------
   *       Verify if Book Collection Directory exists
   *--------------------------------------------------------------------*/

  QDir                  tempDir;
  tempDir.setFilter( QDir::Files | QDir::NoSymLinks | QDir::Readable | QDir::Writable );

  qInfo() << functionName << "Book Collection Directory = " << fi_BookCollectionDir.absoluteFilePath() << Qt::endl;

  if ( ! tempDir.exists( fi_BookCollectionDir.absoluteFilePath() ) )
     {
        qCritical() << functionName
                    << " - Error Message = "
                    << constructedLiteral.errorCodeAndErrorMsg( "dirBookCollection_Error" ).getMessage()
                    << Qt::endl;
        qCritical() << functionName
                    << " - Program will terminate. "
                    << Qt::endl;

     return constructedLiteral.errorCodeAndErrorMsg( "dirBookCollection_Error" ).getErrorCode();
     }

  /*--------------------------------------------------------------------
   *      if lists of books Directory doesn't exist create it
   *      if exists remove all book lists of a previous run
   *--------------------------------------------------------------------*/

  qInfo() << "Lists of books Dir = " << fi_ListsOfBooksDir.absoluteFilePath() << Qt::endl;

  if ( ! tempDir.exists( fi_ListsOfBooksDir.absoluteFilePath() ) )
     {
     qInfo()    << functionName
                << "Lists of books Dir does not exist, it will be created."
                << Qt::endl;

     tempDir.mkdir( fi_ListsOfBooksDir.absoluteFilePath() );
     }
  else
      {
      QDirIterator it( fi_ListsOfBooksDir.absoluteFilePath(), { constructedLiteral.retrieveLiteral( "asteriskListTxt" ) } );

      while ( it.hasNext() )
            {
            QFile( it.next() ).remove();                         // Remove all book lists of a previous run
            }
      }

  /*--------------------------------------------------------------------
   *       Verify if Subjects File exists
   *--------------------------------------------------------------------*/

  qInfo() << "Subjects File = " << fi_SubjectsFile.absoluteFilePath() << Qt::endl;

  if ( ! tempDir.exists( fi_SubjectsFile.absoluteFilePath() ) )
     {
        qCritical() << functionName
                    << " - Error Message = "
                    << constructedLiteral.errorCodeAndErrorMsg( "subjectsFile_Error" ).getMessage()
                    << Qt::endl;
        qCritical() << functionName
                    << " - Program will terminate. "
                    << Qt::endl;

     return constructedLiteral.errorCodeAndErrorMsg( "subjectsFile_Error" ).getErrorCode();
     }

  /*--------------------------------------------------------------------
   *       if selectedBooksTotals.txt exists, remove it
   *--------------------------------------------------------------------*/

  QString   bookTotalsFileName = ListsOfBooksDir + constructedLiteral.retrieveLiteral( "selectedBooksTotalsFile" );
  QFile     bookTotalsFile( bookTotalsFileName );

  if ( bookTotalsFile.exists() )
     {
     qInfo()    << functionName
                << " - File with book totals = "
                << bookTotalsFileName
                << " will be removed."
                << Qt::endl;

     retCodeAndMsgObj = books2Select.removingFiles( bookTotalsFileName );

     if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
       return retCodeAndMsgObj.getErrorCode();
     }

  /*--------------------------------------------------------------------
   *              Variables to be used in starting Processes
   *--------------------------------------------------------------------*/

  QString       cmdToExecute;
  QStringList   arguments;

  /*--------------------------------------------------------------------
   *        check the existence of constructedLiteral.allBooksTxtFile()
   *        if not, create it with the "find" cmdToExecute
   *        manipulating just the names of the books and
   *        respective directories where they reside
   *--------------------------------------------------------------------*/

  QString       allBooksFileName        = fi_ListsOfBooksDir.absoluteFilePath() + constructedLiteral.retrieveLiteral( "allBooksTXTFile" );
  QFile         allBooksTxtFile( allBooksFileName );

  books2Select.setWorkingDirectory( fi_ListsOfBooksDir.absoluteFilePath() );

  /*--------------------------------------------------------------------
   *       if AllBooks.txt exists, remove it so we
   *       may create the latest version of the book set
   *--------------------------------------------------------------------*/

  QString       createAllBooksLowerCase = createAllBooks.toLower();

  if ( allBooksTxtFile.exists() &&
       createAllBooksLowerCase != 'y' )           // do not create
    goto goAway;

  if ( ( ! allBooksTxtFile.exists() ) ||
       ( allBooksTxtFile.exists() &&
         createAllBooksLowerCase == 'y' )         // y = Create it
        )
     {
     if ( allBooksTxtFile.exists() )
        {
        qInfo() << functionName
                << " - Previous File with AllBooks = "
                << allBooksFileName
                << " will be removed."
                << Qt::endl;

        retCodeAndMsgObj = books2Select.removingFiles( allBooksFileName );

        if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
          return retCodeAndMsgObj.getErrorCode();
        }    // if ( allBooksTxtFile.exists())

     /*--------------------------------------------------------------------
      *              Prepare arguments to 'find' Process
      *              to create AllBooks.txt
      *--------------------------------------------------------------------*/

     cmdToExecute       = constructedLiteral.retrieveLiteral( "bashProgam" );
     arguments.clear();
     arguments  << constructedLiteral.retrieveLiteral( "argument2Bash" )
                << constructedLiteral.findCmd( fi_BookCollectionDir.absoluteFilePath() );
     books2Select.setOutputFile( constructedLiteral.retrieveLiteral( "tmpFileName3" ) );

     qInfo() << functionName << "cmdToExecute = "  << cmdToExecute << Qt::endl;

     /*--------------------------------------------------------------------
      *              Start 'find' Process
      *--------------------------------------------------------------------*/

     retCodeAndMsgObj   = books2Select.startTask( cmdToExecute,
                                                  arguments );

     if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
        {
        retCodeAndMsgObj.display();

        return retCodeAndMsgObj.getErrorCode();
        }

     /*--------------------------------------------------------------------
      *              Prepare arguments to 'sort' Process
      *--------------------------------------------------------------------*/

     QString wholeSortCmd = constructedLiteral.retrieveLiteral( "sortCmd" ) +
                            " " +
                            constructedLiteral.retrieveLiteral( "tmpFileName3" );

     cmdToExecute       = constructedLiteral.retrieveLiteral( "bashProgam" );
     arguments.clear();
     arguments  << constructedLiteral.retrieveLiteral( "argument2Bash" )
                << wholeSortCmd;
     books2Select.setOutputFile( ListsOfBooksDir + constructedLiteral.retrieveLiteral( "allBooksTXTFile" ) );

     /*--------------------------------------------------------------------
      *              Start 'sort' Process
      *--------------------------------------------------------------------*/

     retCodeAndMsgObj   = books2Select.startTask( cmdToExecute,
                                                  arguments );

     if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
        {
        retCodeAndMsgObj.display();

        return retCodeAndMsgObj.getErrorCode();
        }

     /*--------------------------------------------------------------------
      *       Remove temp3 file, that had already been used
      *--------------------------------------------------------------------*/

     QString tempFile = tempDir.filePath( constructedLiteral.retrieveLiteral( "tmpFileName3" ) );

     qInfo()    << functionName
                << "File temp3 = "
                << tempFile
                << " will be removed."
                << Qt::endl;

     retCodeAndMsgObj = books2Select.removingFiles( tempFile );

     if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
        {
        retCodeAndMsgObj.display();
        qCritical() << functionName
                    << " - Program will terminate. "
                    << Qt::endl;

        return retCodeAndMsgObj.getErrorCode();
        }
     }

   goAway:

  /*--------------------------------------------------------------------
   *        Calls "realWork" function that uses:
   *   BOOK_LISTS_DIR            -> File to put the list of books found
   *   SUBJECTS_FILE             -> file with subjects
   *--------------------------------------------------------------------*/

  retCodeAndMsgObj = books2Select.realWork( ListsOfBooksDir,                    // BookListsDir
                                            SubjectsFile );                     // Subjects file

  if ( retCodeAndMsgObj.getErrorCode() != EXIT_SUCCESS )
     {
     retCodeAndMsgObj.display();

     return retCodeAndMsgObj.getErrorCode();
     }

  /*--------------------------------------------------------------------
   *              Display books found Totals
   *--------------------------------------------------------------------*/

  QString       FileTotals      = ListsOfBooksDir + constructedLiteral.retrieveLiteral( "selectedBooksTotalsFile" );

  OpenFile_Lib  myTotalsFile( FileTotals,
                                selectBooks( __FUNCTION__ ),
                              "WOAPTX" );       // WO = write only; AP = Append; TX = Text

  QTextStream   myFileForTheTotals( myTotalsFile.getMp_File() );

  /*--------------------------------------------------------------------
   *              Count # of books
   *      # of lines corresponds to the # of books
   *--------------------------------------------------------------------*/

  QString       allBooksFile    = ListsOfBooksDir + constructedLiteral.retrieveLiteral( "allBooksTXTFile" );

  OpenFile_Lib  myAllBooksFile( allBooksFile,
                                selectBooks( __FUNCTION__ ),
                                "ROTX" ); // RO = read only; TX = Text

  QTextStream   booksFile( myAllBooksFile.getMp_File() );
  int           lineCounter     = 0;

  while ( ! booksFile.atEnd() )
        {
        booksFile.readLine();
        ++lineCounter;
        }

  myFileForTheTotals    << allBooksFile
                        << "\n# of books in Library = "
                        << lineCounter
                        << Qt::endl;

  /*--------------------------------------------------------------------
   *              Line full of '='
   *--------------------------------------------------------------------*/

  myFileForTheTotals << constructedLiteral.retrieveLiteral( "equalSignLine" );

  return EXIT_SUCCESS;
  }
