#ifndef VIENNACLBENCH_RANDOM_HPP_
#define VIENNACLBENCH_RANDOM_HPP_

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <time.h>
#include <stdlib.h>
/*! \file Random.hpp
 * \brief Random number generation is implemented here.
 * */

inline void init()
{
  static bool init = false;
  if (!init)
  {
    srand( (unsigned int)time(NULL) );
    init = true;
  }
}

template<class TYPE>
TYPE random();

template<>
inline double random<double>()
{
  init();
  return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

template<>
inline float random<float>()
{
  init();
  return static_cast<float>(random<double>());
}

#endif
