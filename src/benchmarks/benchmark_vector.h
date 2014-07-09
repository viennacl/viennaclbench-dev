#ifndef BENCHMARK_VECTOR_H
#define BENCHMARK_VECTOR_H

#include "abstractbenchmark.h"

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

  template<typename ScalarType>
  void run_benchmark();
private:
  double finalResultValue;
  int finalResultCounter;
signals:
  /* Inherited signals:
   * void finalResultSignal(QString benchmarkName, double finalValue);
   * void resultSignal(QString benchmarkName, double bandwidthValue);
   * void benchmarkComplete();
   * void unitMeasureSignal(QString unitMeasureName);
   * */
public slots:
  void execute();

};

#endif // BENCHMARK_VECTOR_H
