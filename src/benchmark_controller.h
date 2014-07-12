#ifndef BENCHMARK_CONTROLLER_H
#define BENCHMARK_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QQueue>

#include "benchmarks/abstractbenchmark.h"
#include "benchmarks/benchmark_copy.h"
#include "benchmarks/benchmark_vector.h"
#include "benchmarks/benchmark_sparse.h"
#include "benchmarks/benchmark_blas3.h"

#include "benchmarks/benchmark_scheduler.h"
//#include "benchmarks/benchmark_solver.h"
//#include "benchmarks/benchmark_qr.h"

class Benchmark_Controller : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Controller(QObject *parent = 0);
  void enqueueBenchmarks(QStringList benchmarkNames);
private:
  QQueue<QString> benchmarkQ;
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void finalResultSignal(QString benchmarkName, double finalValue);
  void benchmarkComplete();
  void unitMeasureSignal(QString);
  void emptyBenchmarkQ();
  void benchmarkStarted(int benchmarkIdNumber);
public slots:
  void benchmarkStartedSlot(int benchmarkIdNumber);
  void executeSelectedBenchmark(QStringList benchmarkNamesList);
  void finalResultSignalSlot(QString benchmarkName, double finalValue);
  void resultSignalSlot(QString benchmarkName, double bandwidthValue);
  void benchmarkCompleteSlot();
  void unitMeasureSignalSlot(QString unitMeasureName);
  void startNextBenchmark();
  void createBenchmark(AbstractBenchmark *benchmark);
};

#endif // BENCHMARK_CONTROLLER_H
