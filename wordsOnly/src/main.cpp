/********************************************************************
 * Purpose: 1-Split every word of every Book Title and every word
 *          of its directory name.
 *
 *          2-Reject every word that matchs any regular expression
 *          found in the vectors "regexVec" and "regexCV".
 *
 *          3-Select words with size between lowestLimitWordSize and
 *          highestLimitWordSize inclusive.
 *
 *          4-Creates the SUBJECTS file (selectedBooks_OnlyWords.txt)
 *
 *          5-Output files: - selectedBooks_RejectedWords.txt
 *                            file with the words that matched the regexs Vectors.
 *                            file with the words with size between lowestLimitWordSize
 *                            and highestLimitWordSize.
 *
 *                          - selectedBooks_LogFile.txt
 *                            file with only the words that matched the regexs Vectors.
 *
 * Parameters: 1 - File with a summary of a list of all books.
 *             2 - Directory base name of the Book collection
 *                 to be remove from the AllBooks file.
 *                 Unnecessary because it appears in every line.
 *******************************************************************/
#include "wordsOnlyProj.h"
#include "openFile_Lib.h"
#include "elapsedtime_Lib.h"
#include "logger_lib.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************************
 *                      normalize_KC
 *  Compatibility decomposition followed by canonical composition
 *          preserving the accents (Diacritics) of the words
 *******************************************************************/

QString
normalize_KC( QString text )
  {
  QString   formKC = text.normalized( QString::NormalizationForm_KC );

  QString   filtered;

  for ( int i = 0; i < formKC.length(); i++ )
      {
      if ( formKC.at( i ).category() != QChar::Mark_NonSpacing )
        filtered.append( formKC.at( i ) );
      }

//    qInfo() << __FUNCTION__   << "text   = " << text << Qt::endl;
//    qInfo() << __FUNCTION__   << "formKC = " << formKC << Qt::endl;

  return formKC;
  }

/********************************************************************
 *       Remove accents of the words (version 1)
 *                  ( === NOT USED === )
 *******************************************************************/

QString
removeDiacritics( QString & text )
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
 *                  ( === NOT USED === )
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
 *         Global variable
 *******************************************************************/

static int                      matchCount  { 0 };

/********************************************************************
 *      regexs to remove words that match these regular expressions
 *******************************************************************/

static const QRegularExpression regex01( R"(^\d+$)" );                                                 // Only numbers
static const QRegularExpression regex02( R"(^((fig)?((\d+)('s)*([^a-zA-Z0-9]*))*)$)" );                // fig08_09_10
static const QRegularExpression regex03( R"(^(\d+('s)*)*$)" );                                         // 1980's
static const QRegularExpression regex04( R"(^#[0-9]+[^a-zA-Z0-9]*[0-9]*)" );                           // #\d\d\d...
static const QRegularExpression regex05( R"(^c+[0-9]+_*[code|exercises]*)" );                          // c\d\d\d...
static const QRegularExpression regex06( R"(^cartoons+[0-9]+)" );                                      // cartoons\d\d\d...
static const QRegularExpression regex07( R"(^ch+[0-9]+)" );                                            // ch\d\d\d...
static const QRegularExpression regex08( R"(^chap+[0-9]+)" );                                          // chap\d\d\d...
static const QRegularExpression regex09( R"(^chapter+.*[0-9]+)" );                                     // chapter\d\d\d...
static const QRegularExpression regex10( R"(^computers+[0-9]+)" );                                     // computers\d\d\d...
static const QRegularExpression regex11( R"(^cs+[0-9]+[a-zA-Z]*.*[0-9]*)" );                           // cs\d\d\d...
static const QRegularExpression regex12( R"(^ex+[0-9]+[a-zA-Z]*.*[0-9]*)" );                           // ex\d\d\d...
static const QRegularExpression regex13( R"(^fall+[0-9]+[a-zA-Z]*.*[0-9]*)" );                         // fal\d\d\d...
static const QRegularExpression regex14( R"(^os+[0-9]+[a-zA-Z]*.*[0-9]*)" );                           // os\d\d\d...
static const QRegularExpression regex15( R"(^plays+.{1}[0-9]+)" );                                     // plays.\d\d\d...
static const QRegularExpression regex16( R"(^store_os+[0-9]{2}c+[0-9]{5})" );                          // store_os\d\d\d...
static const QRegularExpression regex17( R"(^week[0-9](sln)*)" );                                      // week\d(sln).....
static const QRegularExpression regex18( R"((?<!\d)\d+(?!\d))" );                                      // embeded number(s)

/********************************************************************
 *  regexs to remove words with 5 or more consecutive consonants or vowels
 *******************************************************************/

static const QRegularExpression regexConsonants ( R"((?:(?![aeiouy])[a-z]){5,})" );
static const QRegularExpression regexVowels ( R"((?:(?![b-df-hj-np-tv-xz])[a-z]){5,})" );

/********************************************************************
 *              Vectors of regexs
 *******************************************************************/

