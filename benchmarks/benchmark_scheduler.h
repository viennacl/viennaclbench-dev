#ifndef BENCHMARK_SCHEDULER_H
#define BENCHMARK_SCHEDULER_H

#include <QObject>


//#define VIENNACL_DEBUG_ALL
#ifndef NDEBUG
 #define NDEBUG
#endif

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/inner_prod.hpp"
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/scheduler/execute.hpp"

#include <iostream>
#include <vector>
#include "benchmarks/benchmark-utils.hpp"

using std::cout;
using std::cin;
using std::endl;


#define BENCHMARK_VECTOR_SIZE   2
#define BENCHMARK_RUNS          1000
class Benchmark_Scheduler : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Scheduler(QObject *parent = 0);

  template<typename ScalarType>
  void run_benchmark();
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();
public slots:
  void execute();

};

#endif // BENCHMARK_SCHEDULER_H
