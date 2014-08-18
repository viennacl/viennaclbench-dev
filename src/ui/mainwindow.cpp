#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  jsString = "for (var index = 0, elems = document.body.getElementsByTagName('a'); index < elems.length; ++index) {"
      "if( elems[index].href.indexOf('.mat') != -1 ){"
      "elems[index].parentNode.removeChild(elems[index]);}"
      "else if(elems[index].href.indexOf('/RB/') != -1 ) {"
      "elems[index].parentNode.removeChild(elems[index]);}"
      "}";

  //normalize size of each main menu item
  //otherwise items would not be centered
  for ( int i = 0; i < ui->mainMenuListWidget->count(); i++ ) {
    ui->mainMenuListWidget->item(i)->setSizeHint(ui->mainMenuListWidget->itemSizeHint());
  }

  //get pointers to certain widgets of basic & expert benchmark widgets
  //accessing widget's ui widgets is considered bad design
  //should be done by using signals & slots
  basicBenchmarkListWidget = ui->basicBenchmark->benchmarkListWidget;
  basicStartBenchmarkButton = ui->basicBenchmark->startBenchmarkButton;
  basicStopBenchmarkButton = ui->basicBenchmark->stopBenchmarkButton;
  basicProgressBar = ui->basicBenchmark->progressBar;
  basicContextComboBox = ui->basicBenchmark->contextComboBox;
  basicSinglePrecisionButton = ui->basicBenchmark->singlePrecisionButton;

  expertBenchmarkListWidget = ui->expertBenchmark->benchmarkListWidget;
  expertStartBenchmarkButton = ui->expertBenchmark->startBenchmarkButton;
  expertStopBenchmarkButton = ui->expertBenchmark->stopBenchmarkButton;
  expertProgressBar = ui->expertBenchmark->progressBar;
  expertContextComboBox = ui->expertBenchmark->contextComboBox;
  expertSinglePrecisionButton = ui->expertBenchmark->singlePrecisionButton;

  connect(ui->actionAbout,SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
  connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()) );
  connect(ui->mainMenuListWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)) );
  ui->mainMenuListWidget->setCurrentRow(0);

  maximumBenchProgress = 0;
  currentBenchProgress = 0;

  //setup benchmark plots
  //HomeScreen class takes care of its own init
  //BasicBenchmark class takes care of its own init
  //ExpertBenchmark class takes care of its own init
  interconnectViews();
  initMatrixMarket();
  //SystemInfoScreen class takes care of its own init
  initPlatformDeviceChooser();

  //connect quickstart button
  connect(ui->homeScreen->quickStartButton, SIGNAL(clicked()), this, SLOT(quickstartFullBenchmark()) );
  //run benchmark button clicked -> execute benchmark

  //start basic/expert benchmark execution
  connect(basicStartBenchmarkButton, SIGNAL(clicked()), this, SLOT(startBasicBenchmarkExecution()) );
  connect(expertStartBenchmarkButton, SIGNAL(clicked()), this, SLOT(startExpertBenchmarkExecution()) );

  //stop benchmark button
  connect(basicStopBenchmarkButton, SIGNAL(clicked()), &benchmarkController, SLOT(stopExecution()) );
  connect(expertStopBenchmarkButton, SIGNAL(clicked()), &benchmarkController, SLOT(stopExecution()) );

  //hide stop button, show start button after the benchmark has been stopped
  connect(&benchmarkController, SIGNAL(benchmarkStopped()), ui->basicBenchmark, SLOT(hideStopButton()) );
  connect(&benchmarkController, SIGNAL(benchmarkStopped()), ui->expertBenchmark, SLOT(hideStopButton()) );

  //show the start button once all benchmarks are done
  connect(&benchmarkController, SIGNAL(emptyBenchmarkQ()), ui->basicBenchmark, SLOT(showBenchmarkStartButton()) );
  connect(&benchmarkController, SIGNAL(emptyBenchmarkQ()), ui->expertBenchmark, SLOT(showBenchmarkStartButton()) );

  //show error messages
  connect(&benchmarkController, SIGNAL(errorMessage(QString)), this, SLOT(showErrorMessageBox(QString)) );

  /* ---BASIC MODE CONNECTIONS--- */
  //route incoming benchmark result info to appropriate plots
  connect(&benchmarkController, SIGNAL(benchmarkStarted(int)), ui->basicBenchmark, SLOT(setActiveBenchmarkPlot(int)) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString, int)), ui->basicBenchmark, SLOT(updateBenchmarkUnitMeasure(QString, int)) );
  //received a benchmark test result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString, double, double, int, int)),
          ui->basicBenchmark, SLOT(parseBenchmarkResult(QString, double, double, int, int)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(finalResultSignal(QString, double)), ui->basicBenchmark, SLOT(updateFinalResultPlot(QString,double)) );
  //connect progress signals
  connect(&benchmarkController, SIGNAL(testProgress()), ui->basicBenchmark, SLOT(updateBenchProgress()) );

  /* ---EXPERT MODE CONNECTIONS--- */
  connect(&benchmarkController, SIGNAL(expert_benchmarkStarted(int)), ui->expertBenchmark, SLOT(setActiveBenchmarkPlot(int)) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(expert_unitMeasureSignal(QString, int)), ui->expertBenchmark, SLOT(updateBenchmarkUnitMeasure(QString, int)) );
  //received a benchmark test result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(expert_resultSignal(QString, double, double, int, int)),
          ui->expertBenchmark, SLOT(parseBenchmarkResult(QString, double, double, int, int)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(expert_finalResultSignal(QString, double)), ui->expertBenchmark, SLOT(updateFinalResultPlot(QString,double)) );
  //connect progress signals
  connect(&benchmarkController, SIGNAL(expert_testProgress()), ui->expertBenchmark, SLOT(updateBenchProgress()) );

}

