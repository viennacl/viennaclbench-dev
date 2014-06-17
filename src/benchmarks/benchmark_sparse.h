#ifndef BENCHMARK_SPARSE_H
#define BENCHMARK_SPARSE_H

#include <QObject>

#define VIENNACL_BUILD_INFO
#ifndef NDEBUG
 #define NDEBUG
#endif

#define VIENNACL_WITH_UBLAS 0

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
#include "src/benchmarks/benchmark-utils.hpp"
#include "src/benchmarks/io.hpp"

class Benchmark_Sparse : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Sparse(QObject *parent = 0);

  template<typename ScalarType>
  void run_benchmark();
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();
public slots:
  void execute();


};

#endif // BENCHMARK_SPARSE_H
