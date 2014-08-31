#ifndef BENCHMARKINSTANCE_H
#define BENCHMARKINSTANCE_H

#include "benchmarksettings.h"

/*! \class BenchmarkInstance
 * \brief Holds all relevant info about a finished benchmark session.
 * In other words, contains results and settings of a finished benchmark session.
 */
class BenchmarkInstance
{
public:
  explicit BenchmarkInstance();

  BenchmarkSettings settings; ///< Settings object of this instance
  double blas3Result; ///< Result of Blas3 benchmark
  double copyResult; ///< Result of Copy benchmark
  double sparseResult; ///< Result of Sparse benchmark
  double vectorResult; ///< Result of Vector benchmark
  QString mode; ///< Benchmark mode
  bool full; ///< Indicates if this was a complete benchmark session (all four benchmarks were executed)
  double totalScore; ///< Final score mark
  QString precision; ///< Benchmark precision
  void setSettings(BenchmarkSettings s);
};

#endif // BENCHMARKINSTANCE_H
