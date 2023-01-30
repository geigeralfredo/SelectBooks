#ifndef OPENFILE_LIB_H
#define OPENFILE_LIB_H

#include "openFile_Lib_global.h"

#include "openFile_LibProj.h"
#include "errorCodeAndMsg_Lib.h"

class OPENFILE_LIB_EXPORT OpenFile_Lib
  {
  public:

    explicit
    OpenFile_Lib( const QString & fileName,
                  const QString & Caller,
                  const QString & openMode = "ROTX" );

    ~OpenFile_Lib();

    OpenFile_Lib ( const OpenFile_Lib & copy );

    OpenFile_Lib &
    operator=( const OpenFile_Lib & assign );

    ErrorCodeAndMsg_Lib
            open();

    const QString &
            FileName() const;

    void
            setFileName( const QString & newFileName );

    const QString &
            OpenMode() const;

    void
            setOpenMode( const QString & newOpenMode );

    const QString &
            Caller() const;

    void
            setCaller( const QString & newCaller );

    QFile *
            getMp_File() const;

    void
            setMp_File( QFile *newMp_File );

    QString &
            timeStamp();

  private:

    QString m_FileName;
    QString m_Caller;
    QString m_OpenMode;
    QFile *mp_File;

    enum class EOpenModeType
      {
      eRW, eRWTX, eRWAP, eRWTR, eRWAPTX, eRWTRTX,
      eRO, eROTX, eWO, eWOTX, eWOAP, eWOTR, eWOAPTX, eWOTRTX
      };

    static inline const QMap < QString, EOpenModeType > translate2OpenModeMap =
      {
        {
          { "RW",     EOpenModeType::eRW                                         },
          { "RWTX",   EOpenModeType::eRWTX                                       },
          { "RWAP",   EOpenModeType::eRWAP                                       },
          { "RWTR",   EOpenModeType::eRWTR                                       },
          { "RWAPTX", EOpenModeType::eRWAPTX                                     },
          { "RWTRTX", EOpenModeType::eRWTRTX                                     },
          { "RO",     EOpenModeType::eRO                                         },
          { "ROTX",   EOpenModeType::eROTX                                       },
          { "WO",     EOpenModeType::eWO                                         },
          { "WOTX",   EOpenModeType::eWOTX                                       },
          { "WOAP",   EOpenModeType::eWOAP                                       },
          { "WOTR",   EOpenModeType::eWOTR                                       },
          { "WOAPTX", EOpenModeType::eWOAPTX                                     },
          { "WOTRTX", EOpenModeType::eWOTRTX                                     }
        }
      };
  };

#endif // OPENFILE_LIB_H
