/********************************************************
*    PURPOSE:
*    Read the file with a list of All Books and
*    create 2 files:
*    One with all the accepted extensions with the number of
*    its occurrences and
*    Another with the rejected extensions.
*    With that information we can evaluate the importance
*    of each extension.
*    ARGS:
*    1 - AllBooks file name
*    2 - A file that will contain the selectedBooks_Extensions
*    3 - A file that will contain the selectedBooks_RejectedExtensions
*    Every extension with  1 <= length <= 5 will be accepted
********************************************************/
#include "getExtensionsProj.h"

#include "elapsedtime_Lib.h"
#include "openFile_Lib.h"
#include "logger_lib.h"

/********************************************************
*          Conversion Map for E-Books,
*          Images, Videos and Audio to get
*          their totals
********************************************************/
static const QMap <QString, int> conversionMap
{
  /********************************************************
  *          E-Books
  ********************************************************/
  { "azw", 0 },
  { "azw3", 0 },
  { "cbr", 0 },
  { "chm", 0 },
  { "djv", 0 },
  { "djvu", 0 },
  { "doc", 0 },
  { "docx", 0 },
  { "epub", 0 },
  { "fb2", 0 },
  { "lit", 0 },
  { "mobi", 0 },
  { "pdf", 0 },
  { "rtf", 0 },
  { "txt", 0 },

  /********************************************************
  *          Imagens
  ********************************************************/
  { "gif", 1 },
  { "jpeg", 1 },
  { "jpg", 1 },
  { "png", 1 },
  { "tif", 1 },

  /********************************************************
  *          Videos
  ********************************************************/
  { "flv", 2 },
  { "mov", 2 },
  { "mp4", 2 },
  { "swf", 2 },
  { "mkv", 2 },

  /********************************************************
  *          Audio
  ********************************************************/
  { "mp3", 3 }
};

/********************************************************
*          returns the Project Name
********************************************************/

QString
getExtensions( const QString & project )
{
  if ( project == "main" )
    return __FUNCTION__;
  else
    return project;
}

/********************************************************
*          checkParameters helper function
********************************************************/
bool
checkParameter( const QString & fileName, const QString & project = "checkParameter" );

/********************************************************
*          main Function
********************************************************/