QVector <QRegularExpression>    regexVec { regex01,
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

QVector <QRegularExpression> regexCV { regexConsonants,              // Consonants and vowels
                                       regexVowels };

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************************
 *      verfy if word matchs one of the regex of the Vectors
 *      of regular expressions and return "true" or "false"
 *******************************************************************/

bool
verifyWord ( const QString                  & word,
             QTextStream                    & rejectedRegExFileOut,
             QVector <QRegularExpression>   & regexVec )
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

            if ( regexVec.size() == 2 )
              rejectedRegExFileOut << "Consonants or Vowels matched = " << matched << Qt::endl;
            else
              rejectedRegExFileOut << "matched = " << matched << Qt::endl;

            ++matchCount;
            break;
            }
         }
      else
          {
          rejectedRegExFileOut  << "Regular expression              = "
                                << regexVec[ i ].pattern()
                                << Qt::endl;

          rejectedRegExFileOut  << "Regular expression ERROR        = "
                                << regexVec[ i ].errorString()
                                << Qt::endl;

          rejectedRegExFileOut  << "Regular expression ERROR offset = "
                                << regexVec[ i ].patternErrorOffset()
                                << Qt::endl;

          rejectedRegExFileOut  << "Regular expression is incorrect."
                                << Qt::endl;
          }
      }

  return match.hasMatch();
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************
*          checkParameter helper function
********************************************************/
bool
checkParameter( const QString   & dirOrFileName,
                const QChar     & dirOrFile = 'f',
                const QString   & project   = "checkParameter" );

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

