/******************************************************************
* Purpose:  Identify the eneplicities and put those records
*           in a file.
*
* Input :   file composed by Number + Size in bytes of the
*           proper book + FileName of the book extracted
*
* Output:   - File with the duplicate records.
*           - File with no duplicities
*
* ****************************************************************/

#include "eniplicitiesProj.h"

#include "elapsedtime_Lib.h"
#include "logger_lib.h"

/********************************************************
*          struct
********************************************************/

struct CounterAndLine
{
  qint64 counter;
  QString line;

  CounterAndLine()
    : line ( " " ),
    counter( 0 )
  {
  }

  void
  add()
  {
    ++counter;
  }

};

/********************************************************
*          remove "_(N)" from the line
********************************************************/

void
remove_UnderL_LParen_Number_RParen( CounterAndLine & counterAndLine )
{
  QString proj {"eniplities"};

  static const QRegularExpression re ( R"((_{0,5}\(\d\)\.))" );
  int startOffset {0};
  int endOffset {0};

  QRegularExpressionMatch match = re.match( counterAndLine.line );

  if ( match.hasMatch() )
     {
     startOffset = match.capturedStart( 1 );
     endOffset = match.capturedEnd( 1 );

     qDebug() << "(" << proj << ")" << "line = " << counterAndLine.line << Qt::endl;

     counterAndLine.line.remove( startOffset, endOffset - startOffset - 1);
     counterAndLine.add();

     qDebug() << "(" << proj << ")" << "line = " << counterAndLine.line << Qt::endl;
     }
}

/********************************************************
*          main function
********************************************************/
int
main( int argc, const char *argv[] )
{
  const QString logType = "RELEASE";
  QString proj {"eniplities"};

  Logger_Lib::init(proj, logType );

  /********************************************************
  *          helper objects
  ********************************************************/
  ElapsedTime_Lib timeSpent;

  if ( argc < 4 )
     {
     qWarning() << "(" << proj << ")"
                << "Please inform two (3) parameters: "
                << Qt::endl
                << "1 - input file with (Number+bookSize+bookFileName)"
                << Qt::endl
                << "2 - output file with the duplicate records"
                << Qt::endl
                << "3 - output file without duplicities"
                << Qt::endl
                << "Program will terminate. "
                << Qt::endl;

     return 1;
     }

  /******************************************************************
   *       Received parameters
   *       1st - Number+bookSize+bookFileName
   *       2nd - Out file with the duplicate records
   *****************************************************************/

  qInfo() << "(" << proj << ")" << "Number+bookSize+bookFileName    = " << argv[ 1 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "file with the duplicate records = " << argv[ 2 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "file without duplicities        = " << argv[ 3 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "================================================================================================="
          << Qt::endl;

  /******************************************************************
  *       num_Size_Filename
  * ****************************************************************/

  QString num_Size_Filename = argv[ 1 ];
  QFile num_Size_Filename_File( num_Size_Filename );

  if ( ! num_Size_Filename_File.open( QIODevice::ReadOnly |
                                      QIODevice::Text ) )
     {
     qInfo() << "(" << proj << ")"
              << " - File for the Num+Size+FileName Record is inaccessible, "
              << Qt::endl
              << "program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream Num_Size_Filename_File_In( &num_Size_Filename_File );

  /******************************************************************
  *       duplicate_Rec
  * ****************************************************************/

  QString duplicate_Rec = argv[ 2 ];
  QFile duplicate_Rec_File( duplicate_Rec );

  if ( ! duplicate_Rec_File.open( QIODevice::WriteOnly |
                                  QIODevice::Truncate ) )
     {
     qInfo() << "(" << proj << ")"
              << " - File for the duplicate Records can not be created."
              << Qt::endl
              << "Program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream duplicate_Rec_File_Out( &duplicate_Rec_File );

  /******************************************************************
  *       no_Duplicates
  * ****************************************************************/

  QString no_Duplicates = argv[ 3 ];
  QFile no_Duplicates_File( no_Duplicates );

  if ( ! no_Duplicates_File.open( QIODevice::WriteOnly |
                                  QIODevice::Truncate ) )
     {
     qInfo() << "(" << proj << ")"
              << " - File without duplicities can not be created."
              << Qt::endl
              << "Program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream no_Duplicates_File_Out( &no_Duplicates_File );

  /******************************************************************
  *       counters definition
  * ****************************************************************/

  qint64 readCount{0};
  qint64 duplicities_WriteCount{0};
  qint64 no_Duplicates_WriteCount{0};
  qint64 u_LP_N_RP_Counter {0};

  /******************************************************************
  *       working strings
  * ****************************************************************/

  QString prior_line {""};
  QString prior_bookSize {""};
  QString prior_bookTitle {""};

  QString line {""};
  QString bookSize{""};
  QString bookTitle{""};

  bool hadDuplicate = false;

  CounterAndLine counterAndLine;

  while ( ! Num_Size_Filename_File_In.atEnd() )
        {
        /******************************************************************
        *       save the line just read
        * ****************************************************************/

        prior_line = line;
        prior_bookSize = bookSize;
        prior_bookTitle = bookTitle;

        line = Num_Size_Filename_File_In.readLine();
        ++readCount;

        /******************************************************************
        *       remove "_(N)" from the line just read
        * ****************************************************************/

        counterAndLine.line = line;
        remove_UnderL_LParen_Number_RParen( counterAndLine );
        line = counterAndLine.line;

        /******************************************************************
        *       split the different fields of the line
        * ****************************************************************/

        QStringList recList = line.split( " " );

        bookSize    = recList.at( 1 );
        bookTitle   = recList.at( 2 );

        /******************************************************************
        *       take care of the duplicate records
        * ****************************************************************/

        if ( prior_bookTitle ==  bookTitle  &&
             prior_bookSize == bookSize )
           {
           hadDuplicate = true;
           duplicate_Rec_File_Out << prior_line << Qt::endl;
           ++duplicities_WriteCount;
           }
        else
            {
            if ( hadDuplicate )
               {
               hadDuplicate = false;
               duplicate_Rec_File_Out << prior_line << Qt::endl;
               ++duplicities_WriteCount;
               }
            else
                {
                if ( prior_line != "" )
                   {
                   no_Duplicates_File_Out << prior_line << Qt::endl;
                   ++no_Duplicates_WriteCount;
                   }
                }
            }
        }

  if ( hadDuplicate )
     {
     duplicate_Rec_File_Out << line << Qt::endl;
     ++duplicities_WriteCount;
     }
  else
      {
      no_Duplicates_File_Out << line << Qt::endl;
      ++no_Duplicates_WriteCount;
      }

  u_LP_N_RP_Counter = counterAndLine.counter;

  /******************************************************************
  *                        T O T A L S
  * ****************************************************************/

  qInfo() << "(" << proj << ")"
          << " - Records read                             =  "
          << readCount
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Records with UnderL_LParen_Number_RParen =  "
          << u_LP_N_RP_Counter
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Duplicate Records written                =  "
          << duplicities_WriteCount
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Records without duplicities written      =  "
          << no_Duplicates_WriteCount
          << Qt::endl;

  return 0;
}
