#include "openFile_Lib.h"

OpenFile_Lib::OpenFile_Lib( const QString   & fileName,
                            const QString   & Caller,
                            const QString   & openMode )
  : m_FileName( fileName ),
  m_Caller( Caller ),
  m_OpenMode( openMode ),
  mp_File( new QFile( m_FileName ) )
  {
  open();
  }

OpenFile_Lib::~OpenFile_Lib()
  {
  mp_File->close();
  mp_File = nullptr;
  }

OpenFile_Lib::OpenFile_Lib( const OpenFile_Lib & copy )
  : m_FileName( copy.FileName() ),
  m_Caller( copy.Caller() ),
  m_OpenMode( copy.OpenMode() ),
  mp_File( copy.getMp_File() )
  {
  }

OpenFile_Lib &
OpenFile_Lib::operator=( const OpenFile_Lib & assign )
  {
  if ( this != &assign )
     {
     m_FileName = assign.FileName();
     m_Caller   = assign.Caller();
     m_OpenMode = assign.OpenMode();
     mp_File    = assign.getMp_File();
     }

  return *this;
  }

ErrorCodeAndMsg_Lib
OpenFile_Lib::open()
  {
  QString               functionName    = timeStamp() + " - " + __FUNCTION__ + " " + QString::number( __LINE__ );

  EOpenModeType         eOpenMode       = translate2OpenModeMap.value( m_OpenMode );

  bool                  isItOpened      = false;

  ErrorCodeAndMsg_Lib   retCodeAndMsgObj;

  switch ( eOpenMode )
         {
             case EOpenModeType::eRW:
             isItOpened = mp_File->open( QIODevice::ReadWrite );
             break;

             case EOpenModeType::eRWTX:
             isItOpened = mp_File->open( QIODevice::ReadWrite | QIODevice::Text );
             break;

             case EOpenModeType::eRWAP:
             isItOpened = mp_File->open( QIODevice::ReadWrite | QIODevice::Append );
             break;

             case EOpenModeType::eRWTR:
             isItOpened = mp_File->open( QIODevice::ReadWrite | QIODevice::Truncate );
             break;

             case EOpenModeType::eRWAPTX:
             isItOpened = mp_File->open( QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text );
             break;

             case EOpenModeType::eRWTRTX:
             isItOpened = mp_File->open( QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text );
             break;

             case EOpenModeType::eRO:
             isItOpened = mp_File->open( QIODevice::ReadOnly );
             break;

             case EOpenModeType::eROTX:
             isItOpened = mp_File->open( QIODevice::ReadOnly | QIODevice::Text );
             break;

             case EOpenModeType::eWO:
             isItOpened = mp_File->open( QIODevice::WriteOnly );
             break;

             case EOpenModeType::eWOTX:
             isItOpened = mp_File->open( QIODevice::WriteOnly | QIODevice::Text );
             break;

             case EOpenModeType::eWOAP:
             isItOpened = mp_File->open( QIODevice::WriteOnly | QIODevice::Append );
             break;

             case EOpenModeType::eWOTR:
             isItOpened = mp_File->open( QIODevice::WriteOnly | QIODevice::Truncate );
             break;

             case EOpenModeType::eWOAPTX:
             isItOpened = mp_File->open( QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text );
             break;

             case EOpenModeType::eWOTRTX:
             isItOpened = mp_File->open( QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text );
             break;

             default:
             retCodeAndMsgObj.setErrorCode( EXIT_FAILURE );
             retCodeAndMsgObj.setMessage( functionName + QString( "- Open Mode %1 is invalid" )
                                .arg( m_OpenMode ) );
             retCodeAndMsgObj.display();

             return retCodeAndMsgObj;

             break;
         }

  if ( ! isItOpened )
     {
     qInfo()    << functionName
                << "File not openned "
                << Qt::endl
                << "File Name = " << m_FileName
                << Qt::endl
                << "Caller            = " << m_Caller
                << Qt::endl
                << "Open Mode         = " << m_OpenMode
                << Qt::endl;

     retCodeAndMsgObj.setErrorCode( EXIT_FAILURE );
     retCodeAndMsgObj.setMessage( functionName + QString( "%1 - Could not open %2 " )
                        .arg( m_Caller, m_FileName ) );
     retCodeAndMsgObj.display();

     return retCodeAndMsgObj;
     }

  retCodeAndMsgObj.setErrorCode( EXIT_SUCCESS );
  retCodeAndMsgObj.setMessage( functionName + " - is OK - All is Well!!" );
  retCodeAndMsgObj.display();

  return retCodeAndMsgObj;
  }

const QString &
OpenFile_Lib::FileName() const
  {
  return m_FileName;
  }

void
OpenFile_Lib::setFileName( const QString & newFileName )
  {
  m_FileName = newFileName;
  }

const QString &
OpenFile_Lib::OpenMode() const
  {
  return m_OpenMode;
  }

void
OpenFile_Lib::setOpenMode( const QString & newOpenMode )
  {
  m_OpenMode = newOpenMode;
  }

const QString &
OpenFile_Lib::Caller() const
  {
  return m_Caller;
  }

void
OpenFile_Lib::setCaller( const QString & newCaller )
  {
  m_Caller = newCaller;
  }

QFile *
OpenFile_Lib::getMp_File() const
  {
  return mp_File;
  }

void
OpenFile_Lib::setMp_File( QFile *newMp_File )
  {
  mp_File = newMp_File;
  }

QString &
OpenFile_Lib::timeStamp()
  {
  QDateTime         MyTimeStamp     = QDateTime::currentDateTime();
  static QString    strTimeStamp    = MyTimeStamp.toString( "yyyy.MM.dd-HH:mm:ss" );

  return strTimeStamp;
  }
