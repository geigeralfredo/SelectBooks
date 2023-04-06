/******************************************************************
* Purpose:  Generates 2 output files.
*           The most important of them will be used
*           in the next program to detect eBook eneplicities and
*           to that end will be used the fields
*           file SIZE and FileName.
*           By identifying the eneplicities we will be able to
*           delete the duplicate records.
*
* Input:    Read the AllBooks file
*
* Output:
*   1 - file composed by Number + Record read.
*   2 - file composed by Number + Size in bytes of the
*       proper book + FileName of the book extracted
*       from the record read.
*
* OBS:  Number is a counter of the lines read from the AllBooks file
* ****************************************************************/

#include "removeDuplicates2Proj.h"

#include "elapsedtime_Lib.h"
#include "logger_lib.h"

/********************************************************
*          main function
********************************************************/
int
main( int argc, const char *argv[] )
{
  const QString logType = "RELEASE";
  QString proj {"removeDuplicates2"};

  Logger_Lib::init(proj, logType );

  ElapsedTime_Lib timeIntervalObj;

  if ( argc < 4 )
     {
     qWarning( "Usage: \"removeDuplicates2 AllBooks.txt file(Num+CompleteRecord) file(Num+Size+FileName)\"" );

     return 1;
     }

/******************************************************************
*       Received parameters
*       1st - All Books file
*       2nd - Out file with every book with a sequence number
*       3rd - Out file with a seq number and the size in bytes
*       of the proper ebook
* ****************************************************************/

  qInfo() << "(" << proj << ")" << "AllBooks.txt             = " << argv[ 1 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "file(Num+CompleteRecord) = " << argv[ 2 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "file(Num+Size+FileName)  = " << argv[ 3 ] << Qt::endl;

  /******************************************************************
  *       allBooksFile
  * ****************************************************************/

  QString allBooksFile = argv[ 1 ];
  QFile allBooksFileIn( allBooksFile );

  if ( ! allBooksFileIn.open( QIODevice::ReadOnly | QIODevice::Text ) )
     {
     qWarning() << "(" << proj << ")"
              << " - AllBooks file does not exist, program will terminate. "
              << Qt::endl;

     return 1;
     }

  /******************************************************************
  *       num_Complete_Record
  * ****************************************************************/

  QString num_Complete_Record = argv[ 2 ];
  QFile num_Complete_Record_File( num_Complete_Record );

  if ( ! num_Complete_Record_File.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
     {
     qWarning() << "(" << proj << ")"
              << " - File for the Complete Record is inaccessible, program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream Complete_Record_File_Out ( &num_Complete_Record_File );

  /******************************************************************
  *       num_Size_Filename
  * ****************************************************************/

  QString num_Size_Filename = argv[ 3 ];
  QFile num_Size_Filename_File( num_Size_Filename );

  if ( ! num_Size_Filename_File.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
     {
     qWarning() << "(" << proj << ")"
              << " - File for the Num+Size+FileName Record is inaccessible, program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream Num_Size_Filename_File_Out ( &num_Size_Filename_File );

  /******************************************************************
  *       counters definition
  * ****************************************************************/

  qint64 readCounter{ 0 };
  qint64 missingBookCounter{ 0 };
  qint64 completeWriteCounter{ 0 };
  qint64 numSizeFileWriteCounter{ 0 };

  /******************************************************************
  *       working areas
  * ****************************************************************/

  QString ext;
  qint64 size;
  QString fileName;

  /******************************************************************
  *       Treat the AllBooks file
  * ****************************************************************/

  QTextStream in( &allBooksFileIn );

  while ( ! in.atEnd() )
        {
        QString line = in.readLine();

        ++readCounter;

        /******************************************************************
        *       get extension and fileName
        * ****************************************************************/

        QFileInfo fileInfo{ line };
        ext         = fileInfo.suffix();
        fileName    = fileInfo.fileName();

        QFile newBook ( line );

        if ( ! newBook.open( QIODevice::ReadOnly | QIODevice::Text ) )
           {
           qInfo() << "(" << proj << ")"
                    << " - The Book = "
                    << line
                    << " does not exist. "
                    << Qt::endl;

           ++missingBookCounter;
           continue;
           }

        /******************************************************************
        *       Here the book exists
        *       so get its size
        * ****************************************************************/

        QFileInfo newBookFi ( newBook );
        size = newBookFi.size();

        /******************************************************************
        *  Write the complete record with a sequence number (readCounter)
        * ****************************************************************/

        QString zeroPaddedReadCounter   = QString::number( readCounter ).rightJustified( 7, '0' );
        Complete_Record_File_Out << zeroPaddedReadCounter + " " + line << Qt::endl;
        ++completeWriteCounter;

        /******************************************************************
        *  Write the numSizeFilename record with a
        *  sequence number (readCounter) and the eBooks' size
        * ****************************************************************/

        fileName.replace( " ", "_" );

        QString zeroPaddedSize          = QString::number( size ).rightJustified( 10, '0' );
        Num_Size_Filename_File_Out << zeroPaddedReadCounter + " " + zeroPaddedSize + " " +  fileName << Qt::endl;
        ++numSizeFileWriteCounter;

        newBook.close();
        }

  /******************************************************************
  *                        T O T A L S
  * ****************************************************************/

  qInfo() << "(" << proj << ")"
          << " - Records read from AllBooks file        =  "
          << readCounter
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Records written to CompleteRecordFile  =  "
          << completeWriteCounter
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Records written to numSizeFilenameFile =  "
          << numSizeFileWriteCounter
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Missing Books                          =  "
          << missingBookCounter
          << Qt::endl;

  return 0;
}