void MainWindow::showErrorMessageBox(QString message){
  QMessageBox::warning(this, QString("ViennaCL Benchmark"), message);
}

void MainWindow::initPlatformDeviceChooser(){
  typedef std::vector< viennacl::ocl::platform > platforms_type;
  typedef std::vector< viennacl::ocl::device > devices_type;

  //Generate contexts

  //---PLATFORMS---
  long contextCounter = 0;
  platforms_type platforms = viennacl::ocl::get_platforms();
  for( size_t platformId = 0; platformId < platforms.size() ; ++platformId )
  {
    devices_type devices = platforms[platformId].devices(CL_DEVICE_TYPE_ALL);

    //---DEVICES---
    for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
    {
      //Setup contexts; one context per each device/platform combo
      viennacl::ocl::set_context_platform_index( contextCounter, platformId);
      viennacl::ocl::setup_context(contextCounter, *iter);

      //Construct a context info string to be shown in the UI
      QString contextInfo = "[" + QString::number(contextCounter) +
          "] " + QString::fromStdString(iter->name()) +
          " | " + QString::fromStdString(platforms[platformId].info());
      //Keep track of this context
      contextMap.insert(contextCounter, contextInfo);

      ++contextCounter;
    }
    //END---DEVICES---

  }
  //END---PLATFORMS---

  //Check all generated contexts and their devices
  for( long i = 0; i< contextCounter; i++){
    viennacl::ocl::switch_context( i );
    std::cout << "Context id: "<< i <<" Context value: " << viennacl::ocl::current_context().handle().get() << " Device name: "<<viennacl::ocl::current_device().name() << std::endl;
  }

  //Add contexts to the UI
  for (int i = 0; i <= contextMap.lastKey(); ++i) {
    basicContextComboBox->insertItem( i, contextMap.value(i) );
    expertContextComboBox->insertItem( i, contextMap.value(i) );
  }

  //  connect(ui->basic_platformsComboBox, SIGNAL(currentIndexChanged(int)), ui->expert_platformsComboBox, SLOT(setCurrentIndex(int)) );
  //  connect(ui->expert_platformsComboBox, SIGNAL(currentIndexChanged(int)), ui->basic_platformsComboBox, SLOT(setCurrentIndex(int)) );
  //  connect(ui->basic_contextComboBox, SIGNAL(activated(int)), this, SLOT(switchContext(int)) );
  //  connect(ui->expert_contextComboBox, SIGNAL(activated(int)), this, SLOT(switchContext(int)) );


}

//switches viennacl to selected context and displays its devices
void MainWindow::switchContext(int contextNumber){
  viennacl::ocl::switch_context((long)contextNumber);
  std::cout << "Context id: "<< contextNumber <<" Context value: " << viennacl::ocl::current_context().handle().get() << std::endl;
  std::cout << "Platform_index: "<< viennacl::ocl::current_context().platform_index()<< std::endl;
  std::cout << "Device name: "<< viennacl::ocl::current_context().current_device().name()<< std::endl;
}

//starts a full basic benchmark
void MainWindow::quickstartFullBenchmark(){
  ui->mainMenuListWidget->setCurrentRow(1);//switch to benchmark tab
  ui->benchmarkPageTabWidget->setCurrentIndex(0);//switch to basic benchmark tab
  basicBenchmarkListWidget->selectAllItems();//select all benchmarks
  startBasicBenchmarkExecution();
}

void MainWindow::initMatrixMarket(){
  //enable cache
  QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled, true);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);//enable inspect element
  ui->matrixMarket_Widget->webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
  ui->matrixMarket_Widget->webView->settings()->enablePersistentStorage(ArchiveExtractor::getMatrixMarketUserFolder());//save MatrixMarket web cache in its user folder
  ui->matrixMarket_Widget->webView->settings()->setMaximumPagesInCache(10);
  //web page with all matrices contains around 2700 matrices...
  //needs MOAR cache
  ui->matrixMarket_Widget->webView->settings()->setOfflineWebApplicationCacheQuota(22111000);
  ui->matrixMarket_Widget->webView->load(QUrl("qrc:///mmFiles/matrixmarket/index.html"));
  //    ui->matrixMarket_Widget->webView->load(QUrl("http://localhost/MatrixMarket/index.html"));//I connect to my local WAMP server for easier html editing

