/********************************************************************
 *         Algorithm to generate phonetic code
 *******************************************************************/
#include "soundexProj.h"
#include "openFile_Lib.h"
#include "elapsedtime_Lib.h"
#include "logger_lib.h"

void
tellTheFunctionName( const QString & function, const QString & lineNr );

QString
ParseWord( const QString & word, int codeLength = 4 );

QChar
ParseChar( QChar c );

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *            main
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
int
main ( int argc, const char *argv[] )
  {
    const QString logType = "RELEASE";
    QString proj {"Soundex"};

    Logger_Lib::init(proj, logType );

  ElapsedTime_Lib   timeIntervalObj;

  /********************************************************************
   *         Parameter validation
   *******************************************************************/

  if ( argc < 4 )
     {
     qInfo()    << "(" << proj << ")"
                << "You should inform 3 parameters: "
                << Qt::endl
                << "The 1rst one is MANDATORY, is the inFile with words to be encoded."
                << Qt::endl
                << "The 2nd is also MANDATORY, is the outFile with the input words plus their phonetic codes."
                << Qt::endl
                << "The 3rd is OPTIONAL, is an Integer (default=4) that defines the length of the phonetic code."
                << Qt::endl
                << "Program will terminate "
                << Qt::endl;

     return 1;
     }

  /********************************************************************
   *            Verifying Code Length
   *  if codeLength NOT numeric make codeLength = 5
   *  if codeLength numeric and ( 4 <= codeLength <= 7 ) use it
   *  if codeLength is outside the range, default codeLength = 5
   *******************************************************************/

  static const QRegularExpression   regex_OnlyNumbers( "\\d*" );              // only numbers
  static const QRegularExpression   regex_OnlyLetters( "^[a-zA-ZÀ-ž+]*$" );    // only letters

  QString                           codeLengthAsString;
  int                               codeLengthAsInt = 4;        // default value

  if ( argc >= 4 )
     {
     codeLengthAsString = argv[ 3 ];                            // conde length was informed
     QRegularExpressionMatch    match_OnlyNumbers       = regex_OnlyNumbers.match( codeLengthAsString );
     bool                       hasMatch_OnlyNumbers    = match_OnlyNumbers.hasMatch();         // true

     if ( hasMatch_OnlyNumbers )
        {
        codeLengthAsInt = codeLengthAsString.toInt();

        if ( ! ( 4 <= codeLengthAsInt && codeLengthAsInt <= 7 ) )
          codeLengthAsInt = 4;
        }
     }

  /********************************************************************
   *         File definitions
   *******************************************************************/

  QString   wordsFileIn         = argv[ 1 ];
  QString   encodedWordsFileOut = argv[ 2 ];

  QFileInfo fi_wordsFileIn( wordsFileIn );
  QFileInfo fi_wordsFileOut( encodedWordsFileOut );

  if ( ! fi_wordsFileIn.isAbsolute() ||
       ! fi_wordsFileOut.isAbsolute()
     )
     {
     qInfo()    << "(" << proj << ")"
                << Qt::endl
                << "The informed files must be absolute file paths."
                << Qt::endl
                << "Program will terminate"
                << Qt::endl;

     return EXIT_FAILURE;
     }

  /********************************************************************
   *         File In
   *******************************************************************/

  OpenFile_Lib  FileIn_Words( wordsFileIn, proj, "ROTX" );
  QTextStream   myFileIn_Words( FileIn_Words.getMp_File() );

  /********************************************************************
   *         File Out
   *******************************************************************/

  OpenFile_Lib  FileOut_EncodedWords( encodedWordsFileOut, proj, "WOTRTX" );
  QTextStream   myFileOut_EncodedWords( FileOut_EncodedWords.getMp_File() );

  /********************************************************************
   *            Working fields
   *******************************************************************/

  QString       originalLine;
  QString       encodedWord;
  QString       recordOut;

  // QStringList   wordList;

  int           recordsRead     { 0 };
  int           recordsWritten  { 0 };

  // get name to encode
  while ( ! myFileIn_Words.atEnd() )
        {
        originalLine = myFileIn_Words.readLine();
        ++recordsRead;

        QRegularExpressionMatch match_OnlyLetters       = regex_OnlyLetters.match( originalLine );
        bool                    hasMatch_OnlyLetters    = match_OnlyLetters.hasMatch();            // true

        if ( ! hasMatch_OnlyLetters )
           {
           qInfo()  << "(" << proj << ")"
                    << " - The word = "
                    << originalLine
                    << Qt::endl
                    << " has extraneous chars. The program will terminate."
                    << Qt::endl;

           return EXIT_FAILURE;
           }

        encodedWord = ParseWord( originalLine, codeLengthAsInt );

        /********************************************************************
         *            Just for debugging
         *******************************************************************/

//        qDebug() << "(" << proj << ")"
//                << " code for "
//                << originalLine
//                << " is "
//                << encodedWord
//                << Qt::endl;
        /*******************************************************************/

        recordOut = originalLine + " " + encodedWord;

        myFileOut_EncodedWords << recordOut << Qt::endl;
        ++recordsWritten;
        }

    qInfo() << "(" << proj << ")"
            << "Records Read    =  "
            << recordsRead
            << Qt::endl;

    qInfo() << "(" << proj << ")"
            << "Records Written =  "
            << recordsWritten
            << Qt::endl;

  return EXIT_SUCCESS;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Returns an encoded QString according to the
 * Soundex algorithm.
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
QString
ParseWord( const QString & word, int codeLength )
  {
  QString   withoutDuplicate;
  QString   nameUpperCase = word.toUpper();

  /********************************************************************
   *            Remove duplicate chars
   *******************************************************************/

  int       i { 0 };
  QChar     previous;

//  /********************************************************************
//   *            Just for debugging
//   *******************************************************************/
//    qDebug() << "nameUpperCase.length() = " << nameUpperCase.length()  << Qt::endl;
//   /*******************************************************************/

  while ( i < nameUpperCase.length() )
        {
        if ( i == 0 )
           {
           previous         = nameUpperCase.at( i );
           withoutDuplicate += nameUpperCase.at( i++ );
           }
        else
            {
            if ( nameUpperCase.at( i ) == previous )
               {
               ++i;
               }
            else
                {
                previous            = nameUpperCase.at( i );
                withoutDuplicate    += nameUpperCase.at( i++ );
                }
            }
        }

//  /********************************************************************
//   *            Just for debugging
//   *******************************************************************/

//  qDebug() << "withoutDuplicate = " <<  withoutDuplicate << Qt::endl;
//  /*******************************************************************/

  QString code;

  for ( int i = 0; i < withoutDuplicate.length(); ++i )
      {
      if ( i == 0 )
        code += withoutDuplicate.at( i );   // save first char

      if ( i > 0 && withoutDuplicate.at( i ).isLetter() )
        code += ParseChar( withoutDuplicate.at( i ) );
      }

  if ( code.length() < codeLength )
     {
     int zeroPadding = codeLength - code.length();

     for ( int k = 0; k < zeroPadding; ++k )
         {
         code += '0';     // padd zeros if code is too short
         }
     }
  else
      {
      if ( code.length() > codeLength )
        code.truncate( codeLength );   // truncate if code is too long

      }

//  /*******************************************************************
//  *           just for debugging
//  *******************************************************************/
//  qDebug () << "code = " << code << Qt::endl;
//  /*******************************************************************/

  return code;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *          Map for ParseChar function
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const QMap <QString, int> convertLetterToPhoneticCodeMap =
  {
    { "A", 0 },
    { "B", 1 },
    { "C", 2 },
    { "D", 3 },
    { "E", 0 },
    { "F", 1 },
    { "G", 2 },
    { "H", 0 },
    { "I", 0 },
    { "J", 2 },
    { "K", 2 },
    { "L", 5 },
    { "M", 4 },
    { "N", 4 },
    { "O", 0 },
    { "P", 1 },
    { "Q", 2 },
    { "R", 6 },
    { "S", 2 },
    { "T", 3 },
    { "U", 0 },
    { "V", 1 },
    { "W", 0 },
    { "X", 2 },
    { "Y", 0 },
    { "Z", 2 }
  };

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *          ParseChar
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
QChar
ParseChar( QChar c )
  {
  if ( 'A' <= c && c <= 'Z' )
     {
     int        phoneticCodeAsInt       = convertLetterToPhoneticCodeMap.value( c );
     QString    phoneticCodeAsString    = QString::number( phoneticCodeAsInt );

     return phoneticCodeAsString.at( 0 );
     }
  else
      {
      return '6';
      }
  }
