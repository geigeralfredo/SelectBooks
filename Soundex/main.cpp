/********************************************************************
 *         Algorithm to generate phonetic code
 *******************************************************************/
#include "soundexProj.h"
#include "openFile_Lib.h"

void
tellTheFunctionName( const QString & function, const QString & lineNr );

QString
ParseWord( QString word, int codeLength = 5 );

QChar
ParseChar( QChar c );

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *            main
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
int
main ( int argc, char *argv[] )
  {
  /********************************************************************
   *            Tell the name of the function we are in
   *******************************************************************/
  tellTheFunctionName( QString( __FUNCTION__ ), QString::number( __LINE__ ) );

  /********************************************************************
   *         Parameter validation
   *******************************************************************/

  if ( argc < 2 )
     {
     qInfo()    << "Soundex - Function ( "
                << __FUNCTION__
                << " ) "
                << "You may inform 2 parameters: "
                << Qt::endl
                << "The 1rst is MANDATORY, it is the File with words to be encoded."
                << Qt::endl
                << "The 2nd is OPTIONAL, is an Integer (default=5) the length of the phonetic code."
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

  static const QRegularExpression   re( "\\d*" );  // a digit (\d), zero or more times (*)

  QString                           codeLengthAsString;
  int                               codeLengthAsInt = 5;        // default value

  if ( argc >= 3 )
     {
     codeLengthAsString = argv[ 2 ];                            // conde length was informed
     QRegularExpressionMatch    match       = re.match( codeLengthAsString );
     bool                       hasMatch    = match.hasMatch(); // true

     if ( hasMatch )
        {
        codeLengthAsInt = codeLengthAsString.toInt();

        if ( ! ( 4 <= codeLengthAsInt && codeLengthAsInt <= 7 ) )
          codeLengthAsInt = 5;
        }
     }

  /********************************************************************
   *         File definitions
   *******************************************************************/

  QString   wordsFileIn = argv[ 1 ];

  QFileInfo fi_wordsFileIn( wordsFileIn );

  if ( ! fi_wordsFileIn.isAbsolute() )
     {
     qInfo()    << "Soundex - Function (" << __FUNCTION__ << ") "
                << Qt::endl
                << "File name must be absolute file path."
                << Qt::endl
                << "Program will terminate"
                << Qt::endl;

     return EXIT_FAILURE;
     }

  /********************************************************************
   *         File In
   *******************************************************************/

  OpenFile_Lib  FileIn_Words( wordsFileIn, __FUNCTION__, "ROTX" );
  QTextStream   myFileIn_Words( FileIn_Words.getMp_File() );

  /********************************************************************
   *         Hard coded File Names
   *******************************************************************/

  QString       encodedWordsFileOut = "/home/alfredo/Documents/TXT/selectedBooks_EncodedWords.txt";

  /********************************************************************
   *         File Out
   *******************************************************************/

  OpenFile_Lib  FileOut_EncodedWords( encodedWordsFileOut, __FUNCTION__, "WOTRTX" );
  QTextStream   myFileOut_EncodedWords( FileOut_EncodedWords.getMp_File() );

  /********************************************************************
   *            Working fields
   *******************************************************************/

  QString       originalLine;
  QString       encodedWord;
  QString       recordOut;
  QStringList   wordList;

  int           recordsRead     = 0;
  int           recordsWritten  = 0;

  // get name to encode
  while ( ! myFileIn_Words.atEnd() )
        {
        originalLine    = myFileIn_Words.readLine();
        ++recordsRead;

        wordList        = originalLine.split( " " );

//        /********************************************************************
//         *            Just for debugging
//         *******************************************************************/
//        qInfo() << "wordList = " << wordList;
//        foreach( QString item, wordList )
//        qDebug() << "wordList items = " << item;
//        /********************************************************************/

        encodedWord     = ParseWord( wordList.at( 2 ), codeLengthAsInt );

        qInfo() << "Soundex code for " << wordList[ 2 ]
                << " is " << encodedWord << Qt::endl;

        recordOut       = originalLine + " " + encodedWord;

        myFileOut_EncodedWords << recordOut << Qt::endl;
        ++recordsWritten;
        }

    qInfo() << "Soundex - Function (" << __FUNCTION__ << ") "
            << "Records Read    =  "
            << recordsRead
            << Qt::endl;

    qInfo() << "Soundex - Function (" << __FUNCTION__ << ") "
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
ParseWord( QString word, int codeLength )
  {
  /********************************************************************
   *            Tell the name of the function we are in
   *******************************************************************/
  tellTheFunctionName( QString( __FUNCTION__ ), QString::number( __LINE__ ) );

  QString   withoutDuplicate;
  QString   nameUpperCase = word.toUpper();

  /********************************************************************
   *            Remove duplicate chars
   *******************************************************************/

  int       i { 0 };
  QChar     previous;

  //        /********************************************************************
  //         *            Just for debugging
  //         *******************************************************************/
  //        qInfo() << "nameUpperCase.length() = " << nameUpperCase.length()  << Qt::endl;
  //         *******************************************************************/

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

  //        /********************************************************************
  //         *            Just for debugging
  //         *******************************************************************/
  //        qInfo() << "withoutDuplicate = " <<  withoutDuplicate << Qt::endl;
  //         *******************************************************************/

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

  return code;
  }

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *          ParseChar
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

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *          tellTheFunctionName
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
void
tellTheFunctionName( const QString & function, const QString & lineNr )
  {
  QString functionName =  "FUNCTION = " + function + " " + lineNr;
  qInfo() << functionName << Qt::endl;
  }
