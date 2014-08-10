#include "benchmark_controller.h"
#include "QDebug"
/*
 * Central benchmark execution controller
 * Basically acts as a central connection hub between the UI and benchmarks
 * */

Benchmark_Controller::Benchmark_Controller(QObject *parent) :
  QObject(parent)
{
  precision = DOUBLE_PRECISION;
}

//Starts execution of a new benchmark test in a separate thread
void Benchmark_Controller::createBenchmark(AbstractBenchmark *benchmark){
  qDebug()<<"Creating Benchmark Thread Object";
  QThread *workerThread = new QThread();
  currentBenchmarkThread = workerThread;
  benchmark->moveToThread(workerThread);

  connect(workerThread, SIGNAL(finished()), benchmark, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(finished()), this, SLOT(workerFinishedSlot()) );
  connect(workerThread, SIGNAL(started()), benchmark, SLOT(execute()) );

  connect(benchmark, SIGNAL(unitMeasureSignal(QString)), this, SLOT(unitMeasureSignalSlot(QString)) );
  connect(benchmark, SIGNAL(resultSignal(QString, double, double, int)), this, SLOT(resultSignalSlot(QString, double, double, int)) );
  connect(benchmark, SIGNAL(finalResultSignal(QString, double)), this, SLOT(finalResultSignalSlot(QString, double)) );
  connect(benchmark, SIGNAL(benchmarkStarted(int)), this, SLOT(benchmarkStartedSlot(int)) );
  connect(benchmark, SIGNAL(testProgress()), this, SLOT(testProgressSlot()) );

  connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
  connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(startNextBenchmark()) );
//  connect(benchmark, SIGNAL(benchmarkComplete()), workerThread, SLOT(terminate()) );

  workerThread->start();

}

void Benchmark_Controller::workerFinishedSlot(){
  qDebug()<<"worker thread finished";
}

void Benchmark_Controller::enqueueBenchmarks(QStringList benchmarkNamesList){
  if(!benchmarkNamesList.isEmpty()){
    foreach(QString benchmark, benchmarkNamesList){
      benchmarkQ.enqueue(benchmark);
    }
  }
}

void Benchmark_Controller::testProgressSlot()
{
  emit testProgress();
}

void Benchmark_Controller::stopExecution()
{
  qDebug()<<"---STOPPING BENCHMARK EXECUTION---";
  currentBenchmarkThread->terminate();
  currentBenchmarkThread->wait();
//  currentBenchmarkThread->exit();
//  currentBenchmarkThread->quit();
  benchmarkQ.clear();

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
      createBenchmark(new Benchmark_Blas3( getPrecision() ));
    }
    else if(nextBenchmarkName == "Copy"){
      createBenchmark(new Benchmark_Copy( getPrecision() ));
    }
    else if(nextBenchmarkName == "Scheduler"){
//      createBenchmark(new Benchmark_Scheduler( getPrecision() ));
    }
    else if(nextBenchmarkName == "Solver"){
//      createBenchmark(new Benchmark_Solver( getPrecision() ));
    }
    else if(nextBenchmarkName == "Sparse"){
      createBenchmark(new Benchmark_Sparse( getPrecision() ));
    }
    else if(nextBenchmarkName == "Vector"){
      createBenchmark(new Benchmark_Vector( getPrecision() ));
    }
    else if(nextBenchmarkName == "Qr"){
//      createBenchmark(new Benchmark_Qr());
    }
    else{
      qDebug()<<"Error parsing benchmark name";
    }
  }
  else{
    qDebug()<<"Benchmark queue is empty";
    emit emptyBenchmarkQ(); //no more benchmarks to run
  }
}

void Benchmark_Controller::setPrecision(bool p)
{
  this->precision = p;
}

bool Benchmark_Controller::getPrecision()
{
  return this->precision;
}

void Benchmark_Controller::executeSelectedBenchmark(QStringList benchmarkNamesList, bool precision)
{
  setPrecision(precision);
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

void Benchmark_Controller::resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType)
{
  emit resultSignal(benchmarkName, key, resultValue, graphType);
}

void Benchmark_Controller::benchmarkCompleteSlot()
{
  qDebug()<<"A benchmark has been completed;";
  emit benchmarkComplete();
}

void Benchmark_Controller::unitMeasureSignalSlot(QString unitMeasureName)
{
  emit unitMeasureSignal(unitMeasureName);
}
