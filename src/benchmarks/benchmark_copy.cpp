/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */


#include "benchmark_copy.h"
#include <stdexcept>
#include <QDebug>

#ifndef BENCHMARK_RUNS
  #define BENCHMARK_RUNS 10
#endif

/*!
 * \brief Default constructor.
 * Sets the precision to double and loads default vector sizes & increment factor from \ref BenchmarkSettings
 * \param parent Optional parent object.
 */
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

/*!
 * \brief Constructor with precision and settings
 * \param precision Benchmark precision
 * \param settings Settings from which to load vector sizes & increment factor
 */
Benchmark_Copy::Benchmark_Copy(bool precision, BenchmarkSettings settings)
{
  Benchmark_Copy();
  setPrecision(precision);
  MAX_BENCHMARK_VECTOR_SIZE = settings.copyMaxVectorSize;
  MIN_BENCHMARK_VECTOR_SIZE = settings.copyMinVectorSize;
  INCREMENT_FACTOR = settings.copyIncFactor;
}


template<typename ScalarType>
/*!
 * \brief Resizes the benchmark vectors to \a size.
 * This function is called each time the benchmark vectors need to be increased by \ref INCREMENT_FACTOR
 * \param size New vector size.
 * \param std_vec1 std vector 1
 * \param std_vec2 std vector 2
 * \param vcl_vec1 vcl vector 1
 * \param vcl_vec2 vcl vector 2
 */
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
}

/*!
 * \brief Main benchmarking function
 * Should only be called by the \ref Benchmark_Copy::execute() function,
 * since there are certain requirements that need to be fulfilled before starting the benchmarking procedure.
 */
template<typename ScalarType>
void Benchmark_Copy::run_benchmark()
{
  std::vector<ScalarType> std_vec1;
  std::vector<ScalarType> std_vec2;
  viennacl::vector<ScalarType> vcl_vec1;
  viennacl::vector<ScalarType> vcl_vec2;

//  std::cout << "Benchmarking..." << std::endl;
//  std::cout << "Platform id: "<< viennacl::ocl::current_context().platform_index() //platform id != context id
//            <<" Context value: " << viennacl::ocl::current_context().handle().get() << std::endl;

//  std::cout << "Running on device name: "<< viennacl::ocl::current_device().name() << std::endl;

  Timer timer;
  double exec_time_complete = 0;
  int testId = 0;
  double tempResultValue;

  ///////////// Vector operations /////////////////

  //
  // Benchmark fast_copy operation:
  //

  try
  {
    //host to device
    for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
      resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

      // warmup operation:
      viennacl::fast_copy(std_vec1, vcl_vec1);
      viennacl::backend::finish();

      // timings:
      timer.start();
      for (std::size_t i=0; i<BENCHMARK_RUNS; ++i)
      {
        viennacl::fast_copy(std_vec1, vcl_vec1);
        viennacl::backend::finish();
      }
      exec_time_complete = timer.get();

      tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete * BENCHMARK_RUNS / 1e9;
      emit resultSignal("Copy Host -> Device", vectorSize, tempResultValue, LINE_GRAPH, testId);
      testResultHolder.append(tempResultValue);
    }
    testId++;
    emit testProgress();

    //device to host
    for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR){
      resizeVectors<ScalarType>(vectorSize, std_vec1, std_vec2, vcl_vec1, vcl_vec2);

      // warmup operation:
      viennacl::fast_copy(vcl_vec1, std_vec1);
      viennacl::backend::finish();

      timer.start();
      for (std::size_t i=0; i<BENCHMARK_RUNS; ++i)
      {
        viennacl::fast_copy(vcl_vec1, std_vec1);
        viennacl::backend::finish();
      }
      exec_time_complete = timer.get();

      tempResultValue = vectorSize * sizeof(ScalarType) / exec_time_complete * BENCHMARK_RUNS / 1e9;
      emit resultSignal("Copy Device -> Host", vectorSize, tempResultValue, LINE_GRAPH, testId);
      testResultHolder.append(tempResultValue);
    }
    testId++;
    emit testProgress();

  }
  catch (std::exception const & e)
  {
    emit errorMessage("Execution of copy benchmark failed. Skipping benchmark...");
  }


}

/*!
 * \brief Begins the benchmark execution.
 */
void Benchmark_Copy::execute(){
  emit benchmarkStarted(COPY);
  emit unitMeasureSignal("GB/s", Qt::YAxis);
  emit unitMeasureSignal("Vector Size", Qt::XAxis);

  if(getPrecision() == SINGLE_PRECISION)
    run_benchmark<float>();
  else if( getPrecision() == DOUBLE_PRECISION
#ifdef VIENNACL_WITH_OPENCL
           && viennacl::ocl::current_device().double_support()
#endif
           )
    run_benchmark<double>();

  qSort(testResultHolder);//sort test results in ascending order
  emit finalResultSignal("Copy", testResultHolder[testResultHolder.size()-1]);
  emit benchmarkComplete();
}
