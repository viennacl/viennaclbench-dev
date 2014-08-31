#include "benchmark_controller.h"
#include "QDebug"

/*!
 * \brief Default constructor.
 * Loads default settings on creation: \ref BenchmarkSettings, \ref DOUBLE_PRECISION and \ref BENCHMARK_MODE_BASIC
 * \param parent Optional parent object.
 */
Benchmark_Controller::Benchmark_Controller(QObject *parent) :
  QObject(parent)
{
  precision = DOUBLE_PRECISION;
  mode = BENCHMARK_MODE_BASIC;
  BenchmarkSettings defaultSettings;
  currentBenchmark_Settings.setSettings(defaultSettings);
}

/*!
 * \brief Creates a new thread, assigns the benchmark to it, and starts the benchmark execution.
 * This function is responsible for starting benchmarks in threads and communicating its signals to the controller.
 * A benchmark's completion orders its thread to self-destruct after the benchmark itself was destroyed.
 * \param benchmark The benchmark to be started in a new thread.
 */
void Benchmark_Controller::startBenchmarkThread(AbstractBenchmark *benchmark){
  //  qDebug()<<"Creating Benchmark Thread Object";
  QThread *workerThread = new QThread();
  currentBenchmark_Thread = workerThread;
  benchmark->moveToThread(workerThread);

  connect(workerThread, SIGNAL(finished()), benchmark, SLOT(deleteLater()) );
  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()) );
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

/*!
 * \brief Fills the benchmark queue with names of selected benchmarks
 * \param benchmarkNamesList The list of benchmarks to be enqueued
 */
void Benchmark_Controller::enqueueBenchmarks(QStringList benchmarkNamesList){
  if(!benchmarkNamesList.isEmpty()){
    foreach(QString benchmark, benchmarkNamesList){
      benchmarkQ.enqueue(benchmark);
    }
  }
}

/*!
 * \brief Stops the current benchmark. Clears the benchmark queue. Emits benchmarkStopped() signal when its done.
 */
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

/*!
 * \brief Starts the next queued benchmark. Emits \ref emptyBenchmarkQ() signal if there are no more queued benchmarks.
 */
