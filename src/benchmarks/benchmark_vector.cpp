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
*   Benchmark:   Vector operations (vector.cpp and vector.cu are identical, the latter being required for compilation using CUDA nvcc)
*
*/

#include "benchmark_vector.h"
#include <stdexcept>

// inner prod
struct exec_inner_prod_host
{
  template<typename HostVectorT, typename DeviceVectorT, typename DeviceScalarT>
  static void apply(  HostVectorT &   host_x,   HostVectorT &   host_y,   HostVectorT &   host_z,
                    DeviceVectorT & device_x, DeviceVectorT & device_y, DeviceVectorT & device_z, DeviceScalarT & device_alpha)
  {
    typename HostVectorT::value_type  result;

    for (std::size_t i=0; i<host_x.size(); ++i)
      result += host_x[i] * host_y[i];

    host_z[0] = result;
  }
};

struct exec_inner_prod_device
{
  template<typename HostVectorT, typename DeviceVectorT, typename DeviceScalarT>
  static void apply(  HostVectorT &   host_x,   HostVectorT &   host_y,   HostVectorT &   host_z,
                    DeviceVectorT & device_x, DeviceVectorT & device_y, DeviceVectorT & device_z, DeviceScalarT & device_alpha)
  {
    device_alpha = viennacl::linalg::inner_prod(device_x, device_y);
  }
};

// addition
struct exec_addition_host
{
  template<typename HostVectorT, typename DeviceVectorT, typename DeviceScalarT>
  static void apply(  HostVectorT &   host_x,   HostVectorT &   host_y,   HostVectorT &   host_z,
                    DeviceVectorT & device_x, DeviceVectorT & device_y, DeviceVectorT & device_z, DeviceScalarT & device_alpha)
  {
    for (std::size_t i=0; i<host_x.size(); ++i)
      host_z[i] = host_x[i] * host_y[i];
  }
};

struct exec_addition_device
{
  template<typename HostVectorT, typename DeviceVectorT, typename DeviceScalarT>
  static void apply(  HostVectorT &   host_x,   HostVectorT &   host_y,   HostVectorT &   host_z,
                    DeviceVectorT & device_x, DeviceVectorT & device_y, DeviceVectorT & device_z, DeviceScalarT & device_alpha)
  {
    device_z = device_x + device_y;
  }
};

// mult-add
struct exec_multadd_host
{
  template<typename HostVectorT, typename DeviceVectorT, typename DeviceScalarT>
  static void apply(  HostVectorT &   host_x,   HostVectorT &   host_y,   HostVectorT &   host_z,
                    DeviceVectorT & device_x, DeviceVectorT & device_y, DeviceVectorT & device_z, DeviceScalarT & device_alpha)
  {
    for (std::size_t i=0; i<host_x.size(); ++i)
      host_z[i] += host_x[0] * host_y[i];
  }
};

struct exec_multadd_device
{
  template<typename HostVectorT, typename DeviceVectorT, typename DeviceScalarT>
  static void apply(  HostVectorT &   host_x,   HostVectorT &   host_y,   HostVectorT &   host_z,
                    DeviceVectorT & device_x, DeviceVectorT & device_y, DeviceVectorT & device_z, DeviceScalarT & device_alpha)
  {
    device_z += device_alpha * device_y;
  }
};



/*!
 * \brief Default constructor.
 * Sets the precision to double and load default vector sizes & increment factor from \ref BenchmarkSettings
 * \param parent Optional parent object.
 */
Benchmark_Vector::Benchmark_Vector(QObject *parent) :
  AbstractBenchmark(parent)
{
  testResultHolder.clear();
  setPrecision(DOUBLE_PRECISION);
  BenchmarkSettings settings;
  MAX_BENCHMARK_VECTOR_SIZE = settings.vectorMaxVectorSize;
  MIN_BENCHMARK_VECTOR_SIZE = settings.vectorMinVectorSize;
  INCREMENT_FACTOR = settings.vectorIncFactor;
}

