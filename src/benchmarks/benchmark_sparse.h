#ifndef BENCHMARK_SPARSE_H
#define BENCHMARK_SPARSE_H

#include "abstractbenchmark.h"

#ifndef NDEBUG
#define NDEBUG
#endif

#define VIENNACL_WITH_UBLAS 0
//#ifdef VIENNACL_WITH_UBLAS
//#undef VIENNACL_WITH_UBLAS
//#endif

#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/coordinate_matrix.hpp"
#include "viennacl/compressed_matrix.hpp"
#include "viennacl/ell_matrix.hpp"
#include "viennacl/hyb_matrix.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/io/matrix_market.hpp"
#include "viennacl/linalg/ilu.hpp"

#include <QString>
#include <QDir>

#include <iostream>
#include <vector>
#include "benchmark-utils.hpp"
#include "io.hpp"

class Benchmark_Sparse : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Sparse(QObject *parent = 0);

  template<typename ScalarType>
  void run_benchmark();
private:
  double finalResultValue;
  int finalResultCounter;
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

#endif // BENCHMARK_SPARSE_H
