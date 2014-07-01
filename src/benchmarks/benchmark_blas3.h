#ifndef BENCHMARK_BLAS3_H
#define BENCHMARK_BLAS3_H

#include "abstractbenchmark.h"

//disable debug mechanisms to have a fair benchmark environment
#ifndef NDEBUG
#define NDEBUG
#endif

//#define VIENNACL_DEBUG_ALL
//#define VIENNACL_DEBUG_BUILD

//
// include necessary system headers
//
#include <iostream>

//
// ViennaCL includes
//
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/matrix_proxy.hpp"
#include "viennacl/linalg/lu.hpp"

// Some helper functions for this tutorial:
#include "src/benchmarks/Random.hpp"

#include "src/benchmarks/benchmark-utils.hpp"

#define BLAS3_MATRIX_SIZE   1920

class Benchmark_Blas3 : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Blas3(QObject *parent = 0);

  template<typename ScalarType>
  void run_benchmark();
signals:
  /* Inherited signals:
   * void resultSignal(QString benchmarkName, double bandwidthValue);
   * void benchmarkComplete();
   * void unitMeasureSignal(QString unitMeasureName);
   * */
public slots:
  void execute();
};

#endif // BENCHMARK_BLAS3_H
