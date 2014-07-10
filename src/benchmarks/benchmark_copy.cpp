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
  finalResultCounter = 0;
  finalResultValue = 0;
  connect(this, SIGNAL(resultSignal(QString,double)), this, SLOT(updateBenchmarkData(QString,double)) );
}

template<typename ScalarType>
void Benchmark_Copy::run_benchmark()
{

  Timer timer;
  double exec_time_return = 0;
  double exec_time_complete = 0;

  std::vector<ScalarType> std_vec1(BENCHMARK_VECTOR_SIZE);
  std::vector<ScalarType> std_vec2(BENCHMARK_VECTOR_SIZE);
  viennacl::vector<ScalarType> vcl_vec1(BENCHMARK_VECTOR_SIZE);
  viennacl::vector<ScalarType> vcl_vec2(BENCHMARK_VECTOR_SIZE);


  ///////////// Vector operations /////////////////

  std_vec1[0] = 1.0;
  std_vec2[0] = 1.0;
  for (std::size_t i=1; i<BENCHMARK_VECTOR_SIZE; ++i)
  {
    std_vec1[i] = std_vec1[i-1] * ScalarType(1.000001);
    std_vec2[i] = std_vec1[i-1] * ScalarType(0.999999);
  }

  // warmup:
  viennacl::copy(std_vec1, vcl_vec1);
  viennacl::fast_copy(std_vec2, vcl_vec2);
  viennacl::async_copy(std_vec2, vcl_vec1);
  viennacl::backend::finish();

  //
  // Benchmark copy operation:
  //
  timer.start();
  viennacl::copy(std_vec1, vcl_vec1);
  exec_time_return = timer.get();
  viennacl::backend::finish();
  exec_time_complete = timer.get();
  std::cout << " *** viennacl::copy(), host to device ***" << std::endl;
  std::cout << "  - Time to function return: " << exec_time_return << std::endl;
  std::cout << "  - Time to completion: " << exec_time_complete << std::endl;
  std::cout << "  - Estimated effective bandwidth: " << BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9 << " GB/sec" << std::endl;
  emit resultSignal("viennacl::copy(), host to device", BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9);
  finalResultValue += BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9;
  finalResultCounter++;

  timer.start();
  viennacl::copy(vcl_vec1, std_vec1);
  exec_time_return = timer.get();
  viennacl::backend::finish();
  exec_time_complete = timer.get();
  std::cout << " *** viennacl::copy(), device to host ***" << std::endl;
  std::cout << "  - Time to function return: " << exec_time_return << std::endl;
  std::cout << "  - Time to completion: " << exec_time_complete << std::endl;
  std::cout << "  - Estimated effective bandwidth: " << BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9 << " GB/sec" << std::endl;
  emit resultSignal("viennacl::copy(), device to host", BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9);
  finalResultValue += BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9;
  finalResultCounter++;


  //
  // Benchmark fast_copy operation:
  //
  timer.start();
  viennacl::fast_copy(std_vec1, vcl_vec1);
  exec_time_return = timer.get();
  viennacl::backend::finish();
  exec_time_complete = timer.get();
  std::cout << " *** viennacl::fast_copy(), host to device ***" << std::endl;
  std::cout << "  - Time to function return: " << exec_time_return << std::endl;
  std::cout << "  - Time to completion: " << exec_time_complete << std::endl;
  std::cout << "  - Estimated effective bandwidth: " << BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9 << " GB/sec" << std::endl;
  emit resultSignal("viennacl::fast_copy(), host to device", BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9);
  finalResultValue += BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9;
  finalResultCounter++;

  timer.start();
  viennacl::fast_copy(vcl_vec1, std_vec1);
  exec_time_return = timer.get();
  viennacl::backend::finish();
  exec_time_complete = timer.get();
  std::cout << " *** viennacl::fast_copy(), device to host ***" << std::endl;
  std::cout << "  - Time to function return: " << exec_time_return << std::endl;
  std::cout << "  - Time to completion: " << exec_time_complete << std::endl;
  std::cout << "  - Estimated effective bandwidth: " << BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9 << " GB/sec" << std::endl;
  emit resultSignal("viennacl::fast_copy(), device to host", BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9);
  finalResultValue += BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9;
  finalResultCounter++;

  //
  // Benchmark async_copy operation:
  //
  timer.start();
  viennacl::async_copy(vcl_vec1, std_vec1);
  exec_time_return = timer.get();
  viennacl::backend::finish();
  exec_time_complete = timer.get();
  std::cout << " *** viennacl::async_copy(), host to device ***" << std::endl;
  std::cout << "  - Time to function return: " << exec_time_return << std::endl;
  std::cout << "  - Time to completion: " << exec_time_complete << std::endl;
  std::cout << "  - Estimated effective bandwidth: " << BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9 << " GB/sec" << std::endl;
  emit resultSignal("viennacl::async_copy(), host to device", BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9);
  finalResultValue += BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9;
  finalResultCounter++;

  timer.start();
  viennacl::async_copy(vcl_vec1, std_vec1);
  exec_time_return = timer.get();
  viennacl::backend::finish();
  exec_time_complete = timer.get();
  std::cout << " *** viennacl::async_copy(), device to host ***" << std::endl;
  std::cout << "  - Time to function return: " << exec_time_return << std::endl;
  std::cout << "  - Time to completion: " << exec_time_complete << std::endl;
  std::cout << "  - Estimated effective bandwidth: " << BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9 << " GB/sec" << std::endl;
  emit resultSignal("viennacl::async_copy(), device to host", BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9);
  finalResultValue += BENCHMARK_VECTOR_SIZE * sizeof(ScalarType) / exec_time_complete / 1e9;
  finalResultCounter++;

}

