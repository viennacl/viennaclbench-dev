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

  //normalize size of each list menu item
  for ( int i = 0; i < ui->mainMenuListWidget->count(); i++ ) {
    ui->mainMenuListWidget->item(i)->setSizeHint(ui->mainMenuListWidget->itemSizeHint());
  }

  basicBenchmarkListWidget = ui->basicBenchmarkWidget->benchmarkListWidget;
  basicStartBenchmarkButton = ui->basicBenchmarkWidget->startBenchmarkButton;
  basicStopBenchmarkButton = ui->basicBenchmarkWidget->stopBenchmarkButton;
  basicProgressBar = ui->basicBenchmarkWidget->progressBar;
  basicContextComboBox = ui->basicBenchmarkWidget->contextComboBox;
  basicSinglePrecisionButton = ui->basicBenchmarkWidget->singlePrecisionButton;

  connect(ui->actionAbout,SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
  connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()) );
  connect(ui->mainMenuListWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)) );
  ui->mainMenuListWidget->setCurrentRow(0);

  maximumBenchProgress = 0;
  currentBenchProgress = 0;

  //setup benchmark plots
  //HomeScreen class takes care of its own init
  //BasicBenchmark class takes care of its own init
  initExpert();
  interconnectViews();
  initMatrixMarket();
  //SystemInfoScreen class takes care of its own init
  initPlatformDeviceChooser();

  //connect quickstart button
  connect(ui->homeScreen->quickStartButton, SIGNAL(clicked()), this, SLOT(quickstartFullBenchmark()) );
  //run benchmark button clicked -> execute benchmark

  connect(basicStartBenchmarkButton, SIGNAL(clicked()), this, SLOT(startBasicBenchmarkExecution()) );
  //stop benchmark button
  connect(basicStopBenchmarkButton, SIGNAL(clicked()), &benchmarkController, SLOT(stopExecution()) );

  connect(&benchmarkController, SIGNAL(benchmarkStopped()), ui->basicBenchmarkWidget, SLOT(hideStopButton()) );

  //route incoming benchmark result info to appropriate plots
  connect(&benchmarkController, SIGNAL(benchmarkStarted(int)), ui->basicBenchmarkWidget, SLOT(setActiveBenchmarkPlot(int)) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString, int)), ui->basicBenchmarkWidget, SLOT(updateBenchmarkUnitMeasure(QString, int)) );
  //received a benchmark test result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString, double, double, int, int)),
          ui->basicBenchmarkWidget, SLOT(parseBenchmarkResult(QString, double, double, int, int)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(finalResultSignal(QString, double)), ui->basicBenchmarkWidget, SLOT(updateFinalResultPlot(QString,double)) );
  //show the start button once all benchmarks are done
  connect(&benchmarkController, SIGNAL(emptyBenchmarkQ()), ui->basicBenchmarkWidget, SLOT(showBenchmarkStartButton()) );
  //connect progress signals
  connect(&benchmarkController, SIGNAL(testProgress()), ui->basicBenchmarkWidget, SLOT(updateBenchProgress()) );

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
    ui->expert_contextComboBox->insertItem( i, contextMap.value(i) );
  }

  //  connect(ui->basic_platformsComboBox, SIGNAL(currentIndexChanged(int)), ui->expert_platformsComboBox, SLOT(setCurrentIndex(int)) );
  //  connect(ui->expert_platformsComboBox, SIGNAL(currentIndexChanged(int)), ui->basic_platformsComboBox, SLOT(setCurrentIndex(int)) );
  //  connect(ui->basic_contextComboBox, SIGNAL(activated(int)), this, SLOT(switchContext(int)) );
  //  connect(ui->expert_contextComboBox, SIGNAL(activated(int)), this, SLOT(switchContext(int)) );


}

void MainWindow::selectionChanged()
{

}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
{

}

void MainWindow::updateBenchmarkListWidget(QListWidgetItem *item)
{

}

//switches viennacl to selected context and displays its devices
void MainWindow::switchContext(int contextNumber){
  viennacl::ocl::switch_context((long)contextNumber);
  std::cout << "Context id: "<< contextNumber <<" Context value: " << viennacl::ocl::current_context().handle().get() << std::endl;
  std::cout << "Platform_index: "<< viennacl::ocl::current_context().platform_index()<< std::endl;
  std::cout << "Device name: "<< viennacl::ocl::current_context().current_device().name()<< std::endl;
}

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

void MainWindow::setActiveBenchmarkPlot(int benchmarkIdNumber){//-!
  basic_DetailedPlotTab->setCurrentIndex(benchmarkIdNumber);
  activeBenchmark = benchmarkIdNumber;
}