int
main( int argc, char *argv[] )
{
  const QString logType {"RELEASE"};
  const QString proj {"getExtensions"};

  Logger_Lib::init(proj, logType );

  ElapsedTime_Lib timeIntervalObj;

  if ( argc < 4 )
     {
     qWarning() << "Proj(" << proj << ")"
              << "Please inform:"
              << Qt::endl
              << "The infile with the list of all books"
              << Qt::endl
              << "The outfile that will receive all extensions found"
              << Qt::endl
              << "The outfile that will receive all the rejected extensions"
              << Qt::endl
              << "Program will terminate"
              << Qt::endl;

     return 1;
     }

  QString allBooksFileNameIn          = argv[ 1 ];
  QString allBooksFileNameOut01       = argv[ 2 ];
  QString RejectedExtensionsFileName  = argv[ 3 ];

  bool isFile1Ok                   = checkParameter( argv[ 1 ] );
  bool isFile2Ok                   = checkParameter( argv[ 2 ] );
  bool isFile3Ok                   = checkParameter( argv[ 3 ] );

  if ( ( ! isFile1Ok ) ||
       ( ! isFile2Ok ) ||
       ( ! isFile3Ok )
        )
    return 2;

  /********************************************************
  *          File definitions
  ********************************************************/
  OpenFile_Lib allBooksFileIn( allBooksFileNameIn, proj, "ROTX" );
  OpenFile_Lib allBooksFileOut01( allBooksFileNameOut01, proj, "WOTRTX" );
  OpenFile_Lib RejectedExtensionsFileOut( RejectedExtensionsFileName, proj, "WOTRTX" );

  QTextStream myAllBooksIn( allBooksFileIn.getMp_File() );
  QTextStream myAllBooksOut01( allBooksFileOut01.getMp_File() );
  QTextStream myRejectedExtensionsFileOut( RejectedExtensionsFileOut.getMp_File() );

  int booksRead            { 0 };
  int extensionsSummary    { 0 };
  int ignoredRecs          { 0 };

  QString currentLine;
  QString extension;

  QHash <QString, int>  myHashTable;
  myHashTable.reserve( 2000 );
  int matchCount { 0 };

  /********************************************************
  *          List of integers to get the totals
  *          of: E-Books, Images, Videos and Audio
  ********************************************************/
  QList <int>           resourceTotals( 4 );
  resourceTotals.fill( 0 );

  while ( ! myAllBooksIn.atEnd() )
        {
        /********************************************************
        *          read a line
        ********************************************************/
        currentLine = myAllBooksIn.readLine();
        ++booksRead;

        /********************************************************
        *          ignore zero size lines
        ********************************************************/
        if ( currentLine.size() == 0 )
           {
           ++ignoredRecs;
           continue;
           }

        /********************************************************
        *          here we have an ordinary line
        ********************************************************/
        QFileInfo fi( currentLine );
        extension   = fi.suffix();

        extension   = extension.toLower();

        if ( extension.size() == 0 )
//           qInfo()   << "Proj(" << proj << ")"
//                    << " Line with no Extension = " << currentLine
//                    << Qt::endl;
          continue;

        if ( ( extension.size() == 1 &&
               ( extension != "c" &&
                 extension != "h"
               )
               ) ||
             extension.size() > 6 )
           {
           ++ignoredRecs;

           myRejectedExtensionsFileOut << "Extension Size = "
                                       << extension.size()
                                       << "   Extension   = "
                                       << extension
                                       << Qt::endl;

           continue;
           }

        /********************************************************
        *          get the totals for the number of
        *          E-Books, Images, Videos and Audio
        ********************************************************/
        QMap <QString, int>::const_iterator map_cItr = conversionMap.find ( extension );

        if ( map_cItr != conversionMap.end() )
          ++resourceTotals [ map_cItr.value() ];

        matchCount = myHashTable.value( extension );
        myHashTable.insert( extension, ++matchCount );

        }

  QHashIterator <QString, int>  myHashIter( myHashTable );
  int totalExtCount { 0 };

  while ( myHashIter.hasNext() )
        {
        myHashIter.next();

        myAllBooksOut01 << qSetFieldWidth( 10 )
                        << Qt::left
                        << myHashIter.key()
                        << Qt::right
                        << myHashIter.value()
                        << Qt::endl;

        ++extensionsSummary;
        totalExtCount += myHashIter.value();

        }

  /********************************************************
  *          T o t a l s    listings
  ********************************************************/
  qInfo()  << "Proj(" << proj << ")"
          << " Books that were read        =  " << booksRead
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Qty of different extensions =  " << extensionsSummary
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Total extension count       =  " << totalExtCount
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Records ignored             =  " << ignoredRecs
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Total                       =  " << totalExtCount + ignoredRecs
          << Qt::endl;

  qInfo() << "====================================================================="
          << Qt::endl;

  qInfo() << "Proj(" << proj << ")"
          << " E-Books Total         =  " << resourceTotals [ 0 ]
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Images Total          =  " << resourceTotals [ 1 ]
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Videos Total          =  " << resourceTotals [ 2 ]
          << Qt::endl;
  qInfo() << "Proj(" << proj << ")"
          << " Audio Total           =  " << resourceTotals [ 3 ]
          << Qt::endl;

}

/********************************************************
*          checkParameter function
********************************************************/
bool
checkParameter( const QString & fileName, const QString & proj )
{
  QFileInfo fi_fileName( fileName );

  if (
    ( ! ( fi_fileName.exists() && fi_fileName.isFile() ) )
    )
     {
     qInfo() << "Proj(" << proj << ")"
              << Qt::endl
              << "The file = "
              << fileName << " does not exist or "
              << Qt::endl
              << "the passed strings does not represent a file."
              << Qt::endl
              << "Program will terminate" << Qt::endl;

     return ( fi_fileName.exists() && fi_fileName.isFile() );
     }

  if (
    ( !  fi_fileName.isAbsolute() )
    )
     {
     qInfo() << "Proj(" << proj << ")"
              << Qt::endl
              << "The file = "
              << fileName << " must be an absolute file path."
              << Qt::endl
              << "Program will terminate" << Qt::endl;

     return ( fi_fileName.isAbsolute() );
     }

  return true;
}
