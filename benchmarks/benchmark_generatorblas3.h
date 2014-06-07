//#ifndef BENCHMARK_GENERATORBLAS3_H
//#define BENCHMARK_GENERATORBLAS3_H

//#include <QObject>

////
//// include necessary system headers
////
//#include <iostream>

////
//// ViennaCL includes
////
////#define VIENNACL_DEBUG_BUILD

//#include "viennacl/scalar.hpp"
//#include "viennacl/vector.hpp"
//#include "viennacl/matrix.hpp"
//#include "viennacl/linalg/prod.hpp"
//#include "viennacl/matrix_proxy.hpp"
//#include "viennacl/linalg/lu.hpp"

//#include "viennacl/device_specific/code_generator.hpp"
//#include "viennacl/scheduler/forwards.h"

//// Some helper functions for this tutorial:
//#include "../tutorial/Random.hpp"


//#include "benchmark-utils.hpp"

//#define N_RUNS 2
//#define SIZE_INC 128
//#define MAX_SIZE 1536
//class Benchmark_GeneratorBlas3 : public QObject
//{
//  Q_OBJECT
//public:
//  explicit Benchmark_GeneratorBlas3(QObject *parent = 0);
//  template<typename ScalarType>
//  unsigned int run_benchmark(size_t size, bool is_lhs_trans, bool is_rhs_trans);
//signals:
//  void resultSignal(QString benchmarkName, double bandwidthValue);
//  void benchmarkComplete();
//public slots:
//  void execute();

//};

//#endif // BENCHMARK_GENERATORBLAS3_H