/*!
 * \brief Constructor with precision and settings
 * \param precision Benchmark precision
 * \param settings Settings from which to load vector sizes & increment factor
 */
Benchmark_Vector::Benchmark_Vector(bool precision, BenchmarkSettings settings)
{
  Benchmark_Vector();
  setPrecision(precision);
  MAX_BENCHMARK_VECTOR_SIZE = settings.vectorMaxVectorSize;
  MIN_BENCHMARK_VECTOR_SIZE = settings.vectorMinVectorSize;
  INCREMENT_FACTOR = settings.vectorIncFactor;
}


/*!
 * \brief Main benchmarking function
 * Should only be called by the \ref Benchmark_Vector::execute() function,
 * since there are certain requirements that need to be fulfilled before starting the benchmarking procedure.
 */
template<typename ScalarType>
void Benchmark_Vector::run_benchmark()
{
  try
  {
    run_benchmark_impl<ScalarType, exec_inner_prod_host  >("Inner Product - Host",   2, 0);
    run_benchmark_impl<ScalarType, exec_inner_prod_device>("Inner Product - Device", 2, 1);

    run_benchmark_impl<ScalarType, exec_addition_host  >("Addition - Host",   3, 2);
    run_benchmark_impl<ScalarType, exec_addition_device>("Addition - Device", 3, 3);

    run_benchmark_impl<ScalarType, exec_multadd_host  >("Multiply&Add - Host",   3, 4);
    run_benchmark_impl<ScalarType, exec_multadd_device>("Multiply&Add - Device", 3, 5);
  }
  catch (std::exception const & e)
  {
    emit errorMessage("Execution of vector benchmark failed. Skipping benchmark...");
  }


}


template<typename ScalarType, typename FuncT>
void Benchmark_Vector::run_benchmark_impl(std::string testlabel, std::size_t entries_per_op, int testId)
{
  Timer timer;
  double exec_time;
  double effective_bandwidth;

  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize <= MAX_BENCHMARK_VECTOR_SIZE; vectorSize *= INCREMENT_FACTOR)
  {
    std::vector<ScalarType> std_vec1(vectorSize, ScalarType(1.0));
    std::vector<ScalarType> std_vec2(vectorSize, ScalarType(1.1));
    std::vector<ScalarType> std_vec3(vectorSize, ScalarType(1.2));
    viennacl::vector<ScalarType> vcl_vec1(vectorSize);
    viennacl::vector<ScalarType> vcl_vec2(vectorSize);
    viennacl::vector<ScalarType> vcl_vec3(vectorSize);

    viennacl::scalar<ScalarType> vcl_alpha(0);

    viennacl::copy(std_vec1, vcl_vec1);
    viennacl::copy(std_vec2, vcl_vec2);

    // warmup:
    FuncT::apply(std_vec1, std_vec2, std_vec3,
                 vcl_vec1, vcl_vec2, vcl_vec3, vcl_alpha);

    viennacl::backend::finish();
    timer.start();
    for (std::size_t runs=0; runs<BENCHMARK_RUNS; ++runs)
      FuncT::apply(std_vec1, std_vec2, std_vec3,
                   vcl_vec1, vcl_vec2, vcl_vec3, vcl_alpha);
    viennacl::backend::finish();
    exec_time = timer.get();

    if (std_vec3[0] > 0) // trivially true, but ensures nothing gets optimized away
      effective_bandwidth = entries_per_op * vectorSize * sizeof(ScalarType) / exec_time * BENCHMARK_RUNS / 1e9;

    emit resultSignal(testlabel.c_str(), vectorSize, effective_bandwidth, LINE_GRAPH, testId );
    testResultHolder.append(effective_bandwidth);
  }
  emit testProgress();
}

/*!
 * \brief Begins the benchmark execution.
 */
void Benchmark_Vector::execute()
{
  emit benchmarkStarted(VECTOR);
  emit unitMeasureSignal("GB/sec", Qt::YAxis);
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
  emit finalResultSignal("Vector", testResultHolder[testResultHolder.size()-1]);
  emit benchmarkComplete();
}
