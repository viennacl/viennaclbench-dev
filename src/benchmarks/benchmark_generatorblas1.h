//#ifndef BENCHMARK_GENERATORBLAS1_H
//#define BENCHMARK_GENERATORBLAS1_H

//#include <QObject>
////disable debug mechanisms to have a fair benchmark environment
//#ifndef NDEBUG
//#define NDEBUG
//#endif

////#define VIENNACL_DEBUG_BUILD

////
//// include necessary system headers
////
//#include <iostream>
//#include <iomanip>

////
//// ViennaCL includes
////
//#include "viennacl/scalar.hpp"
//#include "viennacl/vector.hpp"
//#include "viennacl/linalg/inner_prod.hpp"

//#include "viennacl/device_specific/code_generator.hpp"

//// Some helper functions for this tutorial:
//#include "benchmarks/Random.hpp"

//#include "viennacl/scheduler/forwards.h"

//#include "benchmarks/benchmark-utils.hpp"

//#define N_RUNS 100
//#define MAX_SIZE 1e8

//enum operation_type{
//  dot,
//  assign
//};

//class Benchmark_GeneratorBlas1 : public QObject
//{
//  Q_OBJECT
//public:
//  explicit Benchmark_GeneratorBlas1(QObject *parent = 0);
//  template<typename ScalarType>
//  ScalarType run_benchmark(size_t size, operation_type type);
//signals:
//  void resultSignal(QString benchmarkName, double bandwidthValue);
//  void benchmarkComplete();
//public slots:
//  void execute();
//};

//#endif // BENCHMARK_GENERATORBLAS1_H
