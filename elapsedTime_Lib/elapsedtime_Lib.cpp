#include "elapsedtime_Lib.h"

ElapsedTime_Lib::ElapsedTime_Lib()
  : m_TimeStart { std::chrono::high_resolution_clock::now() }
  {
  }

ElapsedTime_Lib::~ElapsedTime_Lib()
  {
  /***********************************************************
  *          Standard C++
  ***********************************************************/

  auto                              end             = std::chrono::high_resolution_clock::now();

  std::chrono::duration <double>    elapsedSeconds  = end - m_TimeStart;

  qInfo()   << Qt::endl
            << "Elapsed time : "
            << elapsedSeconds.count()
            << " seconds. (std C++)"
            << Qt::endl;

  }
