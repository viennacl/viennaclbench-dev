#ifndef BENCHMARK_SPARSE_H
#define BENCHMARK_SPARSE_H

#include "abstractbenchmark.h"

#ifndef NDEBUG
#define NDEBUG
#endif
/*! \file benchmark_sparse.h */

/*!
  \def BENCHMARK_RUNS
  Number of times each test is to be run.
 */
#define BENCHMARK_RUNS          10

#include "viennacl/compressed_matrix.hpp"
#include "viennacl/coordinate_matrix.hpp"
#include "viennacl/ell_matrix.hpp"
#include "viennacl/hyb_matrix.hpp"
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/io/matrix_market.hpp"
#include "viennacl/linalg/ilu.hpp"
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/tools/matrix_generation.hpp"

#include <QString>
#include <QDir>
#include <QVector>
#include <QMessageBox>

#include <iostream>
#include <vector>
#include "benchmark-utils.hpp"

/*!
 * \class Benchmark_Sparse
 * \brief Controls execution of the Sparse benchmark
 * If no custom matrix has been selected, the default matrix is generated on-the-fly.
 * Otherwise, the benchmark attempts to load the specified matrix and run tests with it.
 * Each test is run \ref BENCHMARK_RUNS times.
 */
class Benchmark_Sparse : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Sparse(QObject *parent = 0);
  Benchmark_Sparse(bool precision, BenchmarkSettings settings);

  template<typename ScalarType>
  void run_benchmark();
private:
  viennacl::vcl_size_t xPoints; ///< X dimension of on-the-fly generated matrix
  viennacl::vcl_size_t yPoints; ///< Y dimension of on-the-fly generated matrix
  QString customSparseMatrixPath; ///< Absolute path to a user-specified sparse matrix
  QVector<double> testResultHolder; ///< Holds results of each taken test. Median test value is marked as the final benchmark result.
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

#endif // BENCHMARK_SPARSE_H
