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
*   Benchmark:   Performance of viennacl::copy(), viennacl::fast_copy(), and viennacl::async_copy()
*
*/

#include "benchmark_copy.h"
#include <QDebug>

Benchmark_Copy::Benchmark_Copy(QObject *parent) :
  AbstractBenchmark(parent)
{
  testResultHolder.clear();
  setPrecision(DOUBLE_PRECISION);
  BenchmarkSettings settings;
  MAX_BENCHMARK_VECTOR_SIZE = settings.copyMaxVectorSize;
  MIN_BENCHMARK_VECTOR_SIZE = settings.copyMinVectorSize;
  INCREMENT_FACTOR = settings.copyIncFactor;
}

Benchmark_Copy::Benchmark_Copy(bool precision, BenchmarkSettings settings)
{
  Benchmark_Copy();
  setPrecision(precision);
  MAX_BENCHMARK_VECTOR_SIZE = settings.copyMaxVectorSize;
  MIN_BENCHMARK_VECTOR_SIZE = settings.copyMinVectorSize;
  INCREMENT_FACTOR = settings.copyIncFactor;
}

template<typename ScalarType>
void Benchmark_Copy::resizeVectors(int size, std::vector<ScalarType> &std_vec1, std::vector<ScalarType> &std_vec2,
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
void Benchmark_Copy::run_benchmark()
{
  std::vector<ScalarType> std_vec1;
  std::vector<ScalarType> std_vec2;
  viennacl::vector<ScalarType> vcl_vec1;
  viennacl::vector<ScalarType> vcl_vec2;

  std::cout << "Benchmarking..." << std::endl;
  std::cout << "Platform id: "<< viennacl::ocl::current_context().platform_index() //platform id != context id
            <<" Context value: " << viennacl::ocl::current_context().handle().get() << std::endl;

  std::cout << "Running on device name: "<< viennacl::ocl::current_device().name() << std::endl;
  std::cout << MIN_BENCHMARK_VECTOR_SIZE << "|||" <<MAX_BENCHMARK_VECTOR_SIZE<<"|||"<<INCREMENT_FACTOR<< std::endl;

  Timer timer;
  double exec_time_return = 0;
  double exec_time_complete = 0;
  int testId = 0;
  double tempResultValue;

  ///////////// Vector operations /////////////////

  //
  // Benchmark copy operation:
  //

  //host to device
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
    resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

    timer.start();
    viennacl::copy(std_vec1, vcl_vec1);
    exec_time_return = timer.get();
    viennacl::backend::finish();
    exec_time_complete = timer.get();

    tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete / 1e9;
    emit resultSignal("viennacl::copy(), host to device", vectorSize, tempResultValue, LINE_GRAPH, testId);
    testResultHolder.append(tempResultValue);
  }
  testId++;
  emit testProgress();

  //device to host
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
    resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

    timer.start();
    viennacl::copy(vcl_vec1, std_vec1);
    exec_time_return = timer.get();
    viennacl::backend::finish();
    exec_time_complete = timer.get();

    tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete / 1e9;
    emit resultSignal("viennacl::copy(), device to host", vectorSize, tempResultValue, LINE_GRAPH, testId);
    testResultHolder.append(tempResultValue);
  }
  testId++;
  emit testProgress();


  //
  // Benchmark fast_copy operation:
  //

  //host to device
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
    resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

    timer.start();
    viennacl::fast_copy(std_vec1, vcl_vec1);
    exec_time_return = timer.get();
    viennacl::backend::finish();
    exec_time_complete = timer.get();

    tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete / 1e9;
    emit resultSignal("viennacl::fast_copy(), host to device", vectorSize, tempResultValue, LINE_GRAPH, testId);
    testResultHolder.append(tempResultValue);
  }
  testId++;
  emit testProgress();

  //device to host
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
    resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

    timer.start();
    viennacl::fast_copy(vcl_vec1, std_vec1);
    exec_time_return = timer.get();
    viennacl::backend::finish();
    exec_time_complete = timer.get();

    tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete / 1e9;
    emit resultSignal("viennacl::fast_copy(), device to host", vectorSize, tempResultValue, LINE_GRAPH, testId);
    testResultHolder.append(tempResultValue);
  }
  testId++;
  emit testProgress();

  //
  // Benchmark async_copy operation:
  //

  //host to device
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
    resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

    timer.start();
    viennacl::async_copy(vcl_vec1, std_vec1);
    exec_time_return = timer.get();
    viennacl::backend::finish();
    exec_time_complete = timer.get();

    tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete / 1e9;
    emit resultSignal("viennacl::async_copy(), host to device", vectorSize, tempResultValue, LINE_GRAPH, testId);
    testResultHolder.append(tempResultValue);
  }
  testId++;
  emit testProgress();

  //device to host
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
    resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);
    timer.start();
    viennacl::async_copy(vcl_vec1, std_vec1);
    exec_time_return = timer.get();
    viennacl::backend::finish();
    exec_time_complete = timer.get();
    tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete / 1e9;
    emit resultSignal("viennacl::async_copy(), device to host", vectorSize, tempResultValue, LINE_GRAPH, testId);
    testResultHolder.append(tempResultValue);
  }
  testId++;
  emit testProgress();
}

void Benchmark_Copy::execute(){
  emit benchmarkStarted(COPY);
  emit unitMeasureSignal("GB/s", Qt::YAxis);
  emit unitMeasureSignal("Vector Size", Qt::XAxis);

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
  emit finalResultSignal("Copy", testResultHolder[testResultHolder.size()/2]);
  emit benchmarkComplete();
}
