#include "benchmark_controller.h"
#include "QDebug"
/*
 * Central benchmark execution controller
 * Basically acts as a central connection hub between the UI and benchmarks
 * */

Benchmark_Controller::Benchmark_Controller(QObject *parent) :
  QObject(parent)
{
}

void Benchmark_Controller::createBenchmark(AbstractBenchmark *benchmark){
  benchmark->moveToThread(&workerThread);
  connect(&workerThread, SIGNAL(finished()), benchmark, SLOT(deleteLater()) );
  connect(&workerThread, SIGNAL(started()), benchmark, SLOT(execute()) );

  connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
  connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );

  connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
  connect(benchmark, SIGNAL(benchmarkComplete()), &workerThread, SLOT(terminate()) );

  workerThread.start();

}

void Benchmark_Controller::executeSelectedBenchmark(QString benchmarkName)
{
  if(benchmarkName == "Blas3"){
//    Benchmark_Blas3 *benchmark = new Benchmark_Blas3();
    createBenchmark(new Benchmark_Blas3());
  }
  else if(benchmarkName == "Copy"){
//    Benchmark_Copy *benchmark = new Benchmark_Copy();
    createBenchmark(new Benchmark_Copy());
  }
  else if(benchmarkName == "Scheduler"){
//    Benchmark_Scheduler *benchmark = new Benchmark_Scheduler();
    createBenchmark(new Benchmark_Scheduler());
  }
  else if(benchmarkName == "Solver"){
//    Benchmark_Solver *benchmark = new Benchmark_Solver();
    createBenchmark(new Benchmark_Solver());
  }
  else if(benchmarkName == "Sparse"){
//    Benchmark_Sparse *benchmark = new Benchmark_Sparse();
    createBenchmark(new Benchmark_Sparse());
  }
  else if(benchmarkName == "Vector"){
//    Benchmark_Vector *benchmark = new Benchmark_Vector();
    createBenchmark(new Benchmark_Vector());
  }
  else if(benchmarkName == "Qr"){
//    Benchmark_Qr *benchmark = new Benchmark_Qr();
    createBenchmark(new Benchmark_Qr());
  }
  else{
    qDebug()<<"cannot resolve benchmark name";
  }
}

void Benchmark_Controller::resultSignalSlot(QString benchmarkName, double bandwidthValue)
{
  emit resultSignal(benchmarkName, bandwidthValue);
}

void Benchmark_Controller::benchmarkCompleteSlot()
{
  emit benchmarkComplete();
}

void Benchmark_Controller::unitMeasureSignalSlot(QString unitMeasureName)
{
  emit unitMeasureSignal(unitMeasureName);
}
