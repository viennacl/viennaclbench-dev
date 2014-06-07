#ifndef BENCHMARK_SOLVER_H
#define BENCHMARK_SOLVER_H

#include <QObject>


#ifndef NDEBUG
#define NDEBUG
#endif

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>

#define VIENNACL_WITH_UBLAS 1

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/coordinate_matrix.hpp"
#include "viennacl/compressed_matrix.hpp"
#include "viennacl/ell_matrix.hpp"
#include "viennacl/hyb_matrix.hpp"
#include "viennacl/context.hpp"

#include "viennacl/linalg/cg.hpp"
#include "viennacl/linalg/bicgstab.hpp"
#include "viennacl/linalg/gmres.hpp"

#include "viennacl/linalg/ilu.hpp"
#include "viennacl/linalg/ichol.hpp"
#include "viennacl/linalg/jacobi_precond.hpp"
#include "viennacl/linalg/row_scaling.hpp"

#ifdef VIENNACL_WITH_OPENCL
#include "viennacl/linalg/mixed_precision_cg.hpp"
#endif

#include "viennacl/io/matrix_market.hpp"

#include <iostream>
#include <vector>
#include "benchmarks/benchmark-utils.hpp"
#include "benchmarks/io.hpp"

using namespace boost::numeric;

#define BENCHMARK_RUNS          1


class Benchmark_Solver : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Solver(QObject *parent = 0);

  template<typename ScalarType>
  ScalarType diff_inf(ublas::vector<ScalarType> &v1, viennacl::vector<ScalarType> &v2);

  template<typename ScalarType>
  ScalarType diff_2(ublas::vector<ScalarType> &v1, viennacl::vector<ScalarType> &v2);

//  template <typename MatrixType, typename VectorType, typename SolverTag, typename PrecondTag>
//  void run_solver(const MatrixType &matrix, const VectorType &rhs, const VectorType &ref_result, const SolverTag &solver, const PrecondTag &precond, long ops);

  template <typename MatrixType, typename VectorType, typename SolverTag, typename PrecondTag>
  double run_solver(const MatrixType &matrix, const VectorType &rhs, const VectorType &ref_result, const SolverTag &solver, const PrecondTag &precond, long ops);

  template<typename ScalarType>
  void run_benchmark(viennacl::context ctx);
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();
public slots:
  void execute();

};

#endif // BENCHMARK_SOLVER_H
