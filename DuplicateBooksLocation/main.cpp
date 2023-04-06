/******************************************************************
* Purpose:  Make a match between 2 files.
*           1 - The 1st contain the dir location where all books are
*           2 - The 2nd contain the duplicate records of all books
*           These 2 files have a field at the beginning that will be
*           used to make the match. It is a sequence number that identify
*           each book.
*
* Input :   The 2 files described above
*
* Output:   File with the duplicate books with their directory
*           location.
* ****************************************************************/

#include "duplicateBooksLocationProj.h"

#include "elapsedtime_Lib.h"
#include "logger_lib.h"

/********************************************************
*          struct
********************************************************/

struct SeqAndCounter
{
  qint64 counter;
  QString sequence;
  QString line;
  QString dirLocation;

  SeqAndCounter() :
    counter( 0 ),
    sequence ( "" ),
    line ( "" ),
    dirLocation ( "" )
  {
  }

};

/********************************************************
*          Read file
********************************************************/

void
readFile( QTextStream & file, SeqAndCounter & seqAndCounter )
{
  const int whole_Rec_Seq_StartPos {0};
  const int whole_Rec_Seq_2BeRemoved {8};

  /********************************************************
  *          read line
  ********************************************************/

  seqAndCounter.line = file.readLine();
  ++seqAndCounter.counter;

  /********************************************************
  *          get the sequence
  ********************************************************/

  QStringList list = seqAndCounter.line.split( " " );
  seqAndCounter.sequence = list.at( 0 );

  /********************************************************
  *          remove sequence
  ********************************************************/

  QString workString = seqAndCounter.line;

  workString.remove( whole_Rec_Seq_StartPos,
                     whole_Rec_Seq_2BeRemoved );
  seqAndCounter.dirLocation = workString;
}

/********************************************************
*          main function
********************************************************/