void MainWindow::interconnectViews(){
  //  connect(ui->basic_BenchmarkListWidget, SIGNAL()

}

void MainWindow::initExpert(){
  //  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), ui->expert_BenchmarkListWidget, SLOT() );
  //  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );

  expert_DetailedPlotTab = new QTabWidget(ui->expert_BenchmarkTab);
  expert_DetailedPlotTab->setStyleSheet("QTabBar::tab{width: 75px;height: 25px;}");

  blas3_expertDetailedPlot = new QCustomPlot();
  copy_expertDetailedPlot = new QCustomPlot();
  //  qr_expertDetailedPlot = new QCustomPlot();
  //  solver_expertDetailedPlot = new QCustomPlot();
  sparse_expertDetailedPlot = new QCustomPlot();
  vector_expertDetailedPlot = new QCustomPlot();

  expert_DetailedPlotsVector.insert(BLAS3, blas3_expertDetailedPlot);
  expert_DetailedPlotsVector.insert(COPY, copy_expertDetailedPlot);
  //  expert_DetailedPlotsVector.insert(QR, qr_expertDetailedPlot);
  //  expert_DetailedPlotsVector.insert(SOLVER, solver_expertDetailedPlot);
  expert_DetailedPlotsVector.insert(SPARSE, sparse_expertDetailedPlot);
  expert_DetailedPlotsVector.insert(VECTOR, vector_expertDetailedPlot);

  expert_DetailedPlotTab->insertTab(BLAS3, blas3_expertDetailedPlot,"Blas3");
  expert_DetailedPlotTab->insertTab(COPY, copy_expertDetailedPlot,"Copy");
  //  expert_DetailedPlotTab->insertTab(QR, qr_expertDetailedPlot,"Qr");
  //  expert_DetailedPlotTab->insertTab(SOLVER, solver_expertDetailedPlot,"Solver");
  expert_DetailedPlotTab->insertTab(SPARSE, sparse_expertDetailedPlot,"Sparse");
  expert_DetailedPlotTab->insertTab(VECTOR, vector_expertDetailedPlot,"Vector");

  ui->expert_CollapseWidget->setChildWidget(expert_DetailedPlotTab);
  ui->expert_CollapseWidget->setText("Detailed Test Results");

  //xAxis bottom
  //yAxis left
  //xAxis2 top
  //yAxis2 right
  QColor backgroundColor(240,240,240);
  QBrush backgroundBrush(backgroundColor);

  foreach(QCustomPlot* plot, expert_DetailedPlotsVector){

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(plot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

    //filter each item selection
    connect(plot, SIGNAL( selectionChangedByUser()), this, SLOT(selectionChanged()) );
    //    plot->axisRect()->setAutoMargins(QCP::msNone);
    //    plot->axisRect()->setMargins(QMargins( 0, 0, 50, 0 ));

    plot->axisRect()->setupFullAxesBox();
    //Disable secondary axes
    plot->yAxis2->setVisible(false);
    plot->xAxis2->setVisible(false);

    plot->setInteractions(QCP::iSelectPlottables | QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectLegend);
    plot->legend->setVisible(false);


    plot->yAxis->setTickLength( 0, 2);
    plot->yAxis->grid()->setVisible(true);
    plot->yAxis->setTickLabelRotation( 0 );

    plot->yAxis->setAutoSubTicks(false);
    plot->yAxis->setAutoTickLabels(false);
    plot->yAxis->setAutoTicks(false);
    plot->yAxis->setAutoTickStep(false);
    QVector<double> emptyTickVector;
    plot->yAxis->setTickVector(emptyTickVector);
    QVector<QString> emptyTickVectorLabels;
    plot->yAxis->setTickVectorLabels(emptyTickVectorLabels);

    plot->xAxis->setAutoSubTicks(true);
    plot->xAxis->setAutoTickLabels(true);
    plot->xAxis->setAutoTicks(true);
    plot->xAxis->setAutoTickStep(true);

    plot->setBackground(backgroundBrush);
  }


  ui->expert_FinalResultPlot->axisRect()->setAutoMargins(QCP::msNone);
  ui->expert_FinalResultPlot->axisRect()->setMargins(QMargins( 100, 15, 60, 40 ));
  ui->expert_FinalResultPlot->axisRect()->setupFullAxesBox();
  //Disable secondary axes & legend
  ui->expert_FinalResultPlot->yAxis2->setVisible(false);
  ui->expert_FinalResultPlot->xAxis2->setVisible(false);
  ui->expert_FinalResultPlot->legend->setVisible(false);
  //Enable selecting plots
  ui->expert_FinalResultPlot->setInteractions(QCP::iSelectPlottables|QCP::iRangeDrag|QCP::iRangeZoom);

  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->expert_FinalResultPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

  QVector<QString> finalResultPlotLabels;
  finalResultPlotLabels.append("Vector - GFLOPs");
  finalResultPlotLabels.append("Sparse - GFLOPs");
  //  finalResultPlotLabels.append("Solver - GFLOPs");
  //  finalResultPlotLabels.append("Qr - GFLOPs");
  finalResultPlotLabels.append("Copy - GB/s");
  finalResultPlotLabels.append("Blas3 - GFLOPs");

  //  Plot mapping
  //  Vector - 1
  //  Sparse - 2
  //  Copy - 3
  //  Blas3 - 4

  QVector<double> finalResultPlotTicks;
  finalResultPlotTicks.append(1);
  finalResultPlotTicks.append(2);
  finalResultPlotTicks.append(3);
  finalResultPlotTicks.append(4);

  ui->expert_FinalResultPlot->yAxis->setAutoTickLabels(false);
  ui->expert_FinalResultPlot->yAxis->setAutoTicks(false);
  ui->expert_FinalResultPlot->yAxis->setTickVectorLabels(finalResultPlotLabels);
  ui->expert_FinalResultPlot->yAxis->setTickVector(finalResultPlotTicks);
  ui->expert_FinalResultPlot->yAxis->setSubTickCount( 0 );
  ui->expert_FinalResultPlot->yAxis->setTickLength( 0, 2);
  ui->expert_FinalResultPlot->yAxis->setRange( 0.5, 5.0);
  ui->expert_FinalResultPlot->yAxis->grid()->setVisible(true);
  ui->expert_FinalResultPlot->yAxis->setTickLabelRotation( 0 );


  ui->expert_FinalResultPlot->xAxis->grid()->setSubGridVisible(true);
  ui->expert_FinalResultPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
  ui->expert_FinalResultPlot->xAxis->setScaleLogBase(10);
  ui->expert_FinalResultPlot->xAxis->setNumberFormat("f"); // e = exponential, b = beautiful decimal powers
  ui->expert_FinalResultPlot->xAxis->setNumberPrecision(0);
  ui->expert_FinalResultPlot->xAxis->setAutoTicks(false);
  ui->expert_FinalResultPlot->xAxis->setAutoTickLabels(false);
  ui->expert_FinalResultPlot->xAxis->setAutoTickStep(false);
  QVector<double> ticks;
  ticks.append(0.5);
  ticks.append(1);
  ticks.append(5);
  ticks.append(10);
  ticks.append(20);
  ticks.append(50);
  ticks.append(100);
  ticks.append(200);
  ticks.append(500);
  ticks.append(1000);
  ticks.append(2000);
  ui->expert_FinalResultPlot->xAxis->setTickVector(ticks);

  QVector<QString> tickLabels;
  tickLabels.append("0.5");
  tickLabels.append("1");
  tickLabels.append("5");
  tickLabels.append("10");
  tickLabels.append("20");
  tickLabels.append("50");
  tickLabels.append("100");
  tickLabels.append("200");
  tickLabels.append("500");
  tickLabels.append("1000");
  tickLabels.append("2000");
  ui->expert_FinalResultPlot->xAxis->setTickVectorLabels(tickLabels);

  //  ui->expert_FinalResultPlot->xAxis->setTickLengthOut(200);
  ui->expert_FinalResultPlot->xAxis->setRangeLower(0);
  //  ui->expert_FinalResultPlot->xAxis->setRange(0,1);

  ui->expert_FinalResultPlot->setBackground(backgroundBrush);

  ui->expert_StopBenchmarkButton->hide();

  connect(ui->expert_BenchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  connect(ui->expert_BenchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  for ( int i = 0; i < ui->expert_BenchmarkListWidget->count(); i++ ) {
    ui->expert_BenchmarkListWidget->item(i)->setSelected(true);
  }
}

bool MainWindow::getBasicPrecision(){
  if(basicSinglePrecisionButton->isChecked()){
    return SINGLE_PRECISION;
  }
  else{
    return DOUBLE_PRECISION;
  }
}

//execute the currently selected benchmark
void MainWindow::startBasicBenchmarkExecution(){
  ui->basicBenchmarkWidget->resetAllPlots();
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
  //  qDebug()<<"Selected benchmarks: "<<selectedBenchmarkItems;

  //set progress bar max value
  basicProgressBar->setMaximum(maximumBenchProgress);

  basicStopBenchmarkButton->show();
  basicStartBenchmarkButton->hide();

  switchContext( basicContextComboBox->currentIndex() );//switch to currently selected context
  benchmarkController.executeSelectedBenchmark( selectedBenchmarkItems, getBasicPrecision() );//start the benchmark
}

MainWindow::~MainWindow()
{
  delete ui;
}