//  connect(ui->matrixMarket_Widget->webView, SIGNAL(loadProgress(int)), this, SLOT(modifyMatrixMarketWeb()) );
  connect(ui->matrixMarket_Widget->webView, SIGNAL(fileReadyForBenchmark(QString)), this, SLOT( startMatrixMarketBenchmark(QString) ) );
}

void MainWindow::startMatrixMarketBenchmark(QString filename){
  //todo
}

void MainWindow::modifyMatrixMarketWeb(){
  //  qDebug()<<"---HTML---"<<ui->matrixMarket_Widget->webView->page()->mainFrame()->toHtml();
  //  qDebug()<<"load finished";
  //  if (ui->matrixMarket_Widget->webView->page()->settings()->testAttribute( QWebSettings::JavascriptEnabled ) ){
  //    qDebug()<<"js enabled";
  //  }
  //  ui->matrixMarket_Widget->webView->page()->mainFrame()->evaluateJavaScript(jsString);
}

void MainWindow::interconnectViews(){
  //  connect(ui->basic_BenchmarkListWidget, SIGNAL()

}

bool MainWindow::getBasicPrecision(){
  if(basicSinglePrecisionButton->isChecked()){
    return SINGLE_PRECISION;
  }
  else{
    return DOUBLE_PRECISION;
  }
}

bool MainWindow::getExpertPrecision(){
  if(expertSinglePrecisionButton->isChecked()){
    return SINGLE_PRECISION;
  }
  else{
    return DOUBLE_PRECISION;
  }
}

void MainWindow::startExpertBenchmarkExecution(){
  ui->expertBenchmark->resetAllPlots();
  currentBenchProgress = 0;
  maximumBenchProgress = 0;
  expertProgressBar->setValue(currentBenchProgress);
  expertProgressBar->setFormat("Starting Benchmark...");
  QStringList selectedBenchmarkItems;
  for ( int i = 1; i < expertBenchmarkListWidget->count(); i++ ) {
    if(expertBenchmarkListWidget->item(i)->isSelected() ){
      selectedBenchmarkItems.append(expertBenchmarkListWidget->item(i)->text());
      //add each selected benchmark's number of tests to maximumBenchProgress
      switch(i){
      case 1: maximumBenchProgress += 3; break;//blas3 3 tests
      case 2: maximumBenchProgress += 6; break;//copy 6 tests
      case 3: maximumBenchProgress += 6; break;//sparse 6 tests
      case 4: maximumBenchProgress += 10; break;//vector 10 tests
      default: break;
      }
    }
  }
  if(selectedBenchmarkItems.isEmpty()){
    //no benchmarks were selected, return
    expertProgressBar->setFormat("No benchmarks were selected");
    return;
  }
  //  qDebug()<<"Selected benchmarks: "<<selectedBenchmarkItems;

  //set progress bar max value
  expertProgressBar->setMaximum(maximumBenchProgress);

  expertStopBenchmarkButton->show();
  expertStartBenchmarkButton->hide();

  switchContext( expertContextComboBox->currentIndex() );//switch to currently selected context
  BenchmarkSettings customSettings;
  customSettings.setSettings(ui->expertBenchmark->getExpertSettings());
  benchmarkController.executeSelectedBenchmark( selectedBenchmarkItems, customSettings, getExpertPrecision(), BENCHMARK_MODE_EXPERT );//start the benchmark
}

//execute the currently selected benchmark
void MainWindow::startBasicBenchmarkExecution(){
  ui->basicBenchmark->resetAllPlots();
  currentBenchProgress = 0;
  maximumBenchProgress = 0;
  basicProgressBar->setValue(currentBenchProgress);
  basicProgressBar->setFormat("Starting Benchmark...");
  QStringList selectedBenchmarkItems;
  for ( int i = 1; i < basicBenchmarkListWidget->count(); i++ ) {
    if(basicBenchmarkListWidget->item(i)->isSelected() ){
      selectedBenchmarkItems.append(basicBenchmarkListWidget->item(i)->text());
      //add each selected benchmark's number of tests to maximumBenchProgress
      switch(i){
      case 1: maximumBenchProgress += 4; break;//blas3 4 tests
      case 2: maximumBenchProgress += 6; break;//copy 6 tests
      case 3: maximumBenchProgress += 6; break;//sparse 6 tests
      case 4: maximumBenchProgress += 10; break;//vector 10 tests
      default: break;
      }
    }
  }
  if(selectedBenchmarkItems.isEmpty()){
    //no benchmarks were selected, return
    basicProgressBar->setFormat("No benchmarks were selected");
    return;
  }
  //  qDebug()<<"Selected benchmarks: "<<selectedBenchmarkItems;

  //set progress bar max value
  basicProgressBar->setMaximum(maximumBenchProgress);

  basicStopBenchmarkButton->show();
  basicStartBenchmarkButton->hide();

  switchContext( basicContextComboBox->currentIndex() );//switch to currently selected context
  BenchmarkSettings defaultSettings;
  benchmarkController.executeSelectedBenchmark( selectedBenchmarkItems, defaultSettings, getBasicPrecision(), BENCHMARK_MODE_BASIC );//start the benchmark
}

MainWindow::~MainWindow()
{
  delete ui;
}

