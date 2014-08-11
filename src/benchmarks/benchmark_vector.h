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

#define BENCHMARK_VECTOR_SIZE   3000000
#define BENCHMARK_RUNS          10

class Benchmark_Vector : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Vector(QObject *parent = 0);
  Benchmark_Vector(bool precision);

  template<typename ScalarType>
  void run_benchmark();
  template<typename ScalarType>
  void resizeVectors(int size, std::vector<ScalarType> &std_vec1, std::vector<ScalarType> &std_vec2,
                     viennacl::vector<ScalarType> &vcl_vec1, viennacl::vector<ScalarType> &vcl_vec2);
private:
  QVector<double> testResultHolder;
signals:
  /* Inherited signals:
   * void benchmarkStarted(int benchmarkIdNumber);
   * void finalResultSignal(QString benchmarkName, double finalValue);
   * void resultSignal(QString benchmarkName, double bandwidthValue);
   * void benchmarkComplete();
   * void unitMeasureSignal(QString unitMeasureName);
   * */
public slots:
  void execute();

};

#endif // BENCHMARK_VECTOR_H