void Benchmark_Copy::execute(){
  emit benchmarkStarted(COPY);
  emit unitMeasureSignal("GB/s");
  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "               Device Info" << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
#ifdef VIENNACL_WITH_OPENCL
  std::cout << "OPENCL IS ENABLED!" << std::endl;
  std::cout << viennacl::ocl::current_device().info() << std::endl;
#endif


#ifndef NDEBUG
  std::cout << std::endl;
  std::cout << " ******************************************************************" << std::endl;
  std::cout << " **** WARNING: This is not a release build." << std::endl;
  std::cout << " ****          Performance numbers are therefore lower than normal. " << std::endl;
  std::cout << " ******************************************************************" << std::endl;
  std::cout << std::endl;
#endif


  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "## Benchmark :: Vector" << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << std::endl;
  std::cout << "   -------------------------------" << std::endl;
  std::cout << "   # benchmarking single-precision" << std::endl;
  std::cout << "   -------------------------------" << std::endl;
  run_benchmark<float>();
#ifdef VIENNACL_WITH_OPENCL
  if( viennacl::ocl::current_device().double_support() )
#endif
  {
    std::cout << std::endl;
    std::cout << "   -------------------------------" << std::endl;
    std::cout << "   # benchmarking double-precision" << std::endl;
    std::cout << "   -------------------------------" << std::endl;
    run_benchmark<double>();
  }
  emit finalResultSignal("Copy", finalResultValue/finalResultCounter);
  emit benchmarkComplete();
}

void Benchmark_Copy::updateBenchmarkData(QString benchmarkName, double bandwidthValue)
{
  benchmarkNames.append(benchmarkName);
  dataPoints.append(bandwidthValue);
}

//QVariant Benchmark_Copy::getJsonData(){
//  QJsonObject jsonObject;
//  QJsonArray labels;

//  foreach (QString benchmarkName, benchmarkNames) {
//    labels.append(benchmarkName);
//  }
//  jsonObject["labels"] = labels;

//  QJsonArray dataPointsJsonArray;
//  foreach (double dataPoint, dataPoints) {
//    dataPointsJsonArray.append(dataPoint);
//  }
//  QString color = "rgba(0,0,220,0.5)";
//  QJsonObject chartBarData;
//    chartBarData["fillColor"] = color;
//    chartBarData["strokeColor"] = color;
//    chartBarData["data"] = dataPointsJsonArray;

//    QJsonArray datasets;
//  datasets.append(chartBarData);

//  jsonObject["datasets"] = datasets;

//  QJsonDocument jsonDoc(jsonObject);
////  qDebug()<<"json output";
////  qDebug()<<jsonDoc.toBinaryData();
//  qDebug()<<"---toJson---";
////  qDebug()<<jsonDoc.toJson(QJsonDocument::Compact);
////  return jsonDoc.toJson();
//  dataPoints.clear();
//  benchmarkNames.clear();
//  return jsonDoc.toVariant();
//}
