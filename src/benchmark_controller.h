#ifndef BENCHMARK_CONTROLLER_H
#define BENCHMARK_CONTROLLER_H
/*
 * Main benchmark execution controller
 * Acts as a central communication hub between the UI and benchmarks
 * Passes on relevant signals from active benchmarks to the UI
 * */
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
#include "benchmarkinstance.h"
#include "benchmark_model.h"


/*!
 * \class Benchmark_Controller
 * \brief Main controller. Handles benchmark execution; communicates benchmark data from/to the UI;, orders result saving/uploading.
 * Refer to the mainpage (\ref mvc-backbone  ) for a brief overview of how the execution flow works.
 * Some of the slots/signals in this class are only used to pass on the results from a running benchmark to the UI.
 * The "expert_" prefix is used to mark the signals that are used in expert mode.
 */
class Benchmark_Controller : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Controller(QObject *parent = 0);
private:
  QQueue<QString> benchmarkQ; ///< The main benchmark queue.
  void enqueueBenchmarks(QStringList benchmarkNames);
  bool precision; ///<  Precision in which to run benchmark; false(0) - SINGLE | true(1) - DOUBLE
  QThread *currentBenchmark_Thread; ///< The thread in which the current benchmark is running.
  BenchmarkSettings currentBenchmark_Settings; ///< Holds settings for the current benchmark session.
  BenchmarkInstance currentBenchmark_Instance; ///< Holds results & other info of the current benchmark session.
  int mode; ///< The benchmark mode (basic/expert)
signals:
  void emptyBenchmarkQ(); ///< Emitted when all queued benchmarks have finished execution
  void benchmarkStopped(); ///< Emitted when a benchmark was stopped
  void errorMessage(QString message); ///< Same as \ref AbstractBenchmark::errorMessage()

  //basic mode signals
  void benchmarkComplete(); ///< Same as \ref AbstractBenchmark::benchmarkComplete()
  void benchmarkStarted(int benchmarkIdNumber); ///< Same as \ref AbstractBenchmark::benchmarkStarted()
  void unitMeasureSignal(QString, int axis); ///< Same as \ref AbstractBenchmark::unitMeasureSignal()
  void testProgress(); ///< Same as \ref AbstractBenchmark::testProgress()
  void resultSignal(QString benchmarkName, double key, double resultValue, int graphType, int testId); ///< Same as \ref AbstractBenchmark::resultSignal()
  void finalResultSignal(QString benchmarkName, double finalValue); ///< Same as \ref AbstractBenchmark::finalResultSignal()

  //expert mode signals
  void expert_benchmarkComplete(); ///< Same as \ref AbstractBenchmark::benchmarkComplete()
  void expert_benchmarkStarted(int benchmarkIdNumber); ///< Same as \ref AbstractBenchmark::benchmarkStarted()
  void expert_unitMeasureSignal(QString, int axis); ///< Same as \ref AbstractBenchmark::unitMeasureSignal()
  void expert_testProgress(); ///< Same as \ref AbstractBenchmark::testProgress()
  void expert_resultSignal(QString benchmarkName, double key, double resultValue, int graphType, int testId); ///< Same as \ref AbstractBenchmark::resultSignal()
  void expert_finalResultSignal(QString benchmarkName, double finalValue); ///< Same as \ref AbstractBenchmark::finalResultSignal()

public slots:
  void stopExecution();
  void setPrecision(bool p);
  bool getPrecision();
  void executeSelectedBenchmarks(QStringList benchmarkNamesList, BenchmarkSettings settings, bool precision, int mode);
  void startNextBenchmark();
  void setMode(int m);
  int getMode();
  void startBenchmarkThread(AbstractBenchmark *benchmark);
  void processBenchmarkInstance(BenchmarkInstance instance);
  void errorMessageSlot(QString message);

  //basic mode slots
  void testProgressSlot();
  void benchmarkStartedSlot(int benchmarkIdNumber);
  void finalResultSignalSlot(QString benchmarkName, double finalValue);
  void resultSignalSlot(QString testName, double key, double resultValue, int graphType, int testId);
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
