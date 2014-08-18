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
  mode = BENCHMARK_MODE_BASIC;
  BenchmarkSettings defaultSettings;
  currentBenchmark_Settings.setSettings(defaultSettings);
}

//Starts execution of a new benchmark test in a separate thread
void Benchmark_Controller::createBenchmark(AbstractBenchmark *benchmark){
  //  qDebug()<<"Creating Benchmark Thread Object";
  QThread *workerThread = new QThread();
  currentBenchmark_Thread = workerThread;
  benchmark->moveToThread(workerThread);

  connect(workerThread, SIGNAL(finished()), benchmark, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(finished()), this, SLOT(workerFinishedSlot()) );
  connect(workerThread, SIGNAL(started()), benchmark, SLOT(execute()) );

  if(mode == BENCHMARK_MODE_BASIC){
    connect(benchmark, SIGNAL(unitMeasureSignal(QString, int)), this, SLOT(unitMeasureSignalSlot(QString, int)) );
    connect(benchmark, SIGNAL(resultSignal(QString, double, double, int, int)),
            this, SLOT(resultSignalSlot(QString, double, double, int, int)) );
    connect(benchmark, SIGNAL(finalResultSignal(QString, double)),
            this, SLOT(finalResultSignalSlot(QString, double)) );
    connect(benchmark, SIGNAL(benchmarkStarted(int)), this, SLOT(benchmarkStartedSlot(int)) );
    connect(benchmark, SIGNAL(testProgress()), this, SLOT(testProgressSlot()) );

    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(benchmarkCompleteSlot()) );
  }
  else{//benchmark is set to expert with: BENCHMARK_MODE_EXPERT
    connect(benchmark, SIGNAL(unitMeasureSignal(QString, int)), this, SLOT(expert_unitMeasureSignalSlot(QString, int)) );
    connect(benchmark, SIGNAL(resultSignal(QString, double, double, int, int)),
            this, SLOT(expert_resultSignalSlot(QString, double, double, int, int)) );
    connect(benchmark, SIGNAL(finalResultSignal(QString, double)),
            this, SLOT(expert_finalResultSignalSlot(QString, double)) );
    connect(benchmark, SIGNAL(benchmarkStarted(int)), this, SLOT(expert_benchmarkStartedSlot(int)) );
    connect(benchmark, SIGNAL(testProgress()), this, SLOT(expert_testProgressSlot()) );

    connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(expert_benchmarkCompleteSlot()) );

  }
  connect(benchmark, SIGNAL(errorMessage(QString)), this, SLOT(errorMessageSlot(QString)) );
  connect(benchmark, SIGNAL(benchmarkComplete()), this, SLOT(startNextBenchmark()) );
  workerThread->start();

}

void Benchmark_Controller::errorMessageSlot(QString message){
  emit errorMessage(message);
}

void Benchmark_Controller::workerFinishedSlot(){
  //  qDebug()<<"worker thread finished";
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
  currentBenchmark_Thread->terminate();
  currentBenchmark_Thread->wait();
  //  currentBenchmarkThread->exit();
  //  currentBenchmarkThread->quit();
  benchmarkQ.clear();
  emit benchmarkStopped();
}

void Benchmark_Controller::benchmarkStartedSlot(int benchmarkIdNumber)
{
  emit benchmarkStarted(benchmarkIdNumber);
}

