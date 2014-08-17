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
//#include "benchmarks/benchmark_scheduler.h"
//#include "benchmarks/benchmark_solver.h"
//#include "benchmarks/benchmark_qr.h"
#include "benchmarksettings.h"

class Benchmark_Controller : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Controller(QObject *parent = 0);
private:
  QQueue<QString> benchmarkQ;
  void enqueueBenchmarks(QStringList benchmarkNames);
  bool precision; // false(0) - SINGLE | true(1) - DOUBLE
  QThread *currentBenchmarkThread;
  BenchmarkSettings currentBenchmarkSettings;
  int mode;
signals:
  void emptyBenchmarkQ();
  void benchmarkStopped();

  //basic mode signals
  void benchmarkComplete();
  void benchmarkStarted(int benchmarkIdNumber);
  void unitMeasureSignal(QString, int axis);
  void testProgress();
  void resultSignal(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void finalResultSignal(QString benchmarkName, double finalValue);

  //expert mode signals
  void expert_benchmarkComplete();
  void expert_benchmarkStarted(int benchmarkIdNumber);
  void expert_unitMeasureSignal(QString, int axis);
  void expert_testProgress();
  void expert_resultSignal(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void expert_finalResultSignal(QString benchmarkName, double finalValue);
public slots:
  void stopExecution();
  void setPrecision(bool p);
  bool getPrecision();
  void executeSelectedBenchmark(QStringList benchmarkNamesList, BenchmarkSettings settings, bool precision, int mode);
  void startNextBenchmark();
  void workerFinishedSlot();
  void setMode(int m);
  int getMode();
  void createBenchmark(AbstractBenchmark *benchmark);

  //basic mode slots
  void testProgressSlot();
  void benchmarkStartedSlot(int benchmarkIdNumber);
  void finalResultSignalSlot(QString benchmarkName, double finalValue);
  void resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void benchmarkCompleteSlot();
  void unitMeasureSignalSlot(QString unitMeasureName, int axis);

  //expert mode slots
  void expert_testProgressSlot();
  void expert_benchmarkStartedSlot(int benchmarkIdNumber);
  void expert_finalResultSignalSlot(QString benchmarkName, double finalValue);
  void expert_resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void expert_benchmarkCompleteSlot();
  void expert_unitMeasureSignalSlot(QString unitMeasureName, int axis);
};

#endif // BENCHMARK_CONTROLLER_H
