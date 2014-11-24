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

/*!
 * \brief Default constructor.
 * Sets the precision to double and loads default matrix sizes & custom matrix path from \ref BenchmarkSettings
 * \param parent Optional parent object.
 */
Benchmark_Sparse::Benchmark_Sparse(QObject *parent) :
  AbstractBenchmark(parent)
{
  testResultHolder.clear();
  setPrecision(DOUBLE_PRECISION);
  BenchmarkSettings settings;
  xPoints = static_cast<viennacl::vcl_size_t>(std::sqrt(static_cast<double>(settings.sparseMatSize)));
  yPoints = static_cast<viennacl::vcl_size_t>(settings.sparseMatSize) / xPoints;  // remaining truncation error is negligible
  customSparseMatrixPath = settings.sparseCustomMatrix;
}

/*!
 * \brief Constructor with precision and settings
 * \param precision Benchmark precision
 * \param settings Settings from which to load matrix sizes & custom matrix path
 */
Benchmark_Sparse::Benchmark_Sparse(bool precision, BenchmarkSettings settings)
{
  Benchmark_Sparse();
  setPrecision(precision);
  xPoints = static_cast<viennacl::vcl_size_t>(std::sqrt(static_cast<double>(settings.sparseMatSize)));
  yPoints = static_cast<viennacl::vcl_size_t>(settings.sparseMatSize) / xPoints;  // remaining truncation error is negligible
  customSparseMatrixPath = settings.sparseCustomMatrix;
}

/*!
 * \brief Main benchmarking function
 * Should only be called by the \ref Benchmark_Sparse::execute() function,
 * since there are certain requirements that need to be fulfilled before starting the benchmarking procedure.
 */
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
  viennacl::sliced_ell_matrix<ScalarType> vcl_sliced_ell_matrix_1;

  viennacl::hyb_matrix<ScalarType, 1> vcl_hyb_matrix_1;

  std::vector< std::map<unsigned int, ScalarType> > stl_A;
  viennacl::tools::sparse_matrix_adapter<ScalarType> adapted_A(stl_A);
  if(customSparseMatrixPath.isEmpty()){
    //no custom matrix specified, generate the default one
    //  std::cout << "Generating Matrix..." << std::endl;
    viennacl::tools::generate_fdm_laplace(adapted_A, xPoints, yPoints );
  }
  else
  {
    //load matrix from file
    long success = viennacl::io::read_matrix_market_file(adapted_A, customSparseMatrixPath.toStdString());
    if(!success)
    {
      emit errorMessage("The matrix market reader cannot read the provided file. Only real-valued matrices in coordinate-format are supported.");//tell the GUI thread to show a popup error message
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
  viennacl::copy(adapted_A, vcl_sliced_ell_matrix_1);
  viennacl::copy(adapted_A, vcl_hyb_matrix_1);


  ///////////// Matrix operations /////////////////

  double tempResultValue;

//  std::cout << "------- Matrix-Vector product with compressed_matrix ----------" << std::endl;

  //when using a custom sparse matrix, the benchmark breaks here
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

  emit resultSignal("CSR format (no padding)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
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

  emit resultSignal("CSR format (padding 4)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
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

  emit resultSignal("CSR format (padding 8)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
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

  emit resultSignal("COO format", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
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

  emit resultSignal("ELL format", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();

  //  std::cout << "------- Matrix-Vector product with sliced_ell_matrix ----------" << std::endl;

  vcl_vec1 = viennacl::linalg::prod(vcl_sliced_ell_matrix_1, vcl_vec2); //startup calculation
  viennacl::backend::finish();

  timer.start();
  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
  {
    vcl_vec1 = viennacl::linalg::prod(vcl_sliced_ell_matrix_1, vcl_vec2);
  }
  viennacl::backend::finish();
  exec_time = timer.get();
  tempResultValue = printOps(2.0 * static_cast<double>(vcl_compressed_matrix_1.nnz()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

  emit resultSignal("Sliced ELL format", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
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

  emit resultSignal("HYB (ELL+CCSR)", testResultHolder.size(), tempResultValue, BAR_GRAPH, testId );
  testResultHolder.append(tempResultValue);
  emit testProgress();
}

/*!
 * \brief Begins the benchmark execution.
 */
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
  emit finalResultSignal("Sparse", testResultHolder[testResultHolder.size()-1]);
  emit benchmarkComplete();
}
