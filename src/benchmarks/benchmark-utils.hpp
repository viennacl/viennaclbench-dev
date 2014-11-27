#ifndef _BENCHMARK_UTILS_HPP_
#define _BENCHMARK_UTILS_HPP_

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <iostream>

/*! \file benchmark-utils.hpp
 * \brief GFLOPs calculation and Timer class are implemented here.
*/

/*!
 * \brief Calculates GFLOPs.
 * \param num_ops Number of operations
 * \param exec_time Execution time
 * \return GFLOPs value
 */
inline double printOps(double num_ops, double exec_time)
{
//  std::cout << "GFLOPs: " << num_ops / (1000000 * exec_time * 1000) << std::endl;
  return num_ops / (1000000 * exec_time * 1000);
}


#ifdef _WIN32

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max

/*! \class Timer
 * \brief Provides convenient start/stop timer functionality.
 */
class Timer
{
public:

  Timer()
  {
    QueryPerformanceFrequency(&freq);
  }

  /*!
   * \brief Starts the timer
   */
  void start()
  {
    QueryPerformanceCounter((LARGE_INTEGER*) &start_time);
  }

  /*!
   * \brief Returns time elapsed
   * \return Returns time elapsed
   */
  double get() const
  {
    LARGE_INTEGER  end_time;
    QueryPerformanceCounter((LARGE_INTEGER*) &end_time);
    return (static_cast<double>(end_time.QuadPart) - static_cast<double>(start_time.QuadPart)) / static_cast<double>(freq.QuadPart);
  }


private:
  LARGE_INTEGER freq;
    LARGE_INTEGER start_time;
};

#else

#include <sys/time.h>

/*! \class Timer
 * \brief Provides convenient start/stop timer functionality.
 */
class Timer
{
public:

  Timer() : ts(0)
  {}

  /*!
   * \brief Starts the timer
   */
  void start()
  {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    ts = static_cast<double>(tval.tv_sec * 1000000 + tval.tv_usec);
  }

  /*!
   * \brief Returns time elapsed
   * \return Returns time elapsed
   */
  double get() const
  {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    double end_time = static_cast<double>(tval.tv_sec * 1000000 + tval.tv_usec);

    return static_cast<double>(end_time-ts) / 1000000.0;
  }

private:
  double ts;
};


#endif

#endif
