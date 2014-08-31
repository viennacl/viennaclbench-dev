#ifndef BENCHMARK_VECTOR_H
#define BENCHMARK_VECTOR_H

#include "abstractbenchmark.h"

#include <QVector>

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/linalg/inner_prod.hpp"
#include "viennacl/linalg/norm_2.hpp"

#include <iostream>
#include <vector>
#include "benchmark-utils.hpp"

//#define VIENNACL_DEBUG_ALL
#ifndef NDEBUG
#define NDEBUG
#endif

using std::cout;
using std::cin;
using std::endl;
/*! \file benchmark_vector.h */

/*!
  \def BENCHMARK_RUNS
  Number of times each test is to be run.
 */
#define BENCHMARK_RUNS          10

/*!
 * \class Benchmark_Vector
 * \brief Controls execution of the Vector benchmark
 * The benchmark iterates through selected vector sizes and runs the entire test suite, one size at a time.
 * Each test is run \ref BENCHMARK_RUNS times.
 */
class Benchmark_Vector : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Vector(QObject *parent = 0);
  Benchmark_Vector(bool precision, BenchmarkSettings settings);

  template<typename ScalarType>
  void run_benchmark();
  template<typename ScalarType>
  void resizeVectors(int size, std::vector<ScalarType> &std_vec1, std::vector<ScalarType> &std_vec2,
                     viennacl::vector<ScalarType> &vcl_vec1, viennacl::vector<ScalarType> &vcl_vec2);
private:
  int MAX_BENCHMARK_VECTOR_SIZE; ///< Maximum benchmark vector size. The benchmark ends when the vector size exceeds or matches this limit.
  int MIN_BENCHMARK_VECTOR_SIZE; ///< Starting benchmark vector size
  int INCREMENT_FACTOR; ///< Amount by which the vector size is multiplied on each run
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

#endif // BENCHMARK_VECTOR_H
