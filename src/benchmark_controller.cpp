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

//Starts execution of a new benchmark test in a separate thread
void Benchmark_Controller::createBenchmark(AbstractBenchmark *benchmark){
  qDebug()<<"Creating Benchmark Object";
  QThread *workerThread = new QThread();
  benchmark->moveToThread(workerThread);
  connect(workerThread, SIGNAL(finished()), benchmark, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(started()), benchmark, SLOT(execute()) );

  connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
  connect(benchmark, SIGNAL(resultSignal(QString,double)), this, SLOT(resultSignalSlot(QString,double)) );
  connect(benchmark, SIGNAL(finalResultSignal(QString, double)), this, SLOT(finalResultSignalSlot(QString, double)) );
  connect(benchmark, SIGNAL(benchmarkStarted(int)), this, SLOT(benchmarkStartedSlot(int)) );

  connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
  connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(startNextBenchmark()) );
//  connect(benchmark, SIGNAL(benchmarkComplete()), workerThread, SLOT(terminate()) );

  workerThread->start();

}

void Benchmark_Controller::enqueueBenchmarks(QStringList benchmarkNamesList){
  if(!benchmarkNamesList.isEmpty()){
    foreach(QString benchmark, benchmarkNamesList){
      benchmarkQ.enqueue(benchmark);
    }
  }
}

void Benchmark_Controller::benchmarkStartedSlot(int benchmarkIdNumber)
{
  emit benchmarkStarted(benchmarkIdNumber);
}

void Benchmark_Controller::startNextBenchmark(){
  if(!benchmarkQ.isEmpty()){
    QString nextBenchmarkName;
    nextBenchmarkName = benchmarkQ.dequeue();

    qDebug()<<"String Next benchmark: "<<nextBenchmarkName;
    if(nextBenchmarkName == "Blas3"){
      //    Benchmark_Blas3 *benchmark = new Benchmark_Blas3();
      createBenchmark(new Benchmark_Blas3());
    }
    else if(nextBenchmarkName == "Copy"){
      //    Benchmark_Copy *benchmark = new Benchmark_Copy();
      createBenchmark(new Benchmark_Copy());
    }
    else if(nextBenchmarkName == "Scheduler"){
      //    Benchmark_Scheduler *benchmark = new Benchmark_Scheduler();
      createBenchmark(new Benchmark_Scheduler());
    }
    else if(nextBenchmarkName == "Solver"){
      //    Benchmark_Solver *benchmark = new Benchmark_Solver();
      createBenchmark(new Benchmark_Solver());
    }
    else if(nextBenchmarkName == "Sparse"){
      //    Benchmark_Sparse *benchmark = new Benchmark_Sparse();
      createBenchmark(new Benchmark_Sparse());
    }
    else if(nextBenchmarkName == "Vector"){
      //    Benchmark_Vector *benchmark = new Benchmark_Vector();
      createBenchmark(new Benchmark_Vector());
    }
    else if(nextBenchmarkName == "Qr"){
      //    Benchmark_Qr *benchmark = new Benchmark_Qr();
      createBenchmark(new Benchmark_Qr());
    }
    else{
      qDebug()<<"Error parsing benchmark name";
    }
  }
  else{
    qDebug()<<"Benchmark queue is empty";
    emit emptyBenchmarkQ();
  }
}

void Benchmark_Controller::executeSelectedBenchmark(QStringList benchmarkNamesList)
{
  if(!benchmarkNamesList.isEmpty()){
    enqueueBenchmarks(benchmarkNamesList);
    startNextBenchmark();
  }
  else{
    qDebug()<<"No benchmarks were selected for execution";
  }
}

void Benchmark_Controller::finalResultSignalSlot(QString benchmarkName, double finalValue)
{
  emit finalResultSignal(benchmarkName, finalValue);
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
