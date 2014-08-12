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
*   Benchmark:   Vector operations (vector.cpp and vector.cu are identical, the latter being required for compilation using CUDA nvcc)
*
*/

#include "benchmark_vector.h"

Benchmark_Vector::Benchmark_Vector(QObject *parent) :
  AbstractBenchmark(parent)
{
  testResultHolder.clear();
  setPrecision(DOUBLE_PRECISION);
}

Benchmark_Vector::Benchmark_Vector(bool precision)
{
  Benchmark_Vector();
  setPrecision(precision);
}

template<typename ScalarType>
void Benchmark_Vector::resizeVectors(int size, std::vector<ScalarType> &std_vec1, std::vector<ScalarType> &std_vec2,
                                     viennacl::vector<ScalarType> &vcl_vec1, viennacl::vector<ScalarType> &vcl_vec2){

  std_vec1.resize(size);
  std_vec2.resize(size);
  vcl_vec1.resize(size);
  vcl_vec2.resize(size);

  std_vec1[0] = 1.0;
  std_vec2[0] = 1.0;
  for (std::size_t i=1; i<size; ++i)
  {
    std_vec1[i] = std_vec1[i-1] * ScalarType(1.000001);
    std_vec2[i] = std_vec1[i-1] * ScalarType(0.999999);
  }

  // warmup:
  viennacl::copy(std_vec1, vcl_vec1);
  viennacl::fast_copy(std_vec2, vcl_vec2);
  viennacl::async_copy(std_vec2, vcl_vec1);
  viennacl::backend::finish();
}

