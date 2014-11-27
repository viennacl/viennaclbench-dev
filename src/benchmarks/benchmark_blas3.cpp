/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

/*
*
*   Benchmark: BLAS level 3 functionality for dense matrices (blas3.cpp and blas3.cu are identical, the latter being required for compilation using CUDA nvcc)
*
*/

#include "benchmark_blas3.h"

/*!
 * \brief Default constructor.
 * Sets the precision to double and loadd defaults matrix sizes from \ref BenchmarkSettings
 * \param parent Optional parent object.
 */
Benchmark_Blas3::Benchmark_Blas3(QObject *parent) :
  AbstractBenchmark(parent)
{
  testResultHolder.clear();
  setPrecision(DOUBLE_PRECISION);
  BenchmarkSettings settings;
  blas3MinSize = settings.blas3MinSize;
  blas3MaxSize = settings.blas3MaxSize;
  blas3IncFactor = settings.blas3IncFactor;
}

/*!
 * \brief Constructor with precision and settings
 * \param precision Benchmark precision
 * \param settings Settings from which to load matrix sizes
 */
Benchmark_Blas3::Benchmark_Blas3(bool precision, BenchmarkSettings settings)
{
  Benchmark_Blas3();
  setPrecision(precision);
  blas3MinSize = settings.blas3MinSize;
  blas3MaxSize = settings.blas3MaxSize;
  blas3IncFactor = settings.blas3IncFactor;
}


template<typename ScalarType>
/*!
 * \brief Main benchmarking function
 * Should only be called by the \ref Benchmark_Blas3::execute() function,
 * since there are certain requirements that need to be fulfilled before starting the benchmarking procedure.
 */
void Benchmark_Blas3::run_benchmark()
{
  run_benchmark_impl<ScalarType>(false, false, 0);
  run_benchmark_impl<ScalarType>(false, true,  1);
  run_benchmark_impl<ScalarType>(true,  false, 2);
  run_benchmark_impl<ScalarType>(true,  true,  3);
}

template<typename ScalarType>
/*!
 * \brief Main benchmarking function
 * Should only be called by the \ref Benchmark_Blas3::execute() function,
 * since there are certain requirements that need to be fulfilled before starting the benchmarking procedure.
 */
void Benchmark_Blas3::run_benchmark_impl(bool trans_A, bool trans_B, int testId)
{
  Timer timer;

  for (std::size_t N=blas3MinSize; N<=blas3MaxSize; N *= 2)
  {
    viennacl::matrix<ScalarType> vcl_A = viennacl::scalar_matrix<ScalarType>(N, N, ScalarType(1.0));
    viennacl::matrix<ScalarType> vcl_B = viennacl::scalar_matrix<ScalarType>(N, N, ScalarType(1.1));
    viennacl::matrix<ScalarType> vcl_C(N, N);

    // warmup:
    if (!trans_A && !trans_B)
      vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
    else if (!trans_A && trans_B)
      vcl_C = viennacl::linalg::prod(vcl_A, trans(vcl_B));
    else if (trans_A && !trans_B)
      vcl_C = viennacl::linalg::prod(trans(vcl_A), vcl_B);
    else
      vcl_C = viennacl::linalg::prod(trans(vcl_A), trans(vcl_B));

    ////// Benchmark Start
    viennacl::backend::finish();
    timer.start();

    if (!trans_A && !trans_B)
      vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
    else if (!trans_A && trans_B)
      vcl_C = viennacl::linalg::prod(vcl_A, trans(vcl_B));
    else if (trans_A && !trans_B)
      vcl_C = viennacl::linalg::prod(trans(vcl_A), vcl_B);
    else
      vcl_C = viennacl::linalg::prod(trans(vcl_A), trans(vcl_B));

    viennacl::backend::finish();
    double exec_time = timer.get();
    ////// Benchmark Stop

    std::string line_desc("C = A");
    if (trans_A)
      line_desc += "^T";
    line_desc += " * B";
    if (trans_B)
      line_desc += "^T";
    double gflops_per_second = 2.0 * (vcl_A.size1() / 1000.0) * (vcl_A.size2() / 1000.0) * (vcl_B.size2() / 1000.0) / exec_time ;
    emit resultSignal(line_desc.c_str(), N, gflops_per_second, LINE_GRAPH, testId);
    testResultHolder.append(gflops_per_second);

    if (exec_time > 10.0) // we don't want the benchmark to get stuck here. The relevant information can already be obtained with less than 10 seconds of execution time.
      break;
  }
  emit testProgress();

}



/*!
 * \brief Begins the benchmark execution.
 */
void Benchmark_Blas3::execute()
{
  emit benchmarkStarted(BLAS3);
  emit unitMeasureSignal("GFLOPs", Qt::YAxis);
  emit unitMeasureSignal("Matrix Size", Qt::XAxis);

  if(getPrecision() == SINGLE_PRECISION)
    run_benchmark<float>();
  else if( getPrecision() == DOUBLE_PRECISION
#ifdef VIENNACL_WITH_OPENCL
           && viennacl::ocl::current_device().double_support()
#endif
           )
    run_benchmark<double>();

  qSort(testResultHolder);//sort test results in ascending order
  emit finalResultSignal("Blas3", testResultHolder[testResultHolder.size()-1]);
  emit benchmarkComplete();
}
