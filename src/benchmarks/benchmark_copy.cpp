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
  //  connect(this, SIGNAL(resultSignal(QString,double)), this, SLOT(updateBenchmarkData(QString,double)) );//TODO move json data saving to BenchmarkInstance model
}

Benchmark_Copy::Benchmark_Copy(bool precision, cl_platform_id platform, cl_device_id device)
{
  Benchmark_Copy();
  setPrecision(precision);
  setPlatform(platform);
  setDevice(device);
}

Benchmark_Copy::Benchmark_Copy(bool precision)//, cl_platform_id platform, cl_device_id device)
{
  Benchmark_Copy();
  setPrecision(precision);
//  setPlatform(platform);
//  setDevice(device);
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

  Timer timer;
  double exec_time_return = 0;
  double exec_time_complete = 0;
  int testId = 0;
  double tempResultValue;

  int MAX_BENCHMARK_VECTOR_SIZE = 15000000;
  int MIN_BENCHMARK_VECTOR_SIZE = 1000000;
  int INCREMENT_SIZE = 1000000;

  ///////////// Vector operations /////////////////

  //
  // Benchmark copy operation:
  //

  //host to device
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){
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
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){
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
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){
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
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){
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
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){
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
  for(int vectorSize = MIN_BENCHMARK_VECTOR_SIZE; vectorSize < MAX_BENCHMARK_VECTOR_SIZE; vectorSize += INCREMENT_SIZE){
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
  emit unitMeasureSignal("GB/s");

#ifndef NDEBUG
  std::cout << std::endl;
  std::cout << " ******************************************************************" << std::endl;
  std::cout << " **** WARNING: This is not a release build." << std::endl;
  std::cout << " ****          Performance numbers are therefore lower than normal. " << std::endl;
  std::cout << " ******************************************************************" << std::endl;
  std::cout << std::endl;
#endif

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
