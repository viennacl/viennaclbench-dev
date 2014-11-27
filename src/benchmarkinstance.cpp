/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include "benchmarkinstance.h"

/*!
 * \brief Default constructor. Loads default settings (standard mode, double precision).
 */
BenchmarkInstance::BenchmarkInstance()
{
  blas3Result = 0;
  copyResult = 0;
  sparseResult = 0;
  vectorResult = 0;
  mode = "Standard";
  full = true;
  totalScore = 0;
  precision = "Double";
}

/*!
 * \brief Sets this instance's settings.
 * \param s The settings to be set
 */
void BenchmarkInstance::setSettings(BenchmarkSettings s)
{
  this->settings.blas3MinSize = s.blas3MinSize;
  this->settings.blas3MaxSize = s.blas3MaxSize;
  this->settings.blas3IncFactor = s.blas3IncFactor;

  this->settings.copyIncFactor = s.copyIncFactor;
  this->settings.copyMaxVectorSize = s.copyMaxVectorSize;
  this->settings.copyMinVectorSize = s.copyMinVectorSize;

  this->settings.vectorMinVectorSize = s.vectorMinVectorSize;
  this->settings.vectorMaxVectorSize = s.vectorMaxVectorSize;
  this->settings.vectorIncFactor = s.vectorIncFactor;

  this->settings.sparseMatSize = s.sparseMatSize;
  this->settings.sparseCustomMatrix = s.sparseCustomMatrix;

}
