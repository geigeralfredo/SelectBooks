/***************************************************************
* Purpose:     1-Reads the Subjects file that was already sorted
*              with the -u (unique flag) so there are no duplicate
*              words.
*
*              2-between 2 consective lines with the same word
*              remove the one that have an "s" at the end.
*
*              3-Output:    selectedBooks_SubjectsWithoutPlural.txt
*                           selectedBooks_WordsWithPlural.txt
*
* Parameters:  1 - File with the subject words.
***************************************************************/
#include "removePluralProj.h"
#include "openFile_Lib.h"

/********************************************************
*          returns the Project Name
********************************************************/

QString
removePlural( const QString & project )
  {
  if ( project == "main" )
    return __FUNCTION__;
  else
    return project;
  }

/********************************************************
*          checkParameters helper function
********************************************************/
bool
checkParameter( const QString & fileName, const QString & project = "checkParameter" );

/********************************************************
*          main function
********************************************************/
int
main( int argc, char *argv[] )
  {
  ElapsedTime_Lib   timeIntervalObj;

  QString           thisProjectName = removePlural( __FUNCTION__ );

  if ( argc < 4 )
     {
     qDebug()   << thisProjectName
                << " - Function ("
                << thisProjectName
                << ") "
                << "Please inform:"
                << Qt::endl
                << "The input file of Subjects."
                << Qt::endl
                << "The output file that will receive Subjects Without Plural"
                << Qt::endl
                << "The outfile that will receive Subjects (Words) With Plural"
                << Qt::endl
                << "Program will terminate"
                << Qt::endl;

     return 1;
     }

  QString       subjectsFileNameIn      = argv[ 1 ];
  QString       subjectsWithoutPlural   = argv[ 2 ];
  QString       subjectsWithPlural      = argv[ 3 ];

  bool          isFile1Ok               = checkParameter( argv[ 1 ] );
  bool          isFile2Ok               = checkParameter( argv[ 2 ] );
  bool          isFile3Ok               = checkParameter( argv[ 3 ] );

  if ( ( ! isFile1Ok ) ||
       ( ! isFile2Ok ) ||
       ( ! isFile3Ok )
        )
    return 2;

  OpenFile_Lib  subjectsFileIn( subjectsFileNameIn, thisProjectName, "ROTX" );
  OpenFile_Lib  subjectsWithoutPluralOut( subjectsWithoutPlural, thisProjectName, "WOTRTX" );
  OpenFile_Lib  subjectsWithPluralOut( subjectsWithPlural, thisProjectName, "WOTRTX" );

  QTextStream   mySubjectsIn( subjectsFileIn.getMp_File() );
  QTextStream   mySubjectsWithoutPluralOut( subjectsWithoutPluralOut.getMp_File() );
  QTextStream   mySubjectsWithPluralOut( subjectsWithPluralOut.getMp_File() );

  QString       previousLine;
  QString       currentLine;
  int           readCount               = 0;
  int           writeCountWithPlural    = 0;
  int           writeCountWithoutPlural = 0;
  int           previousLineSize        = 0;
  int           equalCount              = 0;

  while ( ! mySubjectsIn.atEnd() )
        {
        currentLine = mySubjectsIn.readLine();
        ++readCount;
        int currentLineSize = currentLine.size();

        if ( currentLine [ currentLineSize - 1 ] != 's' )
           {
           mySubjectsWithoutPluralOut << currentLine << Qt::endl;
           ++writeCountWithoutPlural;
           }
        else
            {
            if ( previousLineSize != currentLineSize - 1 )
               {
               mySubjectsWithoutPluralOut << currentLine << Qt::endl;
               ++writeCountWithoutPlural;
               }
            else
                {
                for ( int i = 0; i < previousLineSize; ++i )
                    {
                    if ( currentLine[ i ] == previousLine[ i ] )
                      ++equalCount;
                    }

                if ( equalCount == previousLineSize  )
                   {
                   mySubjectsWithPluralOut << currentLine << Qt::endl;
                   ++writeCountWithPlural;
                   }
                else
                    {
                    mySubjectsWithoutPluralOut << currentLine << Qt::endl;
                    ++writeCountWithoutPlural;
                    }
                }
            }

        equalCount          = 0;
        previousLine        = currentLine;
        previousLineSize    = currentLineSize;
        }

    qInfo() << thisProjectName
            << " - Function (" << thisProjectName << ") "
            << "Records read                   =  "
            << readCount
            << Qt::endl;

    qInfo() << thisProjectName
            << " - Function (" << thisProjectName << ") "
            << "Records written with plural    =  "
            << writeCountWithPlural
            << Qt::endl;

    qInfo() << thisProjectName
            << " - Function (" << thisProjectName << ") "
            << "Records written without plural =  "
            << writeCountWithoutPlural
            << Qt::endl;

    qInfo() << thisProjectName
            << " - Function (" << thisProjectName << ") "
            << "With plural                    =  "
            << writeCountWithPlural
            << " + "
            << Qt::endl

            << thisProjectName
            << " - Function (" << thisProjectName << ") "
            << "Without plural                 =  "
            << writeCountWithoutPlural
            << Qt::endl

            << thisProjectName
            << " - Function (" << thisProjectName << ") "
            << "                               =  "
            << writeCountWithoutPlural + writeCountWithPlural
            << Qt::endl;

  }

/********************************************************
*          checkParameters function
********************************************************/
bool
checkParameter( const QString & fileName, const QString & project )
  {
  QFileInfo fi_fileName( fileName );

  if (
    ( ! ( fi_fileName.exists() && fi_fileName.isFile() ) )
     )
     {
     qDebug()   << project
                << " - Function (" << __FUNCTION__ << ") "
                << Qt::endl
                << "The file = "
                << fileName << " does not exist or "
                << Qt::endl
                << "the passed strings does not represent a file."
                << Qt::endl
                << "Program will terminate" << Qt::endl;

     return ( fi_fileName.exists() && fi_fileName.isFile() );
     }

  if (
    ( !  fi_fileName.isAbsolute() )
     )
     {
     qDebug()   << project
                << " - Function (" << __FUNCTION__ << ") "
                << Qt::endl
                << "The file = "
                << fileName << " must be an absolute file path."
                << Qt::endl
                << "Program will terminate" << Qt::endl;

     return ( fi_fileName.isAbsolute() );
     }

  return true;
  }
