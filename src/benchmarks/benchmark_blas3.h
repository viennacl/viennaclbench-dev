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
#include "viennacl/matrix.hpp"
#include "viennacl/matrix_proxy.hpp"
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/linalg/lu.hpp"
#include "viennacl/linalg/prod.hpp"

// Some helper functions:
#include "Random.hpp"

#include <QVector>

#include "benchmark-utils.hpp"

/*!
 * \class  Controls execution of the BLAS3 benchmark
 * This benchmark is straightforward. Creates matrices of specified sizes, and runs 3 tests with them.
 * The 4th test (LU Factorization) is disabled since it does not support non-square matrices.
 */
class Benchmark_Blas3 : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Blas3(QObject *parent = 0);
  Benchmark_Blas3(bool precision, BenchmarkSettings settings);

  template<typename ScalarType>
  void run_benchmark();
private:
  QVector<double> testResultHolder; ///< Holds results of each taken test. Median test value is marked as the final benchmark result.
  int blas3MatrixSizeA; ///< Matrix size1
  int blas3MatrixSizeB; ///< Matrix size2
  int blas3MatrixSizeC; ///< Matrix size3
signals:
  /* Inherited signals:
   * void errorMessage(QString message);
   * void benchmarkStarted(int benchmarkIdNumber);
   * void finalResultSignal(QString benchmarkName, double finalValue);
   * void resultSignal(QString benchmarkName, double bandwidthValue);
   * void benchmarkComplete();
   * void unitMeasureSignal(QString unitMeasureName);
   * void testProgress();
   * */
public slots:
  void execute();
};

#endif // BENCHMARK_BLAS3_H
