#ifndef ELAPSEDTIME_LIB_H
#define ELAPSEDTIME_LIB_H

#include "elapsedTime_Lib_global.h"
#include <chrono>
#include <QElapsedTimer>
#include <QDebug>

using namespace std::chrono;

class ELAPSEDTIME_LIB_EXPORT ElapsedTime_Lib
  {
  public:

    ElapsedTime_Lib();
    ~ElapsedTime_Lib();

  private:

    std::chrono::high_resolution_clock::time_point m_TimeStart;
  };

#endif // ELAPSEDTIME_LIB_H
