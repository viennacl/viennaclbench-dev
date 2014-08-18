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
*   Benchmark:  Sparse matrix operations, i.e. matrix-vector products (sparse.cpp and sparse.cu are identical, the latter being required for compilation using CUDA nvcc)
*
*/

#include "benchmark_sparse.h"
#include <QDebug>

#include "viennacl/tools/matrix_generation.hpp"

Benchmark_Sparse::Benchmark_Sparse(QObject *parent) :
  AbstractBenchmark(parent)
{
  testResultHolder.clear();
  setPrecision(DOUBLE_PRECISION);
  BenchmarkSettings settings;
  xPoints = (viennacl::vcl_size_t) settings.sparseMatSizeA;
  yPoints = (viennacl::vcl_size_t) settings.sparseMatSizeB;
  customSparseMatrixPath = settings.sparseCustomMatrix;
}

Benchmark_Sparse::Benchmark_Sparse(bool precision, BenchmarkSettings settings)
{
  Benchmark_Sparse();
  setPrecision(precision);
  xPoints = (viennacl::vcl_size_t) settings.sparseMatSizeA;
  yPoints = (viennacl::vcl_size_t) settings.sparseMatSizeB;
  customSparseMatrixPath = settings.sparseCustomMatrix;
}

template<typename ScalarType>
void Benchmark_Sparse::run_benchmark()
{
//  std::cout << "Benchmarking..." << std::endl;
//  std::cout << "Platform id: "<< viennacl::ocl::current_context().platform_index() //platform id != context id
//            <<" Context value: " << viennacl::ocl::current_context().handle().get() << std::endl;

//  std::cout << "Running on device name: "<< viennacl::ocl::current_device().name() << std::endl;
  Timer timer;
  double exec_time;
  int testId = 0;

//  ScalarType std_factor1 = ScalarType(3.1415);
//  ScalarType std_factor2 = ScalarType(42.0);

  viennacl::compressed_matrix<ScalarType, 1> vcl_compressed_matrix_1;
  viennacl::compressed_matrix<ScalarType, 4> vcl_compressed_matrix_4;
  viennacl::compressed_matrix<ScalarType, 8> vcl_compressed_matrix_8;

  viennacl::coordinate_matrix<ScalarType> vcl_coordinate_matrix_128;

  viennacl::ell_matrix<ScalarType, 1> vcl_ell_matrix_1;
  viennacl::hyb_matrix<ScalarType, 1> vcl_hyb_matrix_1;

//  std::cout << "Generating Matrix..." << std::endl;
  std::vector< std::map<unsigned int, ScalarType> > stl_A;
  viennacl::tools::sparse_matrix_adapter<ScalarType> adapted_A(stl_A);
  if(customSparseMatrixPath.isEmpty()){
    //no custom matrix specified, generate the default one
    viennacl::tools::generate_fdm_laplace(adapted_A, xPoints, yPoints );
  }
  else{
    //load matrix from file
    if(!viennacl::io::read_matrix_market_file(adapted_A /* Is this the right variable to read into? */, customSparseMatrixPath.toStdString()) ){
      std::cout << "Custom sparse matrix read successfully" <<std::endl;
    }
    else{
      std::cout << "Failed to read custom sparse matrix" <<std::endl;
      emit errorMessage("Failed to read selected sparse matrix");
      return;
    }
  }

  // create vectors and fill them with some (arbitrary) scalar values:
  viennacl::vector<ScalarType> vcl_vec1 = viennacl::scalar_vector<ScalarType>(stl_A.size(), 1.0);
  viennacl::vector<ScalarType> vcl_vec2 = viennacl::scalar_vector<ScalarType>(stl_A.size(), 2.0);

  //cpu to gpu:
  viennacl::copy(adapted_A, vcl_compressed_matrix_1);
  viennacl::copy(adapted_A, vcl_compressed_matrix_4);
  viennacl::copy(adapted_A, vcl_compressed_matrix_8);
  viennacl::copy(adapted_A, vcl_coordinate_matrix_128);
  viennacl::copy(adapted_A, vcl_ell_matrix_1);
  viennacl::copy(adapted_A, vcl_hyb_matrix_1);


  ///////////// Matrix operations /////////////////

  double tempResultValue;

//  std::cout << "------- Matrix-Vector product with compressed_matrix ----------" << std::endl;

  vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_1, vcl_vec2); //startup calculation
  vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_4, vcl_vec2); //startup calculation
  vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_8, vcl_vec2); //startup calculation

  viennacl::backend::finish();
  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_1, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Matrix-Vector product(compressed_matrix) align1", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();


  viennacl::backend::finish();
  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_4, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Matrix-Vector product(compressed_matrix) align4", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();

  viennacl::backend::finish();
  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_8, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Matrix-Vector product(compressed_matrix) align8", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();


//  std::cout << "------- Matrix-Vector product with coordinate_matrix ----------" << std::endl;

  vcl_vec1 = viennacl::linalg::prod(vcl_coordinate_matrix_128, vcl_vec2); //startup calculation
  viennacl::backend::finish();

  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_coordinate_matrix_128, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Matrix-Vector product(coordinate_matrix)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();

//  std::cout << "------- Matrix-Vector product with ell_matrix ----------" << std::endl;

  vcl_vec1 = viennacl::linalg::prod(vcl_ell_matrix_1, vcl_vec2); //startup calculation
  viennacl::backend::finish();

  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_ell_matrix_1, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Matrix-Vector product(ell_matrix)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();

//  std::cout << "------- Matrix-Vector product with hyb_matrix ----------" << std::endl;

  vcl_vec1 = viennacl::linalg::prod(vcl_hyb_matrix_1, vcl_vec2); //startup calculation
  viennacl::backend::finish();

  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_hyb_matrix_1, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Matrix-Vector product(hyb_matrix)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();
}

void Benchmark_Sparse::execute()
{
  emit benchmarkStarted(SPARSE);
  emit unitMeasureSignal("GFLOPs - Sparse", Qt::XAxis);

  if(getPrecision() == SINGLE_PRECISION)
  {//Single
    run_benchmark<float>();
  }

  else if( getPrecision() == DOUBLE_PRECISION)
  {//Double
#ifdef VIENNACL_WITH_OPENCL
    if( viennacl::ocl::current_device().double_support() )
#endif
      //what if current device does not support double precision?
    {
      run_benchmark<double>();
    }
  }

  qSort(testResultHolder);//sort test results in ascending order
  emit finalResultSignal("Sparse", testResultHolder[testResultHolder.size()/2]);
  emit benchmarkComplete();
}
