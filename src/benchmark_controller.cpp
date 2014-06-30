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

void Benchmark_Controller::executeSelectedBenchmark(QString benchmarkName)
{
  if(benchmarkName == "Blas3"){
    Benchmark_Blas3 *benchmark = new Benchmark_Blas3();
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
        benchmark->execute(); //execution is too slow for blas3
    delete benchmark;

  }
  else if(benchmarkName == "Copy"){
    Benchmark_Copy *benchmark = new Benchmark_Copy();
    benchmark->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(finished()), benchmark, SLOT(deleteLater()) );
    connect(&workerThread, SIGNAL(started()), benchmark, SLOT(execute()) );

    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );

    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(benchmarkComplete()), &workerThread, SLOT(terminate()) );

    workerThread.start();

//    benchmark->execute();
//    delete benchmark;

  }
  else if(benchmarkName == "Scheduler"){
    Benchmark_Scheduler *benchmark = new Benchmark_Scheduler();
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
    benchmark->execute();
    delete benchmark;

  }
  else if(benchmarkName == "Solver"){
    Benchmark_Solver *benchmark = new Benchmark_Solver();
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
    benchmark->execute();
    delete benchmark;

  }
  else if(benchmarkName == "Sparse"){
    Benchmark_Sparse *benchmark = new Benchmark_Sparse();
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
    benchmark->execute();
    delete benchmark;

  }
  else if(benchmarkName == "Vector"){
    Benchmark_Vector *benchmark = new Benchmark_Vector();
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
    benchmark->execute();
    delete benchmark;

  }
  else if(benchmarkName == "Qr"){
    Benchmark_Qr *benchmark = new Benchmark_Qr();
    connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
    connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
    benchmark->execute();
    delete benchmark;

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
