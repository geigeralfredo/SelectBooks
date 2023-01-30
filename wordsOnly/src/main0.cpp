/********************************************************************
 * Purpose:     1-Prepend a number of 7 digits (as a string) to every
 *              Book Title of the AllBooks file.
 *
 *              2-Split every word of every Book Title and prepend
 *              to each the 7 digit number of item 1- above.
 *
 *              3-Remove every string that matchs any regular expression
 *              found in the vector of regexs.
 *
 *              4-Select words with size between lowestLimitWordSize and
 *              highestLimitWordSize inclusive.
 *
 * Parameters:  1 - The input file with the book list.
 *              2 - The Directory where the actual Books are located.
 *******************************************************************/
#include "wordsOnlyProj.h"
#include "openFile_Lib.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************************
 *       Remove accents of the words (version 1)
 *******************************************************************/

QString
stripDiacritics( QString & text )
  {
  QString   formD = text.normalized( QString::NormalizationForm_D );

  QString   filtered;

  for ( int i = 0; i < formD.length(); i++ )
      {
      if ( formD.at( i ).category() != QChar::Mark_NonSpacing )
        filtered.append( formD.at( i ) );
      }

  return filtered.normalized( QString::NormalizationForm_C );
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

static const QString        diacriticLetters_ =
  QString::fromUtf8( "ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ" );

static const QStringList    noDiacriticLetters_
  {
  "S", "OE", "Z", "s", "oe", "z", "Y", "Y",
  "u", "A", "A", "A", "A", "A", "A", "AE",
  "C", "E", "E", "E", "E", "I", "I", "I",
  "I", "D", "N", "O", "O", "O", "O", "O",
  "O", "U", "U", "U", "U", "Y", "s", "a",
  "a", "a", "a", "a", "a", "ae", "c", "e",
  "e", "e", "e", "i", "i", "i", "i", "o",
  "n", "o", "o", "o", "o", "o", "o", "u",
  "u", "u", "u", "y", "y"
  };

/********************************************************************
 *       Remove accents of the words (version 2)
 *******************************************************************/

QString
removeAccents( QString & s )
  {
  QString output = "";

  for ( int i = 0; i < s.length(); ++i )
      {
      QChar c       = s[ i ];
      int   dIndex  = diacriticLetters_.indexOf( c );

      if ( dIndex < 0 )
         {
         output.append( c );
         }
      else
          {
          QString replacement = noDiacriticLetters_[ dIndex ];
          output.append( replacement );
          }
      }

  return output;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************************
 *                  GLOBAL CONSTANTS
 *      to remove lines that match these regular expressions
 *******************************************************************/

static const QRegularExpression             regex01( R"(^\d+$)" );                                   // Only numbers
static const QRegularExpression             regex02( R"(^((fig)?((\d+)('s)*([^a-zA-Z0-9]*))*)$)" );  // fig08_09_10
static const QRegularExpression             regex03( R"(^(\d+('s)*)*$)" );                           // 1980's
static const QRegularExpression             regex04( R"(^#[0-9]+[^a-zA-Z0-9]*[0-9]*)" );             // #\d\d\d...
static const QRegularExpression             regex05( R"(^c+[0-9]+_*[code|exercises]*)" );            // c\d\d\d...
static const QRegularExpression             regex06( R"(^cartoons+[0-9]+)" );                        // cartoons\d\d\d...
static const QRegularExpression             regex07( R"(^ch+[0-9]+)" );                              // ch\d\d\d...
static const QRegularExpression             regex08( R"(^chap+[0-9]+)" );                            // chap\d\d\d...
static const QRegularExpression             regex09( R"(^chapter+.*[0-9]+)" );                       // chapter\d\d\d...
static const QRegularExpression             regex10( R"(^computers+[0-9]+)" );                       // computers\d\d\d...
static const QRegularExpression             regex11( R"(^cs+[0-9]+[a-zA-Z]*.*[0-9]*)" );             // cs\d\d\d...
static const QRegularExpression             regex12( R"(^ex+[0-9]+[a-zA-Z]*.*[0-9]*)" );             // ex\d\d\d...
static const QRegularExpression             regex13( R"(^fall+[0-9]+[a-zA-Z]*.*[0-9]*)" );           // fal\d\d\d...
static const QRegularExpression             regex14( R"(^os+[0-9]+[a-zA-Z]*.*[0-9]*)" );             // os\d\d\d...
static const QRegularExpression             regex15( R"(^plays+.{1}[0-9]+)" );                       // plays.\d\d\d...
static const QRegularExpression             regex16( R"(^store_os+[0-9]{2}c+[0-9]{5})" );            // store_os\d\d\d...
static const QRegularExpression             regex17( R"(^week[0-9](sln)*)" );                        // week\d(sln).....
static const QRegularExpression             regex18( R"((?<!\d)\d+(?!\d))" );                        // embeded number(s)

/********************************************************************
 *              Vector of regex
 *******************************************************************/

static const QVector <QRegularExpression>   regexVec { regex01,
                                                       regex02,
                                                       regex03,
                                                       regex04,
                                                       regex05,
                                                       regex06,
                                                       regex07,
                                                       regex08,
                                                       regex09,
                                                       regex10,
                                                       regex11,
                                                       regex12,
                                                       regex13,
                                                       regex14,
                                                       regex15,
                                                       regex16,
                                                       regex17,
                                                       regex18
                                                       };

static int          matchCount  { 0 };

static const int    lowestLimitWordSize { 4 };
static const int    highestLimitWordSize { 17 };

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************************
 *      verfy if word matchs one of the regex of the Vector
 *      of regular expressions and return "true" or "false"
 *******************************************************************/

bool
verifyWord ( const QString  & word,
             QTextStream    & logFile )
  {
  QRegularExpressionMatch   match;
  QString                   matched;

  for ( int i = 0; i < regexVec.size(); ++i )
      {
      if ( regexVec[ i ].isValid() )
         {
         match = regexVec[ i ].match( word );

         if ( match.hasMatch() )
            {
            matched = match.captured( 0 );

            logFile << "matched = " << matched << Qt::endl;
            ++matchCount;
            break;
            }
         }
      else
          {
          logFile   << "Regular expression              = "
                    << regexVec[ i ].pattern()
                    << Qt::endl;

          logFile   << "Regular expression ERROR        = "
                    << regexVec[ i ].errorString()
                    << Qt::endl;

          logFile   << "Regular expression ERROR offset = "
                    << regexVec[ i ].patternErrorOffset()
                    << Qt::endl;

          logFile   << "Regular expression is incorrect."
                    << Qt::endl;
          }
      }

  return match.hasMatch();
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

int
main( int argc, char *argv[] )
  {
  if ( argc < 3 )
     {
     qDebug()   << "WordsOnly - Function (" << __FUNCTION__ << ") "
                << "This program expects two parameters:"
                << " 1 - The input file with the book list."
                << " 2 - The Directory where the actual Books are located."
                << "Program will terminate" << Qt::endl;

     return 1;
     }

  /********************************************************************
   *         File definitions
   *******************************************************************/

  QString   allBooksFileIn = argv[ 1 ];
  QFileInfo fi_allBooksFileIn( allBooksFileIn );

  if ( ! fi_allBooksFileIn.isAbsolute() )
     {
     qDebug()   << "WordsOnly - Function (" << __FUNCTION__ << ") "
                << "The informed file names must be absolute file paths."
                << "Program will terminate" << Qt::endl;

     return 1;
     }

  QString       dirName2BeSearched      = argv [ 2 ];
  int           dirNameSize             = dirName2BeSearched.size();

  /********************************************************************
   *         Hard coded File Names
   *******************************************************************/

  QString       wordsOnlyFileOut        = "/home/alfredo/Documents/TXT/selectedBooks_OnlyWords.txt";
  QString       lineNrWordsFileOut      = "/home/alfredo/Documents/TXT/selectedBooks_LineNrWords.txt";
  QString       lineNrAllBooksFileOut   = "/home/alfredo/Documents/TXT/selectedBooks_LineNrAllBooks.txt";

  QString       rejectedWordsFileOut    = "/home/alfredo/Documents/TXT/selectedBooks_RejectedWords.txt";
  QString       logFile                 = "/home/alfredo/Documents/TXT/selectedBooks_LogFile.txt";

  /********************************************************************
   *         File In
   *******************************************************************/

  OpenFile_Lib  FileIn_AllBooks( allBooksFileIn, __FUNCTION__, "ROTX" );
  QTextStream   myFileIn_AllBooks( FileIn_AllBooks.getMp_File() );

  /********************************************************************
   *         File Out
   *******************************************************************/

  OpenFile_Lib  FileOut_WordsOnly( wordsOnlyFileOut, __FUNCTION__, "WOTRTX" );
  QTextStream   myFileOut_WordsOnly( FileOut_WordsOnly.getMp_File() );

  OpenFile_Lib  FileOut_LineNrWords( lineNrWordsFileOut, __FUNCTION__, "WOTRTX" );
  QTextStream   myFileOut_LineNrWords( FileOut_LineNrWords.getMp_File() );

  OpenFile_Lib  FileOut_LineNrAllBooks( lineNrAllBooksFileOut, __FUNCTION__, "WOTRTX" );
  QTextStream   myFileOut_LineNrAllBooks( FileOut_LineNrAllBooks.getMp_File() );

  OpenFile_Lib  FileOut_RejectedWords ( rejectedWordsFileOut, __FUNCTION__, "WOTRTX" );
  QTextStream   myFileOut_RejectedWords( FileOut_RejectedWords.getMp_File() );

  OpenFile_Lib  FileOut_Log( logFile, __FUNCTION__, "WOTRTX" );
  QTextStream   myFileOut_Log( FileOut_Log.getMp_File() );

  /********************************************************************
   *            Counters
   *******************************************************************/

  int           readCount   { 0 };
  int           ignoredRecs { 0 };
  int           goodWordsCount   { 0 };

  int           lineNrAllBooksCount { 0 };
  int           lineNrWordsCount { 0 };

  int           rejectedWordsCount { 0 };

  QString       originalLine;
  QString       currentLine;

  /********************************************************************
   *                 Read AllBooks file
   *******************************************************************/

  while ( ! myFileIn_AllBooks.atEnd() )
        {
        /********************************************************************
         *  Le registro transforma em letras minusculas
         *  e remove o nome do diretorio que aparece em todas as linhas
         *******************************************************************/

        originalLine    = myFileIn_AllBooks.readLine();
        ++readCount;

        qsizetype   dirIndex            = originalLine.indexOf( dirName2BeSearched );

        if ( dirIndex != -1 )
          originalLine.remove( dirIndex, dirNameSize );

        currentLine     = originalLine;

        QString     readCountAsString   = QString( "%1" ).arg( readCount, 7, 10, QChar( '0' ) );

        myFileOut_LineNrAllBooks    << readCountAsString
                                    << " "
                                    << originalLine
                                    << Qt::endl;
        ++lineNrAllBooksCount;

        currentLine     = currentLine.toLower();

        /********************************************************************
         *      despreza registro com size = 0
         *******************************************************************/

        if ( currentLine.size() == 0 )
           {
           ++ignoredRecs;
           continue;
           }

        /********************************************************************
         *      remove acentos
         *******************************************************************/

        QString                         strUnAccented{};
        strUnAccented = stripDiacritics( currentLine );

        /********************************************************************
         *      quebra linha em palavras considerando todo tipo
         *      de caracteres especiais como separadores
         *******************************************************************/

        QString                         strRe   = R"([^A-Za-z0-9]\s*(?=\w*))";
        static const QRegularExpression regex( strRe );
        QStringList                     strList = strUnAccented.split( regex, Qt::SkipEmptyParts );

        for ( int i = 0; i < strList.size(); ++i )
            {
            /********************************************************************
             *      Apenas aceitar palavras entre
             *      lowestLimitWordSize (4) e
             *      highestLimitWordSize (17) caracteres (inclusive)
             *******************************************************************/

            if ( lowestLimitWordSize <= strList.at( i ).size() &&
                 strList.at( i ).size() <= highestLimitWordSize )
               {
               bool hadMatch = verifyWord( strList.at( i ), myFileOut_Log );

               /********************************************************************
                *      estamos interessados em hadMatch = false
                *******************************************************************/

               if ( ! hadMatch )
                  {
                  myFileOut_LineNrWords << readCountAsString
                                        << " "
                                        << strList.at( i )
                                        << Qt::endl;
                  ++lineNrWordsCount;

                  myFileOut_WordsOnly << strList.at( i ) << Qt::endl;
                  ++goodWordsCount;
                  }
               else
                   {
                   myFileOut_RejectedWords << strList.at( i ) << Qt::endl;
                   ++rejectedWordsCount;
                   }
               }
            else
                {
                myFileOut_RejectedWords << strList.at( i ) << Qt::endl;
                ++rejectedWordsCount;
                }
            }
        }

  /********************************************************************
   *                    T O T A I S
   *******************************************************************/

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Records read                   =  "
            << readCount
            << Qt::endl;

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Records ignored                =  "
            << ignoredRecs
            << Qt::endl;

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Accepted Words                 =  "
            << goodWordsCount
            << Qt::endl;

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Line Number and Books          =  "
            << lineNrAllBooksCount
            << Qt::endl;

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Line Number and Accepted Words =  "
            << lineNrWordsCount
            << Qt::endl;

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Rejected Words                 =  "
            << rejectedWordsCount
            << Qt::endl;

    qInfo() << "WordsOnly - Function (" << __FUNCTION__ << ") "
            << "Undesirable Words              =  "
            << matchCount
            << Qt::endl;

  }
