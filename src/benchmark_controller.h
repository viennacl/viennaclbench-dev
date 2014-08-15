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
  void stopExecution();
private:
  QQueue<QString> benchmarkQ;
  void enqueueBenchmarks(QStringList benchmarkNames);
  bool precision; // false(0) - SINGLE | true(1) - DOUBLE
  QThread *currentBenchmarkThread;
signals:
  void resultSignal(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void finalResultSignal(QString benchmarkName, double finalValue);
  void benchmarkComplete();
  void unitMeasureSignal(QString, int axis);
  void emptyBenchmarkQ();
  void benchmarkStarted(int benchmarkIdNumber);
  void testProgress();
public slots:
  void testProgressSlot();
  void setPrecision(bool p);
  bool getPrecision();
  void benchmarkStartedSlot(int benchmarkIdNumber);
  void executeSelectedBenchmark(QStringList benchmarkNamesList, bool precision);
  void finalResultSignalSlot(QString benchmarkName, double finalValue);
  void resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void benchmarkCompleteSlot();
  void unitMeasureSignalSlot(QString unitMeasureName, int axis);
  void startNextBenchmark();
  void createBenchmark(AbstractBenchmark *benchmark);
  void workerFinishedSlot();
};

#endif // BENCHMARK_CONTROLLER_H
