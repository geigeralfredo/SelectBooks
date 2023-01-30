#include <constants.h>

/*--------------------------------------------------------------------
 *       helpText
 *--------------------------------------------------------------------*/

const QString &
Constants::helpText()
  {
  return s_helpText;
  }

/*--------------------------------------------------------------------
 *       funcNameAndRetString
 *--------------------------------------------------------------------*/

void
Constants::funcNameAndRetString( const QString & funcName, const QString & string )
  {
  qInfo()   << "Constants::"
            << funcName
            << " Composed String = "
            << string
            << Qt::endl;
  }

/*--------------------------------------------------------------------
 *       errorCodeAndErrorMsg
 *--------------------------------------------------------------------*/

ErrorCodeAndMsg_Lib
Constants::errorCodeAndErrorMsg( const QString & keyString )
  {
  QString               valueString     = errorCodeAndMsgMap.value( keyString );

  bool                  isNumeric;
  int                   retCode         = valueString.split( " " )[ 0 ].toInt( &isNumeric );

  QString               errorMessage    = valueString.last( valueString.size() - 3 );

  if ( ! isNumeric )
     {
     retCode        = 1;
     errorMessage   = "Error Code Not Numeric";
     }

  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;
  retCodeAndMsgObj.setErrorCode( retCode );
  retCodeAndMsgObj.setMessage( errorMessage );
  retCodeAndMsgObj.display();

  return retCodeAndMsgObj;

  }

/*--------------------------------------------------------------------
 *       timeStamp
 *--------------------------------------------------------------------*/

QString &
Constants::timeStamp()
  {
  QDateTime         MyTimeStamp     = QDateTime::currentDateTime();
  static QString    strTimeStamp    = MyTimeStamp.toString( "yyyy.MM.dd-HH:mm:ss:zzz" );

  return strTimeStamp;
  }

/*--------------------------------------------------------------------
 *       Retrieve Literal
 *--------------------------------------------------------------------*/

QString
Constants::retrieveLiteral( const QString & keyString )
  {
  QString valueString = cmdMap.value( keyString );
  funcNameAndRetString( __FUNCTION__, valueString );

  return valueString;
  }

/*--------------------------------------------------------------------
 *       Command Construction Functions
 *--------------------------------------------------------------------
 *                 findCmd
 *--------------------------------------------------------------------*/

QString &
Constants::findCmd( const QString & dirOfBookCollection )
  {
  static QString s_string = retrieveLiteral( "findCmdStart" ) +
                            " " +
                            dirOfBookCollection +
                            " " +
                            retrieveLiteral( "findCmdEnd" );
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }

/*--------------------------------------------------------------------
 *                 bashCmdAppendTo
 *--------------------------------------------------------------------*/

QString &
Constants::bashCmdAppendTo( const QString & fileTotals )
  {
  static QString s_string = retrieveLiteral( "equalSignLine" ) +
                            " " +
                            retrieveLiteral( "bashCmdAppendTo" ) +
                            fileTotals;
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }

/*--------------------------------------------------------------------
 *                 sortCmdComplement2
 *--------------------------------------------------------------------*/

QString &
Constants::sortCmdComplement2()
  {
  static QString s_string = retrieveLiteral( "tmpFileName2" ) +
                            " " +
                            retrieveLiteral( "bashCmdWriteTo" );
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }

/*--------------------------------------------------------------------
 *                 sortCmdComplement3
 *--------------------------------------------------------------------*/

QString &
Constants::sortCmdComplement3()
  {
  static QString s_string = retrieveLiteral( "tmpFileName3" ) +
                            " " +
                            retrieveLiteral( "bashCmdWriteTo" );
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }

/*--------------------------------------------------------------------
 *                 sortCmdWhole2
 *--------------------------------------------------------------------*/

QString &
Constants::sortCmdWhole2()
  {
  static QString s_string = retrieveLiteral( "sortCmd" ) +
                            " " +
                            sortCmdComplement2();
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }

/*--------------------------------------------------------------------
 *                 sortCmdWhole3
 *--------------------------------------------------------------------*/

QString &
Constants::sortCmdWhole3()
  {
  static QString s_string = retrieveLiteral( "sortCmd" ) +
                            " " +
                            sortCmdComplement3();
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }

/*--------------------------------------------------------------------
 *                 writeToTemp
 *--------------------------------------------------------------------*/

QString &
Constants::writeToTemp()
  {
  static QString s_string = retrieveLiteral( "bashCmdWriteTo" ) +
                            " " +
                            retrieveLiteral( "tmpFileName" );
  funcNameAndRetString( __FUNCTION__, s_string );

  return s_string;
  }
