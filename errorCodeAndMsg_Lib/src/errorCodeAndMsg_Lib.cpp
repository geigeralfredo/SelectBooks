#include "errorCodeAndMsg_Lib.h"

ErrorCodeAndMsg_Lib::ErrorCodeAndMsg_Lib( const int     & errorCode,
                                          const QString & errorMsg )
  : m_ErrorCode{ errorCode },
  m_Message{ errorMsg }
  {
  }

ErrorCodeAndMsg_Lib::ErrorCodeAndMsg_Lib( const ErrorCodeAndMsg_Lib & copy )
  : m_ErrorCode{ copy.getErrorCode() },
  m_Message{ copy.getMessage() }
  {
  }

ErrorCodeAndMsg_Lib &
ErrorCodeAndMsg_Lib::operator=( const ErrorCodeAndMsg_Lib & assign )
  {
  if ( this != &assign )
     {
     m_ErrorCode    = assign.getErrorCode();
     m_Message      = assign.getMessage();
     }

  return *this;
  }

ErrorCodeAndMsg_Lib::~ErrorCodeAndMsg_Lib()
  {
  }

void
ErrorCodeAndMsg_Lib::display()
  {
  QString   functionName    = timeStamp() + " - " + __FUNCTION__ + " " + QString::number( __LINE__ );

  QString   fileName        = "/tmp/temporary" + timeStamp() + ".txt";

  // qInfo() << "ErrorCodeAndMsg_Lib::display - fileName = " << fileName << Qt::endl;

  if ( ! fileName.isEmpty() )                                   // Most used option

     {
     QFile          file( fileName );
     QTextStream    outFile( &file );

     if ( file.open( QIODevice::WriteOnly | QIODevice::Append ) )
        {
        QString record1 = functionName + " - Exit Code = " + QString::number( m_ErrorCode );
        QString record2 = functionName + " - Message   = " + m_Message;

        outFile << record1 << Qt::endl;
        outFile << record2 << Qt::endl;

        file.close();
        }
     else                                                       // Here we have an open error
         {
         QFileDevice::FileError openErrorCode       = file.error();
         QString                fileOpenMsgError    = file.errorString();

            qDebug()    << functionName
                        << " - Open Error Code    = " << openErrorCode << Qt::endl;
            qDebug()    << functionName
                        << " - Open Error Message = " << fileOpenMsgError << Qt::endl;
            qDebug()    << functionName
                        << " - Error Code         = " << m_ErrorCode << Qt::endl;
            qDebug()    << functionName
                        << " - Error Message      = " << m_Message << Qt::endl;
         }
     }
  else                                                          // Here file name is empty
      {
        qDebug()    << functionName
                    << " - Error Code      = " << m_ErrorCode << Qt::endl;
        qDebug()    << functionName
                    << " - Error Message   = " << m_Message << Qt::endl;
      }
  }

int
ErrorCodeAndMsg_Lib::getErrorCode() const
  {
  return m_ErrorCode;
  }

void
ErrorCodeAndMsg_Lib::setErrorCode( int newErrorCode )
  {
  m_ErrorCode = newErrorCode;
  }

const QString &
ErrorCodeAndMsg_Lib::getMessage() const
  {
  return m_Message;
  }

void
ErrorCodeAndMsg_Lib::setMessage( const QString & newMessage )
  {
  m_Message = newMessage;
  }

QString &
ErrorCodeAndMsg_Lib::timeStamp()
  {
  QDateTime         MyTimeStamp     = QDateTime::currentDateTime();
  static QString    strTimeStamp    = MyTimeStamp.toString( "yyyy.MM.dd-HH:mm:ss:zzz" );

  return strTimeStamp;
  }
