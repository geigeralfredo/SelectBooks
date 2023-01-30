#ifndef ERRORCODEANDMSG_LIB_H
#define ERRORCODEANDMSG_LIB_H

#include "errorCodeAndMsg_Lib_global.h"

#include "errorCodeAndMsg_LibProj.h"

class ERRORCODEANDMSG_LIB_EXPORT ErrorCodeAndMsg_Lib
  {
  public:

    ErrorCodeAndMsg_Lib( const int      & errorCode = 0,
                         const QString  & errorMsg = QString() );

    ErrorCodeAndMsg_Lib ( const ErrorCodeAndMsg_Lib & copy );

    ErrorCodeAndMsg_Lib &
    operator=( const ErrorCodeAndMsg_Lib & assign );

    virtual
    ~ErrorCodeAndMsg_Lib();

    void
            display();

    int
            getErrorCode() const;

    void
            setErrorCode( int newErrorCode );

    const QString &
            getMessage() const;

    void
            setMessage( const QString & newMessage );

    QString &
            timeStamp();

  private:

    int m_ErrorCode;
    QString m_Message;
  };

#endif // ERRORCODEANDMSG_LIB_H