void Benchmark_Controller::startNextBenchmark(){
  if(!benchmarkQ.isEmpty()){
    QString nextBenchmarkName;
    nextBenchmarkName = benchmarkQ.dequeue();

    //    qDebug()<<"String Next benchmark: "<<nextBenchmarkName;
    if(nextBenchmarkName == "Blas3"){
      startBenchmarkThread(new Benchmark_Blas3( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Copy"){
      startBenchmarkThread(new Benchmark_Copy( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Scheduler"){
      //      createBenchmark(new Benchmark_Scheduler( getPrecision(), *currentBenchmarkSettings ));
    }
    else if(nextBenchmarkName == "Solver"){
      //      createBenchmark(new Benchmark_Solver( getPrecision(), *currentBenchmarkSettings ));
    }
    else if(nextBenchmarkName == "Sparse"){
      startBenchmarkThread(new Benchmark_Sparse( getPrecision(), currentBenchmark_Settings ));
    }
    else if(nextBenchmarkName == "Vector"){
      startBenchmarkThread(new Benchmark_Vector( getPrecision(), currentBenchmark_Settings ));
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

/*!
 * \brief This function sets up everything needed before a benchmark session can start and then issues an order to start execution.
 * \param benchmarkNamesList List of selected benchmark names
 * \param settings Settings to be used when running the benchmarks
 * \param precision Benchmark precision
 * \param mode Benchmark mode
 */
void Benchmark_Controller::executeSelectedBenchmarks(QStringList benchmarkNamesList,
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

/*!
 * \brief Calls on the model to process and save the benchmark results.
 * \param instance Instance object that holds all the result info.
 */
void Benchmark_Controller::processBenchmarkInstance(BenchmarkInstance instance){
  Benchmark_Model model;
  model.processBenchmarkInstance(instance);
}

/*!
 * \brief Passes on the error message from \ref AbstractBenchmark::errorMessage()
 * \param message The error message
 */
void Benchmark_Controller::errorMessageSlot(QString message){
  emit errorMessage(message);
}

/*!
 * \brief Passes on the signal from \ref AbstractBenchmark::testProgress()
 */
void Benchmark_Controller::testProgressSlot()
{
  emit testProgress();
}

/*!
 * \brief Passes on the benchmark ID number from \ref AbstractBenchmark::benchmarkStarted()
 * \param benchmarkIdNumber The benchmark ID number
 */
void Benchmark_Controller::benchmarkStartedSlot(int benchmarkIdNumber)
{
  emit benchmarkStarted(benchmarkIdNumber);
}

/*!
 * \brief Adds the finished benchmark result to the current instance and passes on the result from \ref AbstractBenchmark::finalResultSignal()
 * \param benchmarkName Name of the finished benchmark
 * \param finalValue Final result of the finished benchmark
 */
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

/*!
 * \brief Passes on test results from \ref AbstractBenchmark::resultSignal()
 * \param testName The name of the completed test (e.g. ""Matrix-Matrix product")
 * \param key The key on x/y axis for which the result is to be displayed
 * \param value The test result value
 * \param graphType Which graph type is to be used when showing the result see \see GraphType ref \ref GraphType
 * \param testId Used to identify the test result so that it can receive "special" graphing treatment (different graph color)
 */
void Benchmark_Controller::resultSignalSlot(QString testName, double key, double resultValue, int graphType, int testId)
{
  emit resultSignal(testName, key, resultValue, graphType, testId);
}

/*!
 * \brief Passes on the signal from \ref AbstractBenchmark::benchmarkComplete()
 */
void Benchmark_Controller::benchmarkCompleteSlot()
{
  //  qDebug()<<"A benchmark has been completed;";
  emit benchmarkComplete();
}

/*!
 * \brief Passes on the measure from \ref AbstractBenchmark::unitMeasureSignal()
 * \param unitMeasureName The name of the measure (e.g. "GB/s")
 * \param axis The axis on which to show the measure (Qt::XAxis/Qt::YAxis)
 */
void Benchmark_Controller::unitMeasureSignalSlot(QString unitMeasureName, int axis)
{
  emit unitMeasureSignal(unitMeasureName, axis);
}

/*!
 * \brief Expert mode version of \ref Benchmark_Controller::testProgressSlot()
 */
void Benchmark_Controller::expert_testProgressSlot()
{
  emit expert_testProgress();
}

/*!
 * \brief Expert mode version of \ref Benchmark_Controller::benchmarkStartedSlot()
 * \param benchmarkIdNumber The benchmark ID number
 */
void Benchmark_Controller::expert_benchmarkStartedSlot(int benchmarkIdNumber)
{
  emit expert_benchmarkStarted(benchmarkIdNumber);
}

/*!
 * \brief Expert mode version of \ref Benchmark_Controller::finalResultSignalSlot()
 * \param benchmarkName Name of the finished benchmark
 * \param finalValue Final result of the finished benchmark
 */
void Benchmark_Controller::expert_finalResultSignalSlot(QString benchmarkName, double finalValue)
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
  emit expert_finalResultSignal(benchmarkName, finalValue);
}

/*!
 * \brief Expert mode version of \ref Benchmark_Controller::resultSignalSlot()
 * \param testName The name of the completed test (e.g. ""Matrix-Matrix product")
 * \param key The key on x/y axis for which the result is to be displayed
 * \param value The test result value
 * \param graphType Which graph type is to be used when showing the result see \see GraphType ref \ref GraphType
 * \param testId Used to identify the test result so that it can receive "special" graphing treatment (different graph color)
 */
void Benchmark_Controller::expert_resultSignalSlot(QString benchmarkName, double key, double resultValue, int graphType, int testId)
{
  emit expert_resultSignal(benchmarkName, key, resultValue, graphType, testId);
}

/*!
 * \brief Expert mode version of \ref Benchmark_Controller::benchmarkCompleteSlot()
 */
void Benchmark_Controller::expert_benchmarkCompleteSlot()
{
  emit expert_benchmarkComplete();
}

/*!
 * \brief Expert mode version of \ref Benchmark_Controller::unitMeasureSignalSlot()
 * \param unitMeasureName The name of the measure (e.g. "GB/s")
 * \param axis The axis on which to show the measure (Qt::XAxis/Qt::YAxis)
 */
void Benchmark_Controller::expert_unitMeasureSignalSlot(QString unitMeasureName, int axis)
{
  emit expert_unitMeasureSignal(unitMeasureName, axis);
}

/*!
 * \brief Set the benchmark precision
 * \param p Precision to be set
 */
void Benchmark_Controller::setPrecision(bool p)
{
  this->precision = p;
}

/*!
 * \brief Currently set precision
 * \return Currently set precision
 */
bool Benchmark_Controller::getPrecision()
{
  return this->precision;
}

/*!
 * \brief Currently selected benchmark mode
 * \return Currently selected benchmark mode
 */
int Benchmark_Controller::getMode(){
  return this->mode;
}

/*!
 * \brief Changes the currently selected benchmark mode
 * \param m Benchmark mode to be set
 */
void Benchmark_Controller::setMode(int m){
  this->mode = m;
}
