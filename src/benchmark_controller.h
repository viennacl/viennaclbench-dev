#ifndef BENCHMARK_CONTROLLER_H
#define BENCHMARK_CONTROLLER_H

#include <QObject>
#include <QString>

#include "benchmarks/benchmark_copy.h"
#include "benchmarks/benchmark_scheduler.h"
#include "benchmarks/benchmark_solver.h"
#include "benchmarks/benchmark_vector.h"
#include "benchmarks/benchmark_sparse.h"
#include "benchmarks/benchmark_blas3.h"
#include "benchmarks/benchmark_qr.h"

class Benchmark_Controller : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Controller(QObject *parent = 0);
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();

public slots:
  void executeSelectedBenchmark(QString benchmarkName);
  void resultSignalSlot(QString benchmarkName, double bandwidthValue);
  void benchmarkCompleteSlot();

};

#endif // BENCHMARK_CONTROLLER_H