void Benchmark_Controller::startNextBenchmark(){
  if(!benchmarkQ.isEmpty()){
    QString nextBenchmarkName;
    nextBenchmarkName = benchmarkQ.dequeue();

    //    qDebug()<<"String Next benchmark: "<<nextBenchmarkName;
    if(nextBenchmarkName == "Blas3"){
      createBenchmark(new Benchmark_Blas3( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Copy"){
      createBenchmark(new Benchmark_Copy( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Scheduler"){
      //      createBenchmark(new Benchmark_Scheduler( getPrecision(), *currentBenchmarkSettings ));
    }
    else if(nextBenchmarkName == "Solver"){
      //      createBenchmark(new Benchmark_Solver( getPrecision(), *currentBenchmarkSettings ));
    }
    else if(nextBenchmarkName == "Sparse"){
      createBenchmark(new Benchmark_Sparse( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Vector"){
      createBenchmark(new Benchmark_Vector( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Qr"){
      //      createBenchmark(new Benchmark_Qr());
    }
    else{
      qDebug()<<"Error parsing benchmark name";
    }
  }
  else{
    //---ALL BENCHMARKS HAVE BEEN EXECUTED---//

    //        qDebug()<<"Benchmark queue is empty";
    processBenchmarkInstance(currentBenchmark_Instance);//save/upload benchmark results
    emit emptyBenchmarkQ(); //let ui know all benchmarks are complete
  }
}

void Benchmark_Controller::processBenchmarkInstance(BenchmarkInstance instance){
  Benchmark_Model model;
  model.processBenchmarkInstance(instance);
}

void Benchmark_Controller::setPrecision(bool p)
{
  this->precision = p;
}

bool Benchmark_Controller::getPrecision()
{
  return this->precision;
}

int Benchmark_Controller::getMode(){
  return this->mode;
}

void Benchmark_Controller::setMode(int m){
  this->mode = m;
}

void Benchmark_Controller::executeSelectedBenchmark(QStringList benchmarkNamesList,
                                                    BenchmarkSettings settings,
                                                    bool precision = DOUBLE_PRECISION,
                                                    int mode = BENCHMARK_MODE_BASIC)
{
  currentBenchmark_Settings.setSettings(settings);

  currentBenchmark_Instance.setSettings(currentBenchmark_Settings);
  if(mode == BENCHMARK_MODE_BASIC){
    currentBenchmark_Instance.mode = "Standard";
  }
  else{
    currentBenchmark_Instance.mode = "Advanced";
  }
  if(precision == DOUBLE_PRECISION){
    currentBenchmark_Instance.precision = "Double";
  }
  else{
    currentBenchmark_Instance.precision = "Single";
  }
  if(benchmarkNamesList.length() == 4){//all 4 benchmark have been selected
    currentBenchmark_Instance.full = true;
  }
  else{
    currentBenchmark_Instance.full = false;
  }

  setPrecision(precision);
  setMode(mode);

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
  //intercept final results and store them in the benchmark instance
  if(benchmarkName == "Blas3"){
    currentBenchmark_Instance.blas3Result = finalValue;
  }
  else if(benchmarkName == "Copy"){
    currentBenchmark_Instance.copyResult = finalValue;
  }
  else if(benchmarkName == "Sparse"){
    currentBenchmark_Instance.sparseResult = finalValue;
  }
  else if(benchmarkName == "Vector"){
    currentBenchmark_Instance.vectorResult = finalValue;
  }
  emit finalResultSignal(benchmarkName, finalValue);
}

void Benchmark_Controller::resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType, int testId)
{
  emit resultSignal(benchmarkName, key, resultValue, graphType, testId);
}

void Benchmark_Controller::benchmarkCompleteSlot()
{
  //  qDebug()<<"A benchmark has been completed;";
  emit benchmarkComplete();
}

void Benchmark_Controller::unitMeasureSignalSlot(QString unitMeasureName, int axis)
{
  emit unitMeasureSignal(unitMeasureName, axis);
}

void Benchmark_Controller::expert_testProgressSlot()
{
  emit expert_testProgress();
}

void Benchmark_Controller::expert_benchmarkStartedSlot(int benchmarkIdNumber)
{
  emit expert_benchmarkStarted(benchmarkIdNumber);
}

void Benchmark_Controller::expert_finalResultSignalSlot(QString benchmarkName, double finalValue)
{
  emit expert_finalResultSignal(benchmarkName, finalValue);
}

void Benchmark_Controller::expert_resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType, int testId)
{
  emit expert_resultSignal(benchmarkName, key, resultValue, graphType, testId);
}

void Benchmark_Controller::expert_benchmarkCompleteSlot()
{
  emit expert_benchmarkComplete();
}

void Benchmark_Controller::expert_unitMeasureSignalSlot(QString unitMeasureName, int axis)
{
  emit expert_unitMeasureSignal(unitMeasureName, axis);
}