int
main( int argc, const char *argv[] )
  {
  const QString logType = "RELEASE";
  QString proj {"wordsOnly"};

  Logger_Lib::init(proj, logType );

  ElapsedTime_Lib   timeIntervalObj;

  if ( argc < 6 )
     {
     qWarning() << "(" << proj << ")"
                << "This program expects 5 parameters:"
                << " 1 - The input file with the list of All Books."
                << " 2 - The Directory where the actual Books are located."
                << " 3 - The file with Only Words"
                << " 4 - The file with Rejected Words"
                << " 5 - The file for the Rejected Words by RegExs"
                << "Program will terminate" << Qt::endl;

     return 1;
     }

  /********************************************************************
   *         word size limits
   *******************************************************************/

  static const int  lowestLimitWordSize { 4 };
  static const int  highestLimitWordSize { 17 };

  /********************************************************************
   *         File definitions
   *******************************************************************/

  QString           allBooksFileIn          = argv[ 1 ];
  QString           bookCollectionDir       = argv[ 2 ];
  QString           wordsOnlyFileOut        = argv[ 3 ];
  QString           rejectedWordsFileOut    = argv[ 4 ];
  QString           rejectedRegExFileOut    = argv[ 5 ];

  bool              isFile1_Ok              = checkParameter( allBooksFileIn );
  bool              isFile2_Ok              = checkParameter( bookCollectionDir, 'd' );
  bool              isFile3_Ok              = checkParameter( wordsOnlyFileOut );
  bool              isFile4_Ok              = checkParameter( rejectedWordsFileOut );
  bool              isFile5_Ok              = checkParameter( rejectedRegExFileOut );

  if ( ( ! isFile1_Ok ) ||
       ( ! isFile2_Ok ) ||
       ( ! isFile3_Ok ) ||
       ( ! isFile4_Ok ) ||
       ( ! isFile5_Ok )
        )
    return 2;

  int               dirNameSize             = bookCollectionDir.size();

  /********************************************************************
   *         File In
   *******************************************************************/

  OpenFile_Lib      FileIn_AllBooks( allBooksFileIn, proj, "ROTX" );
  QTextStream       myFileIn_AllBooks( FileIn_AllBooks.getMp_File() );

  /********************************************************************
   *         File Out
   *******************************************************************/

  OpenFile_Lib      FileOut_WordsOnly( wordsOnlyFileOut, proj, "WOTRTX" );
  QTextStream       myFileOut_WordsOnly( FileOut_WordsOnly.getMp_File() );

  OpenFile_Lib      FileOut_RejectedWords ( rejectedWordsFileOut, proj, "WOTRTX" );
  QTextStream       myFileOut_RejectedWords( FileOut_RejectedWords.getMp_File() );

  OpenFile_Lib      FileOut_Log( rejectedRegExFileOut, proj, "WOTRTX" );
  QTextStream       myFileOut_Log( FileOut_Log.getMp_File() );

  /********************************************************************
   *            Counters
   *******************************************************************/

  int               readCount   { 0 };
  int               goodWordsCount   { 0 };

  int               AllBooksCount { 0 };
  int               rejectedWordsCount { 0 };

  QString           originalLine;
  QString           currentLine;

  /********************************************************************
   *                 Read AllBooks file
   *******************************************************************/

  while ( ! myFileIn_AllBooks.atEnd() )
        {
        /********************************************************************
         *  Read line and convert each to lower case and remove the base name
         *  of each directory (appears in every line)
         *******************************************************************/

        originalLine    = myFileIn_AllBooks.readLine();
        ++readCount;

        qsizetype dirIndex = originalLine.indexOf( bookCollectionDir );

        if ( dirIndex != -1 )
          originalLine.remove( dirIndex, dirNameSize );

        currentLine     = originalLine;
        ++AllBooksCount;

        currentLine     = currentLine.toLower();

        /********************************************************************
         *      Do not remove accents (diacritics).
         *      In texts the base char and its diacritc (accented letter)
         *      in Utf8 they're really two (2) separated chars and the
         *      "normalize_KC" operation substitutes these two chars
         *      by just one keeping its diacritic (still Utf8), so
         *      you may treat the letter as a normal char as
         *      if it has no diacritic.
         *******************************************************************/

        QString                         strAccented{};
        strAccented     = normalize_KC( currentLine );

        /********************************************************************
         *   Split every word of each line considering every kind
         *   of special characters as separators but letters with diacritics
         *******************************************************************/

//        QString                         strRe   = R"([^A-Za-z0-9]\s*(?=\w*))";

        QString                         strRe   =
          R"([^A-Za-z0-9ÀÁÂÃÄÅàáâãäåÈÉÊËèéêëÌÍÎÏìíîïÒÓÔÕÖòóôõöÙÚÛÜùúûüÇçÑñÝýÿ]\s*(?=\w*))";

        static const QRegularExpression regex( strRe );
        QStringList                     strList = strAccented.split( regex, Qt::SkipEmptyParts );

        for ( int i = 0; i < strList.size(); ++i )
            {
            /********************************************************************
             *      Just accept words between lowestLimitWordSize (4) and
             *      highestLimitWordSize (17) characters (inclusive)
             *******************************************************************/

            if ( lowestLimitWordSize <= strList.at( i ).size() &&
                 strList.at( i ).size() <= highestLimitWordSize )
               {
               bool hadMatch                    = verifyWord( strList.at( i ), myFileOut_Log, regexVec );
               bool hadConsonantsVowelsMatch    = verifyWord( strList.at( i ), myFileOut_Log, regexCV );

               /********************************************************************
                *      here we are interested in hadMatch = false
                *******************************************************************/

               if ( ! hadMatch &&
                    ! hadConsonantsVowelsMatch )
                  {
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
   *    Inserts line = "c++" that corresponds to a bunch of books.
   *    That's because the plus sign (+) is considered as a separator
   *    char so it is stripped out from the output
   *******************************************************************/

  myFileOut_WordsOnly << "c++" << Qt::endl;
  ++goodWordsCount;

  /********************************************************************
   *                    T O T A L S
   *******************************************************************/

    qInfo() << "(" << proj << ")"            
            << "Records read                   =  "
            << readCount
            << Qt::endl;

    qInfo() << "(" << proj << ")"
            << "Accepted Words                 =  "
            << goodWordsCount
            << Qt::endl;

    qInfo() << "(" << proj << ")"
            << "All Books count                =  "
            << AllBooksCount
            << Qt::endl;

    qInfo() << "(" << proj << ")"            
            << "Rejected Words                 =  "
            << rejectedWordsCount
            << Qt::endl;

    qInfo() << "(" << proj << ")"            
            << "Undesirable Words              =  "
            << matchCount
            << Qt::endl;

  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/********************************************************
*          checkParameter function
********************************************************/
bool
checkParameter( const QString   & dirOrFileName,
                const QChar     & dirOrFile,
                const QString   & proj )
  {
  QFileInfo fi_dirOrFileName( dirOrFileName );

  if ( dirOrFile == 'f' )
     {
     if ( ! ( fi_dirOrFileName.exists() && fi_dirOrFileName.isFile() ) )
        {
         qWarning() << "(" << proj << ")"
                    << Qt::endl
                    << "The file = "
                    << dirOrFileName << " does not exist or "
                    << Qt::endl
                    << "the passed strings does not represent a file."
                    << Qt::endl
                    << "Program will terminate" << Qt::endl;

        return ( fi_dirOrFileName.exists() && fi_dirOrFileName.isFile() );
        }
     }
  else
      {
      if ( ! ( fi_dirOrFileName.exists() && fi_dirOrFileName.isDir() ) )
         {
         qWarning() << "(" << proj << ")"
                    << Qt::endl
                    << "The directory = "
                    << dirOrFileName << " does not exist or "
                    << Qt::endl
                    << "the passed strings does not represent a directory."
                    << Qt::endl
                    << "Program will terminate" << Qt::endl;

         return ( fi_dirOrFileName.exists() && fi_dirOrFileName.isDir() );
         }
      }

  if  ( !  fi_dirOrFileName.isAbsolute() )
     {
     qWarning() << "(" << proj << ")"
                << Qt::endl
                << "The string = "
                << dirOrFileName << " must be an absolute path."
                << Qt::endl
                << "Program will terminate" << Qt::endl;

     return ( fi_dirOrFileName.isAbsolute() );
     }

  return true;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
