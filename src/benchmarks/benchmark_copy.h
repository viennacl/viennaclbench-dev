#ifndef BENCHMARK_COPY_H
#define BENCHMARK_COPY_H

#include "abstractbenchmark.h"

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/linalg/inner_prod.hpp"
#include "viennacl/linalg/norm_2.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>
#include "benchmark-utils.hpp"

using std::cout;
using std::cin;
using std::endl;

#define BENCHMARK_VECTOR_SIZE   10000000
#define BENCHMARK_RUNS          10

//#include <QJsonArray>
//#include <QJsonDocument>
//#include <QJsonObject>

#include <QThread>
#include <QVector>
#include <QVariant>
#include <QStringList>
#include <QString>

class Benchmark_Copy : public AbstractBenchmark
{
  Q_OBJECT
public:
  explicit Benchmark_Copy(QObject *parent = 0);
  Benchmark_Copy(bool precision, BenchmarkSettings settings);

  template<typename ScalarType>
  void run_benchmark();
  template<typename ScalarType>
  void resizeVectors(int size, std::vector<ScalarType> &std_vec1, std::vector<ScalarType> &std_vec2,
                     viennacl::vector<ScalarType> &vcl_vec1, viennacl::vector<ScalarType> &vcl_vec2);
private:
  int MAX_BENCHMARK_VECTOR_SIZE;
  int MIN_BENCHMARK_VECTOR_SIZE;
  int INCREMENT_FACTOR;

  QStringList benchmarkNames;
  QList<double> dataPoints;

  QVector<double> testResultHolder;
signals:
  /* Inherited signals:
   * void benchmarkStarted(int benchmarkIdNumber);
   * void finalResultSignal(QString benchmarkName, double finalValue);
   * void resultSignal(QString benchmarkName, double bandwidthValue);
   * void benchmarkComplete();
   * void unitMeasureSignal(QString unitMeasureName);
   * void testProgress();
   * */
public slots:
  void execute();
  void updateBenchmarkData(QString benchmarkName, double bandwidthValue);
  //  QVariant getJsonData();
};

#endif // BENCHMARK_COPY_H
