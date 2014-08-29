#ifndef BENCHMARK_COPY_H
#define BENCHMARK_COPY_H

#include "abstractbenchmark.h"

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/linalg/inner_prod.hpp"
#include "viennacl/linalg/norm_2.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>
#include "benchmark-utils.hpp"

using std::cout;
using std::cin;
using std::endl;

#include <QThread>
#include <QVector>
#include <QVariant>
#include <QStringList>
#include <QString>

/*!
 * \class Benchmark_Copy
 * \brief Controls execution of the Copy benchmark
 * Tests are run one by one, with each iterating through all specified vector sizes before the next one starts.
 */
class Benchmark_Copy : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Copy(QObject *parent = 0);
  Benchmark_Copy(bool precision, BenchmarkSettings settings);

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

#endif // BENCHMARK_COPY_H