template<typename ScalarType>
void Benchmark_Vector::run_benchmark()
{
  Timer timer;
  double exec_time;


  int MAX_BENCHMARK_VECTOR_SIZE = 15000000;
  int MIN_BENCHMARK_VECTOR_SIZE = 1000000;
  int INCREMENT_SIZE = 1000000;

  /* HOLD MY BEER, IMMA GONNA FORLOOP EVERYTHING */
  /* Seriously, I run the entire test suite for each vector size */

  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize <= MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){

    int testId = 0;
    ScalarType std_result = 0;

    ScalarType std_factor1 = static_cast<ScalarType>(3.1415);
    ScalarType std_factor2 = static_cast<ScalarType>(42.0);
    viennacl::scalar<ScalarType> vcl_factor1(std_factor1);
    viennacl::scalar<ScalarType> vcl_factor2(std_factor2);

    std::vector<ScalarType> std_vec1(vectorSize);
    std::vector<ScalarType> std_vec2(vectorSize);
    std::vector<ScalarType> std_vec3(vectorSize);
    viennacl::vector<ScalarType> vcl_vec1(vectorSize);
    viennacl::vector<ScalarType> vcl_vec2(vectorSize);
    viennacl::vector<ScalarType> vcl_vec3(vectorSize);


    ///////////// Vector operations /////////////////

    double tempResultValue;

    std_vec1[0] = 1.0;
    std_vec2[0] = 1.0;
    for (std::size_t i=1; i<vectorSize; ++i)
    {
      std_vec1[i] = std_vec1[i-1] * ScalarType(1.000001);
      std_vec2[i] = std_vec1[i-1] * ScalarType(0.999999);
    }

    viennacl::copy(std_vec1, vcl_vec1);
    viennacl::fast_copy(std_vec1, vcl_vec1);
    viennacl::copy(std_vec2, vcl_vec2);

    viennacl::swap(vcl_vec1, vcl_vec2);
    //check that vcl_vec1 is now equal to std_vec2:
    viennacl::fast_copy(vcl_vec1, std_vec3);
    for (std::size_t i=0; i<vectorSize; ++i)
      if (std_vec3[i] != std_vec2[i])
        std::cout << "ERROR in swap(): Failed at entry " << i << std::endl;

    viennacl::fast_swap(vcl_vec1, vcl_vec2);
    //check that vcl_vec1 is now equal to std_vec1 again:
    viennacl::copy(vcl_vec1, std_vec3);
    for (std::size_t i=0; i<vectorSize; ++i)
      if (std_vec3[i] != std_vec1[i])
        std::cout << "ERROR in fast_swap(): Failed at entry " << i << std::endl;


    // inner product
    viennacl::backend::finish();

    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      std_result = 0;
      for (std::size_t i=0; i<vectorSize; ++i)
        std_result += std_vec1[i] * std_vec2[i];
    }
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

    emit resultSignal("Vector inner products - CPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    std_result = viennacl::linalg::inner_prod(vcl_vec1, vcl_vec2); //startup calculation
    std_result = 0.0;
    viennacl::backend::finish();
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      vcl_factor2 = viennacl::linalg::inner_prod(vcl_vec1, vcl_vec2);
    }
    viennacl::backend::finish();
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

    emit resultSignal("Vector inner products - GPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    // inner product
    viennacl::backend::finish();

    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      std_result = 0;
      for (std::size_t i=0; i<vectorSize; ++i)
      {
        ScalarType entry = std_vec1[i];
        std_result += entry * entry;
      }
    }
    std_result = std::sqrt(std_result);
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

    emit resultSignal("Vector norm_2 - CPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    std_result = viennacl::linalg::norm_2(vcl_vec1); //startup calculation
    std_result = 0.0;
    viennacl::backend::finish();
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      vcl_factor2 = viennacl::linalg::norm_2(vcl_vec1);
    }
    viennacl::backend::finish();
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));

    emit resultSignal("Vector norm_2 - GPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    // vector addition

    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      for (std::size_t i=0; i<vectorSize; ++i)
        std_vec3[i] = std_vec1[i] + std_vec2[i];
    }
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));
    emit resultSignal("Vector addition - CPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    vcl_vec3 = vcl_vec1 + vcl_vec2; //startup calculation
    viennacl::backend::finish();
    std_result = 0.0;
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      vcl_vec3 = vcl_vec1 + vcl_vec2;
    }
    viennacl::backend::finish();
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));
    emit resultSignal("Vector addition - GPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();


    // multiply add:
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      for (std::size_t i=0; i<vectorSize; ++i)
        std_vec1[i] += std_factor1 * std_vec2[i];
    }
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));
    emit resultSignal("Vector multiply add - CPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    vcl_vec1 += vcl_factor1 * vcl_vec2; //startup calculation
    viennacl::backend::finish();
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      vcl_vec1 += vcl_factor1 * vcl_vec2;
    }
    viennacl::backend::finish();
    exec_time = timer.get();

    tempResultValue = printOps(2.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));
    emit resultSignal("Vector multiply add - GPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();



    //complicated vector addition:
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      for (std::size_t i=0; i<vectorSize; ++i)
        std_vec3[i] += std_vec2[i] / std_factor1 + std_factor2 * (std_vec1[i] - std_factor1 * std_vec2[i]);
    }
    exec_time = timer.get();

    tempResultValue = printOps(6.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));
    emit resultSignal("Vector complicated expression - CPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();

    vcl_vec3 = vcl_vec2 / vcl_factor1 + vcl_factor2 * (vcl_vec1 - vcl_factor1*vcl_vec2); //startup calculation
    viennacl::backend::finish();
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
    {
      vcl_vec3 = vcl_vec2 / vcl_factor1 + vcl_factor2 * (vcl_vec1 - vcl_factor1*vcl_vec2);
    }
    viennacl::backend::finish();
    exec_time = timer.get();

    tempResultValue = printOps(6.0 * static_cast<double>(std_vec1.size()), static_cast<double>(exec_time) / static_cast<double>(BENCHMARK_RUNS));
    emit resultSignal("Vector complicated expression - GPU", vectorSize, tempResultValue, LINE_GRAPH, testId );
    testId++;
    testResultHolder.append(tempResultValue);
    emit testProgress();
  }
}

void Benchmark_Vector::execute()
{
  emit benchmarkStarted(VECTOR);
  emit unitMeasureSignal("GFLOPs");

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
  emit finalResultSignal("Vector", testResultHolder[testResultHolder.length()/2]);
  emit benchmarkComplete();
}
