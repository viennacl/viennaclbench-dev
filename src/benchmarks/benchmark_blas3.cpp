/* =========================================================================
   Copyright (c) 2010-2014, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.
   Portions of this software are copyright by UChicago Argonne, LLC.

                            -----------------
                  ViennaCL - The Vienna Computing Library
                            -----------------

   Project Head:    Karl Rupp                   rupp@iue.tuwien.ac.at

   (A list of authors and contributors can be found in the PDF manual)

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
  blas3MatrixSizeA = settings.blas3MatSizeA;
  blas3MatrixSizeB = settings.blas3MatSizeB;
  blas3MatrixSizeC = settings.blas3MatSizeC;
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
  blas3MatrixSizeA = settings.blas3MatSizeA;
  blas3MatrixSizeB = settings.blas3MatSizeB;
  blas3MatrixSizeC = settings.blas3MatSizeC;
}


template<typename ScalarType>
/*!
 * \brief Main benchmarking function
 * Should only be called by the \ref Benchmark_Blas3::execute() function,
 * since there are certain requirements that need to be fulfilled before starting the benchmarking procedure.
 */
void Benchmark_Blas3::run_benchmark()
{
  Timer timer;
  int testId = 0;

  for (std::size_t N=128; N<=blas3MatrixSizeA; N *= 2)
  {
    viennacl::matrix<ScalarType> vcl_A(N, N);
    viennacl::matrix<ScalarType> vcl_B(N, N);
    viennacl::matrix<ScalarType> vcl_C(N, N);

    //
    // One alternative: Put the matrices into a contiguous block of memory (allows to use viennacl::fast_copy(), avoiding temporary memory)
    //
    std::vector<ScalarType> stl_A(vcl_A.internal_size());
    std::vector<ScalarType> stl_B(vcl_B.internal_size());

    //
    // Fill the matrix
    //
    for (unsigned int i = 0; i < N; ++i)
      for (unsigned int j = 0; j < N; ++j)
        stl_A[i*N + j] = random<ScalarType>();

    for (unsigned int i = 0; i < N; ++i)
      for (unsigned int j = 0; j < N; ++j)
        stl_B[i + j*N] = random<ScalarType>();

    //  std::cout << " ------ Benchmark 1: Matrix-Matrix product ------ " << std::endl;

    viennacl::fast_copy(&(stl_A[0]), &(stl_A[0]) + stl_A.size(), vcl_A);
    viennacl::fast_copy(&(stl_B[0]), &(stl_B[0]) + stl_B.size(), vcl_B);
    vcl_C = viennacl::linalg::prod(vcl_A, vcl_B); // warmup
    viennacl::backend::finish();
    timer.start();
    vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
    viennacl::backend::finish();
    double exec_time = timer.get();

    double gflops_per_second = 2.0 * (vcl_A.size1() / 1000.0) * (vcl_A.size2() / 1000.0) * (vcl_B.size2() / 1000.0) / exec_time ;
    emit resultSignal("Matrix-Matrix product", N, gflops_per_second, LINE_GRAPH, testId);
    testResultHolder.append(gflops_per_second);
    emit testProgress();

    if (exec_time > 10.0) // we don't want the benchmark to get stuck here. The relevant information can already be obtained with less than 10 seconds of execution time.
      break;
  }

}

/*!
 * \brief Begins the benchmark execution.
 */
void Benchmark_Blas3::execute()
{
  emit benchmarkStarted(BLAS3);
  emit unitMeasureSignal("GFLOPs", Qt::YAxis);
  emit unitMeasureSignal("Matrix Size", Qt::XAxis);

  //Single Precision
  if(getPrecision() == SINGLE_PRECISION)
    run_benchmark<float>();

  //Double Precision
  else if( getPrecision() == DOUBLE_PRECISION)
  {
#ifdef VIENNACL_WITH_OPENCL
    if( viennacl::ocl::current_device().double_support() )
#endif
      //what if current device does not support double precision?
    {
      run_benchmark<double>();
    }
  }

  qSort(testResultHolder);//sort test results in ascending order
  emit finalResultSignal("Blas3", testResultHolder[testResultHolder.size()-1]);
  emit benchmarkComplete();
}