int
main( int argc, const char *argv[] )
{
  const QString logType = "RELEASE";
  QString proj {"DuplicateBooksLocation"};

  Logger_Lib::init(proj, logType );

  ElapsedTime_Lib timeIntervalObj;

  if ( argc < 4 )
     {
     qWarning() << "(" << proj << ")"
                << "Please inform tree (3) parameters: "
                << Qt::endl
                << "1 - input file with the Complete Book Record"
                << Qt::endl
                << "2 - input file with the Duplicate Book records"
                << Qt::endl
                << "3 - output file with Duplicate Books with their full directory location."
                << Qt::endl
                << "Program will terminate. "
                << Qt::endl;

     return 1;
     }

  /******************************************************************
   *       Received parameters
   *****************************************************************/

  qInfo() << "(" << proj << ")" << "Complete Book Record              = " << argv[ 1 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "Duplicate Book record             = " << argv[ 2 ] << Qt::endl;
  qInfo() << "(" << proj << ")" << "Duplicate Books plus Dir location = " << argv[ 3 ] << Qt::endl;

  /******************************************************************
  *       complete_Record
  * ****************************************************************/

  QString complete_Record = argv[ 1 ];
  QFile complete_Record_File( complete_Record );

  if ( ! complete_Record_File.open( QIODevice::ReadOnly |
                                    QIODevice::Text ) )
     {
     qInfo() << "(" << proj << ")"
              << " - File with the Complete Record is inaccessible, "
        "program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream complete_Record_File_In( &complete_Record_File );

  /******************************************************************
  *       duplicate_Rec
  * ****************************************************************/

  QString duplicate_Rec = argv[ 2 ];
  QFile duplicate_Rec_File( duplicate_Rec );

  if ( ! duplicate_Rec_File.open( QIODevice::ReadOnly |
                                  QIODevice::Text ) )
     {
     qInfo() << "(" << proj << ")"
              << " - File with the duplicate Records is inaccessible."
              << Qt::endl
              << "Program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream duplicate_Rec_File_In( &duplicate_Rec_File );

  /******************************************************************
  *       duplicates_Dir_Loc
  * ****************************************************************/

  QString duplicates_Dir_Loc = argv[ 3 ];
  QFile duplicates_Dir_Loc_File( duplicates_Dir_Loc );

  if ( ! duplicates_Dir_Loc_File.open( QIODevice::WriteOnly |
                                       QIODevice::Truncate ) )
     {
     qInfo() << "(" << proj << ")"
              << " - File with Duplicate Books can not be created."
              << Qt::endl
              << "Program will terminate. "
              << Qt::endl;

     return 1;
     }

  QTextStream duplicates_Dir_Loc_File_Out( &duplicates_Dir_Loc_File );

  /******************************************************************
  *       counters definition
  * ****************************************************************/

  qint64 duplicates_Dir_Loc_WriteCount{0};

  /******************************************************************
  *       working storage
  * ****************************************************************/

  QString complete_Rec_Sequence;
  QString duplicate_Rec_Sequence;

  SeqAndCounter complete_Rec_seqAndCounter;
  SeqAndCounter duplicate_Rec_seqAndCounter;

  /******************************************************************
  *       starts with both input records already read
  * ****************************************************************/

  readFile ( complete_Record_File_In, complete_Rec_seqAndCounter );
  complete_Rec_Sequence = complete_Rec_seqAndCounter.sequence;

  readFile ( duplicate_Rec_File_In, duplicate_Rec_seqAndCounter );
  duplicate_Rec_Sequence = duplicate_Rec_seqAndCounter.sequence;

  while ( ! duplicate_Rec_File_In.atEnd() )
        {
        /******************************************************************
        *       if duplicate > complete -> reads complete
        *       (reads the lesser one)
        * ****************************************************************/
        if ( duplicate_Rec_Sequence > complete_Rec_Sequence )
           {
           readFile ( complete_Record_File_In, complete_Rec_seqAndCounter );
           complete_Rec_Sequence = complete_Rec_seqAndCounter.sequence;
           }
        else
            {
            /******************************************************************
            *       if duplicate < complete -> reads duplicate
            *       (reads the lesser one)
            * ****************************************************************/
            if ( duplicate_Rec_Sequence < complete_Rec_Sequence )
               {
               readFile ( duplicate_Rec_File_In, duplicate_Rec_seqAndCounter );
               duplicate_Rec_Sequence = duplicate_Rec_seqAndCounter.sequence;
               }
            else

            /******************************************************************
            *       if duplicate == complete -> read both files
            *       and writes the composed record
            * ****************************************************************/
                {
                const int dup_Rec_Seq_nSize_StartPos {0};
                const int dup_Rec_Seq_nSize_2BeRemoved {8};
                duplicate_Rec_seqAndCounter.line.remove( dup_Rec_Seq_nSize_StartPos, dup_Rec_Seq_nSize_2BeRemoved );

                duplicates_Dir_Loc_File_Out << duplicate_Rec_seqAndCounter.line + " " +  complete_Rec_seqAndCounter.dirLocation
                                            << Qt::endl;
                ++duplicates_Dir_Loc_WriteCount;

                readFile ( complete_Record_File_In, complete_Rec_seqAndCounter );
                complete_Rec_Sequence = complete_Rec_seqAndCounter.sequence;

                readFile ( duplicate_Rec_File_In, duplicate_Rec_seqAndCounter );
                duplicate_Rec_Sequence = duplicate_Rec_seqAndCounter.sequence;
                }
            }
        }

  /******************************************************************
  *                        T O T A L S
  * ****************************************************************/

  qInfo() << "(" << proj << ")"
          << " - Complete Records read                =  "
          << complete_Rec_seqAndCounter.counter
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Duplicate Records read               =  "
          << duplicate_Rec_seqAndCounter.counter
          << Qt::endl;

  qInfo() << "(" << proj << ")"
          << " - Duplicates with Dir Location written =  "
          << duplicates_Dir_Loc_WriteCount
          << Qt::endl;

  return 0;
}
