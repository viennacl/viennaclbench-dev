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

Benchmark_Blas3::Benchmark_Blas3(bool precision, BenchmarkSettings settings)//, cl_platform_id platform, cl_device_id device)
{
  Benchmark_Blas3();
  setPrecision(precision);
  blas3MatrixSizeA = settings.blas3MatSizeA;
  blas3MatrixSizeB = settings.blas3MatSizeB;
  blas3MatrixSizeC = settings.blas3MatSizeC;
}

template<typename ScalarType>
void Benchmark_Blas3::run_benchmark()
{
  Timer timer;
  double exec_time;
  int testId = 0;

  std::cout << "Benchmarking..." << std::endl;
  std::cout << "Platform id: "<< viennacl::ocl::current_context().platform_index() //platform id != context id
            <<" Context value: " << viennacl::ocl::current_context().handle().get() << std::endl;

  std::cout << "Running on device name: "<< viennacl::ocl::current_device().name() << std::endl;
  std::cout << blas3MatrixSizeA << "|||" <<blas3MatrixSizeB<<"|||"<<blas3MatrixSizeC<< std::endl;
  //
  // Set up some ViennaCL objects
  //

  //viennacl::ocl::current_context().build_options("-cl-mad-enable -cl-fast-relaxed-math");   //uncomment for additional optimizations
  //viennacl::ocl::current_context().build_options("-cl-opt-disable");                        //uncomment to get poor performance
  viennacl::matrix<ScalarType> vcl_A(blas3MatrixSizeA, blas3MatrixSizeB);
  viennacl::matrix<ScalarType> vcl_B(blas3MatrixSizeB, blas3MatrixSizeC);
  viennacl::matrix<ScalarType> vcl_C(blas3MatrixSizeA, blas3MatrixSizeC);

  //
  // One alternative: Put the matrices into a contiguous block of memory (allows to use viennacl::fast_copy(), avoiding temporary memory)
  //
  std::vector<ScalarType> stl_A(vcl_A.internal_size());
  std::vector<ScalarType> stl_B(vcl_B.internal_size());

  //
  // Fill the matrix
  //
  for (unsigned int i = 0; i < blas3MatrixSizeA; ++i)
    for (unsigned int j = 0; j < blas3MatrixSizeB; ++j)
      stl_A[i*blas3MatrixSizeA + j] = random<ScalarType>();

  for (unsigned int i = 0; i < blas3MatrixSizeB; ++i)
    for (unsigned int j = 0; j < blas3MatrixSizeC; ++j)
      stl_B[i + j*blas3MatrixSizeC] = random<ScalarType>();


  double tempResultValue;

  /////////////////////////////////////////////////
  //////////// Matrix-matrix products /////////////
  /////////////////////////////////////////////////

  //  std::cout << " ------ Benchmark 1: Matrix-Matrix product ------ " << std::endl;

  viennacl::fast_copy(&(stl_A[0]),
      &(stl_A[0]) + stl_A.size(),
      vcl_A);
  viennacl::fast_copy(&(stl_B[0]),
      &(stl_B[0]) + stl_B.size(),
      vcl_B);
  vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
  viennacl::backend::finish();
  timer.start();
  vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
  viennacl::backend::finish();
  exec_time = timer.get();

  tempResultValue = 2.0 * (vcl_A.size1() / 1000.0) * (vcl_A.size2() / 1000.0) * (vcl_B.size2() / 1000.0) / exec_time ;
  emit resultSignal("Matrix-Matrix product", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();

  //  std::cout << " ------ Benchmark 2: Matrix-Matrix product using ranges ------ " << std::endl;

  viennacl::range r(blas3MatrixSizeB/4, 3 * blas3MatrixSizeB/4);

  viennacl::fast_copy(&(stl_A[0]),
      &(stl_A[0]) + stl_A.size(),
      vcl_A);
  viennacl::fast_copy(&(stl_B[0]),
      &(stl_B[0]) + stl_B.size(),
      vcl_B);
  viennacl::project(vcl_C, r, r) = viennacl::linalg::prod(viennacl::project(vcl_A, r, r), viennacl::project(vcl_B, r, r));
  viennacl::backend::finish();
  timer.start();
  viennacl::project(vcl_C, r, r) = viennacl::linalg::prod(viennacl::project(vcl_A, r, r), viennacl::project(vcl_B, r, r));
  viennacl::backend::finish();
  exec_time = timer.get();

  tempResultValue = 2.0 * (vcl_A.size1() / 2000.0) * (vcl_A.size2() / 2000.0) * (vcl_B.size2() / 2000.0) / exec_time ;
  emit resultSignal("Matrix-Matrix product using ranges", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();

  //  std::cout << " ------ Benchmark 3: Matrix-Matrix product using slices ------ " << std::endl;

  viennacl::slice s(0, 2, blas3MatrixSizeB/2);

  viennacl::fast_copy(&(stl_A[0]),
      &(stl_A[0]) + stl_A.size(),
      vcl_A);
  viennacl::fast_copy(&(stl_B[0]),
      &(stl_B[0]) + stl_B.size(),
      vcl_B);
  viennacl::project(vcl_C, s, s) = viennacl::linalg::prod(viennacl::project(vcl_A, s, s), viennacl::project(vcl_B, s, s));
  viennacl::backend::finish();
  timer.start();
  viennacl::project(vcl_C, s, s) = viennacl::linalg::prod(viennacl::project(vcl_A, s, s), viennacl::project(vcl_B, s, s));
  viennacl::backend::finish();
  exec_time = timer.get();

  tempResultValue = 2.0 * (vcl_A.size1() / 2000.0) * (vcl_A.size2() / 2000.0) * (vcl_B.size2() / 2000.0) / exec_time;
  emit resultSignal("Matrix-Matrix product using slices", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();
/*
 * LU factorization is disabled (does not support non-square matrices)
  //  std::cout << " ------ Benchmark 4: LU factorization ------ " << std::endl;

  viennacl::fast_copy(&(stl_A[0]),
      &(stl_A[0]) + stl_A.size(),
      vcl_A);
  viennacl::linalg::lu_factorize(vcl_A);
  viennacl::backend::finish();
  timer.start();
  viennacl::linalg::lu_factorize(vcl_A);
  viennacl::backend::finish();
  exec_time = timer.get();

  tempResultValue = 2.0 * (vcl_A.size1() / 1000.0) * (vcl_A.size2() / 1000.0) * (vcl_A.size2() / 1000.0) / exec_time;
  emit resultSignal("LU factorization", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();
*/
}

void Benchmark_Blas3::execute()
{
  emit benchmarkStarted(BLAS3);
  emit unitMeasureSignal("GFLOPs", Qt::XAxis);

  //Single Precision
  if(getPrecision() == SINGLE_PRECISION)
  {
    run_benchmark<float>();
  }

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
  emit finalResultSignal("Blas3", testResultHolder[testResultHolder.size()/2]);
  emit benchmarkComplete();
}
