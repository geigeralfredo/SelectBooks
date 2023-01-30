#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QMap>
#include <QDebug>
#include <QDateTime>

#include "errorCodeAndMsg_Lib.h"

#ifdef NDEBUG
#define QT_NO_DEBUG_OUTPUT
#define QT_NO_INFO_OUTPUT
#define QT_NO_WARNING_OUTPUT
#endif

/*--------------------------------------------------------------------
 *                    constants
 *--------------------------------------------------------------------*/

class Constants
  {
  public:

    /*--------------------------------------------------------------------
     *                       Help text
     *--------------------------------------------------------------------*/
    const QString &
    helpText();

    void
    funcNameAndRetString( const QString & funcName,
                          const QString & string );

    /*----------------------------------------------------------------------
     *              Error Code and Message
     *----------------------------------------------------------------------*/

    ErrorCodeAndMsg_Lib
    errorCodeAndErrorMsg( const QString & keyString );

    /*----------------------------------------------------------------------
     *              timeStamp
     *----------------------------------------------------------------------*/
    QString &
    timeStamp();

    /*----------------------------------------------------------------------
     *              retrieveLiteral
     *----------------------------------------------------------------------*/
    QString
    retrieveLiteral( const QString & keyString );

    /*----------------------------------------------------------------------
     *              Command Construction Functions
     *----------------------------------------------------------------------
     *              Find cmd
     *----------------------------------------------------------------------*/
    QString &
    findCmd( const QString & dirOfBookCollection );

    /*----------------------------------------------------------------------
     *              bashCmdAppendTo
     *----------------------------------------------------------------------*/
    QString &
    bashCmdAppendTo( const QString & fileTotals );

    /*----------------------------------------------------------------------
     *              sortCmdComplement2()
     *----------------------------------------------------------------------*/
    QString &
    sortCmdComplement2();

    /*----------------------------------------------------------------------
     *              sortCmdComplement3()
     *----------------------------------------------------------------------*/
    QString &
    sortCmdComplement3();

    /*----------------------------------------------------------------------
     *              sortCmdWhole2()
     *----------------------------------------------------------------------*/
    QString &
    sortCmdWhole2();

    /*----------------------------------------------------------------------
     *              sortCmdWhole3()
     *----------------------------------------------------------------------*/
    QString &
    sortCmdWhole3();

    /*----------------------------------------------------------------------
     *              write to temp file
     *----------------------------------------------------------------------*/
    QString &
    writeToTemp();

  private:

    /*--------------------------------------------------------------------
     *                       Help text
     *--------------------------------------------------------------------*/

    const QString s_helpText =
      {
      "\nSelectBooks - Mandatory Parameters:\n"
      "                '-b' (or '--book-colletion')\n"
      "                '-s' (or '--subjects-file')\n"
      "                '-l' (or '--lists-of-books')\n"
      " \n"
      "                Optional Parameter:\n"
      "                '-a' (or '--create-let-allbooks')\n"
      " \n"
      "                This application reads just the file names of all books\n"
      "                that are present in the directory informed by the \n"
      "                '-b' (or '--book-colletion') parameter.\n"
      " \n"
      "                Then, selects the books that have in their names or in their\n"
      "                directory names the words that are present in the subjects file.\n"
      " \n"
      "                The parameter '-s' (or '--subjects-file') inform the subjects file name.\n"
      "                Subjects are words or group of words that denote a subject\n"
      "                that will be searched for in the collection of books.\n"
      " \n"
      "                The lists of books directory informed by '-l' (or '--lists-of-books') parameter, \n"
      "                are the books found in the book collection that have at least one of the words \n"
      "                present in the subjects file.\n"
      " \n"
      "                The file names of the lists are a concatenation of the words of subjects \n"
      "                plus the suffix '.list' and the extension '.txt'.\n"
      " \n"
      "                The parameter '-a' (or '--create-let-allbooks') is used to speed up the book selection process \n"
      "                the value 'C' is: CREATE the AllBooks file (SLOWER)\n"
      "                the value 'L' is: DON'T CREATE a new version of the file, use the old one(FASTER).\n"
      };

    const QMap < QString, QString > cmdMap =
      {
        { "allBooksTXTFile",         "AllBooks.txt"                                                                                    },
        { "argument2Bash",           "-c"                                                                                              },
        { "asteriskListTxt",         "*.list.txt"                                                                                      },
        { "bashCmdAppendTo",         " >> "                                                                                            },
        { "bashCmdPipe",             " | "                                                                                             },
        { "bashCmdWriteTo",          " > "                                                                                             },
        { "bashProgam",              "/usr/bin/bash"                                                                                   },
        { "catCmd",                  "/usr/bin/cat "                                                                                   },
        { "cdCmd",                   "/usr/bin/cd "                                                                                    },
        { "emptyString",             ""                                                                                                },
        { "equalSignLine",           "============================================================"                                    },
        { "findCmdEnd",              " -iname '*' -type f -print"                                                                      },
        { "findCmdStart",            "/usr/bin/find"                                                                                   },
        { "grepEiEndCmd",            "([^a-z]|$)\'"                                                                                    },
        { "grepEiStartCmd",          "/usr/bin/grep -Ei \'(^|[^a-z])"                                                                  },
        { "grepiCmd",                "/usr/bin/grep -i "                                                                               },
        { "grepivCmd",               "/usr/bin/grep -iv "                                                                              },
        { "invertedMatchSymbol",     "~"                                                                                               },
        { "listTxt",                 ".list.txt"                                                                                       },
        { "numberSign",              "#"                                                                                               },
        { "selectedBooksTotalsFile", "selectedBooksTotals.txt"                                                                         },
        { "singleSpace",             " "                                                                                               },
        { "sortCmd",                 "/usr/bin/sort "                                                                                  },
        { "tmpFileName2",            "/tmp/temporary2.txt"                                                                             },
        { "tmpFileName3",            "/tmp/temporary3.txt"                                                                             },
        { "tmpFileName",             "/tmp/temporary.txt"                                                                              },
        { "wholeWordSymbol",         "^"                                                                                               }
      };

    const QMap < QString, QString> errorCodeAndMsgMap =
      {
        {
          { "dirBookCollection_Error",          "10 Book Collection Directory - does not exist "                             },
          { "subjectsFile_Error",               "20 Subjects File             - does not exist "                             },
          { "allParametersMissing_Error",       "30 All Parameters are missing "                                             },
          { "fileNotRemoved_Error",             "40 File could not be removed to trash "                                     },
          { "fileNameEmpty_Error",              "50 File Name is empty "                                                     },
          { "file2BeRemovedDoesNotExist_Error", "60 File To Be Removed Does Not Exist "                                      }
        }
      };

  };

#endif // CONSTANTS_H
