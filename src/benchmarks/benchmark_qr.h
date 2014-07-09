#ifndef BENCHMARK_QR_H
#define BENCHMARK_QR_H

#include "abstractbenchmark.h"

#define VIENNACL_WITH_UBLAS
#ifndef NDEBUG
#define NDEBUG
#endif

#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdio.h>
//#include <sys/time.h>
#include <time.h>
#include "benchmark-utils.hpp"
#include "viennacl/scalar.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/compressed_matrix.hpp"
#include "viennacl/linalg/cg.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/linalg/inner_prod.hpp"
#include "viennacl/linalg/ilu.hpp"
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/io/matrix_market.hpp"
#include "viennacl/linalg/qr.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/io.hpp"


//typedef viennacl::compressed_matrix<float> SparseMatrix;
using namespace boost::numeric::ublas;
//using namespace viennacl::linalg;

class Benchmark_Qr : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Qr(QObject *parent = 0);

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
  //  void updateBenchmarkData(QString benchmarkName, double bandwidthValue);
};

#endif // BENCHMARK_QR_H
