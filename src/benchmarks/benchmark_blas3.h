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
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/matrix_proxy.hpp"
#include "viennacl/linalg/lu.hpp"

// Some helper functions for this tutorial:
#include "Random.hpp"

#include <QVector>

#include "benchmark-utils.hpp"

#define BLAS3_MATRIX_SIZE   1920

class Benchmark_Blas3 : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Blas3(QObject *parent = 0);
  Benchmark_Blas3(bool precision, cl_platform_id platform, cl_device_id device);
  Benchmark_Blas3(bool precision);//, cl_platform_id platform, cl_device_id device);

  template<typename ScalarType>
  void run_benchmark();
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

#endif // BENCHMARK_BLAS3_H
