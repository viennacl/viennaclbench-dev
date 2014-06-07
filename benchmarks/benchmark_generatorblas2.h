//#ifndef BENCHMARK_GENERATORBLAS2_H
//#define BENCHMARK_GENERATORBLAS2_H

//#include <QObject>

////disable debug mechanisms to have a fair benchmark environment
//#ifndef NDEBUG
//#define NDEBUG
//#endif

////#define VIENNACL_DEBUG_ALL
////#define VIENNACL_DEBUG_BUILD

////
//// include necessary system headers
////
//#include <iostream>
//#include <iomanip>

////
//// ViennaCL includes
////
//#include "viennacl/vector.hpp"
//#include "viennacl/matrix.hpp"
//#include "viennacl/linalg/prod.hpp"
//#include "viennacl/linalg/norm_2.hpp"

//#include "viennacl/device_specific/code_generator.hpp"

//// Some helper functions for this tutorial:
//#include "benchmarks/Random.hpp"


//#include "benchmark-utils.hpp"

//#define N_RUNS 100
//#define SIZE_INC 256
//#define MAX_SIZE 7936
//class Benchmark_GeneratorBlas2 : public QObject
//{
//  Q_OBJECT
//public:
//  explicit Benchmark_GeneratorBlas2(QObject *parent = 0);
//  template<typename ScalarType, class FB>
//  double run_benchmark(size_t size, bool is_trans);
//signals:
//  void resultSignal(QString benchmarkName, double bandwidthValue);
//  void benchmarkComplete();
//public slots:
//  void execute();

//};

//#endif // BENCHMARK_GENERATORBLAS2_H
