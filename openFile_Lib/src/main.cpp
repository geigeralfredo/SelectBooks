#include "OpenFile_LibProj.h"
#include "OpenFile_Lib.h"

#include <QLatin1Char>

int
main( int argc, char *argv[] )
  {
  if ( argc < 3 )
     {
     qDebug() << "Please inform the beautifyer File In and Out. Program will terminate" << Qt::endl;

     return 1;
     }

  QString   beautifyerFileNameIn    = argv[ 1 ];
  QFileInfo fi_beautifyerFileNameIn ( beautifyerFileNameIn );

  QString   beautifyerFileNameOut   = argv[ 2 ];
  QFileInfo fi_beautifyerFileNameOut ( beautifyerFileNameOut );

    qDebug() << "FileIn  = " << beautifyerFileNameIn << Qt::endl;
    qDebug() << "FileOut = " << beautifyerFileNameOut << Qt::endl;

  if ( ! fi_beautifyerFileNameIn.isAbsolute() )
     {
     qDebug() << "The beautifyer Filename In isn't an absolute path. Program will terminate" << Qt::endl;

     return 1;
     }

  if ( ! fi_beautifyerFileNameOut.isAbsolute() )
     {
     qDebug() << "The beautifyer Filename Out isn't an absolute path. Program will terminate" << Qt::endl;

     return 1;
     }

  OpenFile_Lib      beautifyerFileIn( beautifyerFileNameIn,
                                  __FUNCTION__,
                                  "ROTX" );

  OpenFile_Lib      beautifyerFileOut( beautifyerFileNameOut,
                                   __FUNCTION__,
                                   "WOAPTX" );

  QTextStream   myBeautifyerIn( beautifyerFileIn.getMp_File() );
  QTextStream   myBeautifyerOut( beautifyerFileOut.getMp_File() );
  QString       line;
  int           writeCounter    = 0;
  int           readCounter     = 0;

  while ( ! myBeautifyerIn.atEnd() )
        {
        line = myBeautifyerIn.readLine();
        ++readCounter;
        qsizetype   lineSize        = line.size();
        int         lineSizeAsInt   = static_cast <int>( lineSize );

        if ( lineSizeAsInt > 0 &&
             line.at( 0 ) == QLatin1Char( '#' ) )
           {
           myBeautifyerOut << line <<  Qt::endl;
           ++writeCounter;

           }
        else
            {
            myBeautifyerOut << line <<  Qt::endl <<  Qt::endl;
            writeCounter += 2;

            }
        }

    qInfo() << "Records read     = " << readCounter <<  Qt::endl;
    qInfo() << "Records written  = " << writeCounter <<  Qt::endl;

  }
