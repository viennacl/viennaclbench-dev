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

  connect(ui->actionAbout,SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
  connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()) );
  connect(ui->menuListWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)) );
  ui->menuListWidget->setCurrentRow(0);

  maximumBenchProgress = 0;
  currentBenchProgress = 0;

  //setup benchmark plots
  //HomeScreen class takes care of its own init
  initBasic();
  initExpert();
  interconnectViews();
  initMatrixMarket();
  //SystemInfoScreen class takes care of its own init
  initPlatformDeviceChooser();


  //    Benchmark_Sparse s; //working
  //  Benchmark_Copy s; //working
  //    Benchmark_Vector s; working
  //  Benchmark_Solver s; //working
  //  Benchmark_Scheduler s; //working
  //    Benchmark_Blas3 s; //working
  //  Benchmark_Qr s; //working
  //    s.execute();

  //connect quickstart button
  connect(ui->homeScreen->quickStartButton, SIGNAL(clicked()), this, SLOT(quickstartFullBenchmark()) );
  //run benchmark button clicked -> execute benchmark
  connect(ui->basic_StartBenchmarkButton, SIGNAL(clicked()), this, SLOT(startBenchmarkExecution()) );
  //stop benchmark button
  connect(ui->basic_StopBenchmarkButton, SIGNAL(clicked()), this, SLOT(stopBenchmarkExecution()) );

  //route incoming benchmark result info to appropriate plots
  connect(&benchmarkController, SIGNAL(benchmarkStarted(int)), this, SLOT(setActiveBenchmarkPlot(int)) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString, int)), this, SLOT(updateBenchmarkUnitMeasure(QString, int)) );
  //received a benchmark test result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString, double, double, int, int)),
          this, SLOT(parseBenchmarkResult(QString, double, double, int, int)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(finalResultSignal(QString, double)), this, SLOT(updateFinalResultPlot(QString,double)) );
  //show the start button once all benchmarks are done
  connect(&benchmarkController, SIGNAL(emptyBenchmarkQ()), this, SLOT(showBenchmarkStartButton()) );
  //connect progress signals
  connect(&benchmarkController, SIGNAL(testProgress()), this, SLOT(updateBenchProgress()) );

  connect(ui->basic_DoubleButton, SIGNAL(clicked()), this, SLOT(updateDoublePrecisionButtons()) );
  connect(ui->basic_SingleButton, SIGNAL(clicked()), this, SLOT(updateSinglePrecisionButtons()) );

}

void MainWindow::showBenchmarkStartButton(){
  ui->basic_StopBenchmarkButton->hide();
  ui->basic_StartBenchmarkButton->show();
  ui->basic_ProgressBar->setFormat("Done");
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
    ui->basic_contextComboBox->insertItem( i, contextMap.value(i) );
    ui->expert_contextComboBox->insertItem( i, contextMap.value(i) );
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

void MainWindow::updateSinglePrecisionButtons(){
  ui->basic_SingleButton->setChecked(true);
  ui->basic_SingleButton->setIcon(QIcon(":/icons/icons/checkTrue.png"));

  ui->basic_DoubleButton->setChecked(false);
  ui->basic_DoubleButton->setIcon(QIcon(":/icons/icons/empty.png"));

}
void MainWindow::updateDoublePrecisionButtons(){
  ui->basic_DoubleButton->setChecked(true);
  ui->basic_DoubleButton->setIcon(QIcon(":/icons/icons/checkTrue.png"));

  ui->basic_SingleButton->setChecked(false);
  ui->basic_SingleButton->setIcon(QIcon(":/icons/icons/empty.png"));

}

void MainWindow::quickstartFullBenchmark(){
  ui->basic_BenchmarkListWidget->selectAllItems();
  ui->menuListWidget->setCurrentRow(1);
  startBenchmarkExecution();
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

void MainWindow::setActiveBenchmarkPlot(int benchmarkIdNumber){
  basic_DetailedPlotTab->setCurrentIndex(benchmarkIdNumber);
  activeBenchmark = benchmarkIdNumber;
}

void MainWindow::updateFinalResultPlot(QString benchmarkName, double finalResult){
  plotFinalResult(benchmarkName, finalResult, ui->basic_FinalResultPlot);
}

//shows the detailed graph of a clicked final result bar
void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
{
  QString clickedBenchmarkBar = plottable->name();
  if(clickedBenchmarkBar == "Blas3"){
    basic_DetailedPlotTab->setCurrentIndex(BLAS3);
  }
  else if(clickedBenchmarkBar == "Copy"){
    basic_DetailedPlotTab->setCurrentIndex(COPY);
  }
  else if(clickedBenchmarkBar == "Qr"){
    basic_DetailedPlotTab->setCurrentIndex(QR);
  }
  else if(clickedBenchmarkBar == "Scheduler"){
    basic_DetailedPlotTab->setCurrentIndex(SCHEDULER);
  }
  else if(clickedBenchmarkBar == "Solver"){
    basic_DetailedPlotTab->setCurrentIndex(SOLVER);
  }
  else if(clickedBenchmarkBar == "Sparse"){
    basic_DetailedPlotTab->setCurrentIndex(SPARSE);
  }
  else if(clickedBenchmarkBar == "Vector"){
    basic_DetailedPlotTab->setCurrentIndex(VECTOR);
  }
}

//detailed plot selection filter
void MainWindow::selectionChanged()
{
  int currentPlotIndex = basic_DetailedPlotTab->currentIndex();
  QCustomPlot *currentPlot = basic_DetailedPlotsVector[currentPlotIndex];
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i< currentPlot->graphCount(); ++i)
  {
    QCPGraph *graph = currentPlot->graph(i);
    QCPPlottableLegendItem *item = currentPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}

void MainWindow::initBasic(){
  //normalize size of each list menu item
  for ( int i = 0; i < ui->menuListWidget->count(); i++ ) {
    ui->menuListWidget->item(i)->setSizeHint(ui->menuListWidget->itemSizeHint());
  }

  basic_DetailedPlotTab = new QTabWidget(ui->basic_BenchmarkTab);
  basic_DetailedPlotTab->setStyleSheet("QTabBar::tab{width: 75px;height: 25px;}");

  blas3_DetailedPlot = new QCustomPlot();
  copy_DetailedPlot = new QCustomPlot();
  //  qr_DetailedPlot = new QCustomPlot();
  //  solver_DetailedPlot = new QCustomPlot();
  sparse_DetailedPlot = new QCustomPlot();
  vector_DetailedPlot = new QCustomPlot();

  basic_DetailedPlotsVector.insert(BLAS3, blas3_DetailedPlot);
  basic_DetailedPlotsVector.insert(COPY, copy_DetailedPlot);
  //  basic_DetailedPlotsVector.insert(QR, qr_DetailedPlot);
  //  basic_DetailedPlotsVector.insert(SOLVER, solver_DetailedPlot);
  basic_DetailedPlotsVector.insert(SPARSE, sparse_DetailedPlot);
  basic_DetailedPlotsVector.insert(VECTOR, vector_DetailedPlot);

  basic_DetailedPlotTab->insertTab(BLAS3, blas3_DetailedPlot,"Blas3");
  basic_DetailedPlotTab->insertTab(COPY, copy_DetailedPlot,"Copy");
  //  basic_DetailedPlotTab->insertTab(QR, qr_DetailedPlot,"Qr");
  //  basic_DetailedPlotTab->insertTab(SOLVER, solver_DetailedPlot,"Solver");
  basic_DetailedPlotTab->insertTab(SPARSE, sparse_DetailedPlot,"Sparse");
  basic_DetailedPlotTab->insertTab(VECTOR, vector_DetailedPlot,"Vector");

  ui->basic_CollapseWidget->setChildWidget(basic_DetailedPlotTab);
  ui->basic_CollapseWidget->setText("Detailed Test Results");

  //xAxis bottom
  //yAxis left
  //xAxis2 top
  //yAxis2 right
  QColor backgroundColor(240,240,240);
  QBrush backgroundBrush(backgroundColor);

  foreach(QCustomPlot* plot, basic_DetailedPlotsVector){

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


  ui->basic_FinalResultPlot->axisRect()->setAutoMargins(QCP::msNone);
  ui->basic_FinalResultPlot->axisRect()->setMargins(QMargins( 100, 15, 60, 40 ));
  ui->basic_FinalResultPlot->axisRect()->setupFullAxesBox();
  //Disable secondary axes & legend
  ui->basic_FinalResultPlot->yAxis2->setVisible(false);
  ui->basic_FinalResultPlot->xAxis2->setVisible(false);
  ui->basic_FinalResultPlot->legend->setVisible(false);
  //Enable selecting plots
  ui->basic_FinalResultPlot->setInteractions(QCP::iSelectPlottables|QCP::iRangeDrag|QCP::iRangeZoom);

  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->basic_FinalResultPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

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

  ui->basic_FinalResultPlot->yAxis->setAutoTickLabels(false);
  ui->basic_FinalResultPlot->yAxis->setAutoTicks(false);
  ui->basic_FinalResultPlot->yAxis->setTickVectorLabels(finalResultPlotLabels);
  ui->basic_FinalResultPlot->yAxis->setTickVector(finalResultPlotTicks);
  ui->basic_FinalResultPlot->yAxis->setSubTickCount( 0 );
  ui->basic_FinalResultPlot->yAxis->setTickLength( 0, 2);
  ui->basic_FinalResultPlot->yAxis->setRange( 0.5, 5.0);
  ui->basic_FinalResultPlot->yAxis->grid()->setVisible(true);
  ui->basic_FinalResultPlot->yAxis->setTickLabelRotation( 0 );


  ui->basic_FinalResultPlot->xAxis->grid()->setSubGridVisible(true);
  ui->basic_FinalResultPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
  ui->basic_FinalResultPlot->xAxis->setScaleLogBase(10);
  ui->basic_FinalResultPlot->xAxis->setNumberFormat("f"); // e = exponential, b = beautiful decimal powers
  ui->basic_FinalResultPlot->xAxis->setNumberPrecision(0);
  ui->basic_FinalResultPlot->xAxis->setAutoTicks(false);
  ui->basic_FinalResultPlot->xAxis->setAutoTickLabels(false);
  ui->basic_FinalResultPlot->xAxis->setAutoTickStep(false);
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
  ui->basic_FinalResultPlot->xAxis->setTickVector(ticks);

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
  ui->basic_FinalResultPlot->xAxis->setTickVectorLabels(tickLabels);

  //  ui->basic_FinalResultPlot->xAxis->setTickLengthOut(200);
  ui->basic_FinalResultPlot->xAxis->setRangeLower(0);
  //  ui->basic_FinalResultPlot->xAxis->setRange(0,1);

  ui->basic_FinalResultPlot->setBackground(backgroundBrush);

  ui->basic_StopBenchmarkButton->hide();

  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  for ( int i = 0; i < ui->basic_BenchmarkListWidget->count(); i++ ) {
    ui->basic_BenchmarkListWidget->item(i)->setSelected(true);
  }
}

void MainWindow::legendClicked(QCPLegend* legend,QCPAbstractLegendItem* item,QMouseEvent* event){
  qDebug()<<"legend item clicked";
  //item->
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

bool MainWindow::getPrecision(){
  if(ui->basic_SingleButton->isChecked()){
    return SINGLE_PRECISION;
  }
  else{
    return DOUBLE_PRECISION;
  }
}

//stop the benchmarking process
void MainWindow::stopBenchmarkExecution(){
  benchmarkController.stopExecution();
  ui->basic_StopBenchmarkButton->hide();
  ui->basic_StartBenchmarkButton->show();
}

void MainWindow::updateBenchProgress(){
  currentBenchProgress++;
  ui->basic_ProgressBar->setValue(currentBenchProgress);
  ui->basic_ProgressBar->setFormat("Running Test %v of %m");
}

//execute the currently selected benchmark
void MainWindow::startBenchmarkExecution(){
  resetAllPlots();
  currentBenchProgress = 0;
  maximumBenchProgress = 0;
  ui->basic_ProgressBar->setValue(currentBenchProgress);
  ui->basic_ProgressBar->setFormat("Starting Benchmark...");
  QStringList selectedBenchmarkItems;
  for ( int i = 1; i < ui->basic_BenchmarkListWidget->count(); i++ ) {
    if(ui->basic_BenchmarkListWidget->item(i)->isSelected() ){
      selectedBenchmarkItems.append(ui->basic_BenchmarkListWidget->item(i)->text());
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
  ui->basic_ProgressBar->setMaximum(maximumBenchProgress);

  ui->basic_StopBenchmarkButton->show();
  ui->basic_StartBenchmarkButton->hide();

  switchContext( ui->basic_contextComboBox->currentIndex() );//switch to currently selected context
  benchmarkController.executeSelectedBenchmark( selectedBenchmarkItems, getPrecision() );//start the benchmark
}

void MainWindow::updateBenchmarkListWidget(QListWidgetItem *item)
{
  //item(0) is the 'All' benchmarks selection option
  if(ui->basic_BenchmarkListWidget->row(item) == 0){
    if(item->isSelected()){
      ui->basic_BenchmarkListWidget->selectAllItems();
    }
    else{
      ui->basic_BenchmarkListWidget->deselectAllItems();
    }
  }
  else{
    if(item->isSelected()){
      item->setIcon(QIcon(":/icons/icons/checkTrue.png"));
      ui->basic_BenchmarkListWidget->checkSelectedItems();
    }
    else{
      ui->basic_BenchmarkListWidget->item(0)->setSelected(false);
      ui->basic_BenchmarkListWidget->item(0)->setIcon(QIcon(":/icons/icons/checkFalse.png"));
      item->setIcon(QIcon(":/icons/icons/checkFalse.png"));
    }
  }
}

void MainWindow::resetAllPlots(){
  resetPlotData(ui->basic_FinalResultPlot);
  //reset all plots
  foreach(QCustomPlot* plot, basic_DetailedPlotsVector){
    resetPlotData(plot);
    plot->yAxis->setTickVector(QVector<double>() );
    plot->yAxis->setTickVectorLabels(QVector<QString>() );
    plot->yAxis->setRange(0,5);
    plot->xAxis->setTickVector(QVector<double>() );
    plot->xAxis->setTickVectorLabels(QVector<QString>() );
    plot->xAxis->setRange(0,5);
    plot->replot();
  }
}

//reset the graph
void MainWindow::resetPlotData(QCustomPlot *benchmarkGraph)
{
  benchmarkGraph->clearGraphs();
  benchmarkGraph->clearPlottables();
  benchmarkGraph->clearItems();
  benchmarkGraph->xAxis->setRange(0,1);
  benchmarkGraph->replot();
}

//set the benchmark's unit measure
void MainWindow::updateBenchmarkUnitMeasure(QString unitMeasureName, int axis)
{
  switch(axis){
  case Qt::XAxis:
    qDebug()<<""<<unitMeasureName;
    basic_DetailedPlotsVector[activeBenchmark]->xAxis->setLabel(unitMeasureName);
    break;
  case Qt::YAxis:
    qDebug()<<""<<unitMeasureName;
    basic_DetailedPlotsVector[activeBenchmark]->yAxis->setLabel(unitMeasureName);
    break;
  default:
    break;
  }

}

//parse the received benchmark result name and value
void MainWindow::parseBenchmarkResult(QString benchmarkName, double key, double resultValue, int graphType, int testId){
  if(graphType == BAR_GRAPH){
    plotBarResult(benchmarkName, key, resultValue, basic_DetailedPlotsVector[activeBenchmark]);
  }
  else if(graphType == LINE_GRAPH){
    plotLineResult(benchmarkName, key, resultValue, basic_DetailedPlotsVector[activeBenchmark], testId);
  }
}

void MainWindow::plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId){
  if(customPlot->legend->visible() == false){
    customPlot->plotLayout()->addElement(0,1, customPlot->legend);
    customPlot->legend->setVisible(true);
    customPlot->legend->setSelectableParts( QCPLegend::spItems );
    customPlot->legend->setMaximumSize( 150, QWIDGETSIZE_MAX );
  }
  customPlot->legend->setFont(QFont("Helvetica", 9));
  customPlot->legend->setRowSpacing(-3);

  customPlot->xAxis->setAutoTicks(false);
  customPlot->xAxis->setAutoTickLabels(false);
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setAutoSubTicks(false);

  QVector<double> tickPositions = customPlot->xAxis->tickVector();
  QVector<QString> tickLabels =  customPlot->xAxis->tickVectorLabels();

  double newTickPosition = key;
  QString newTickLabel = QString::number(key, 'f', 0);

  tickPositions.append( newTickPosition );
  tickLabels.append( newTickLabel );

  customPlot->xAxis->setTickVector(tickPositions);
  customPlot->xAxis->setTickVectorLabels(tickLabels);

  //  customPlot->xAxis->grid()->setSubGridVisible(true);
  customPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
  customPlot->xAxis->setScaleLogBase(10);
  customPlot->xAxis->setNumberFormat("f"); // e = exponential, b = beautiful decimal powers
  customPlot->xAxis->setNumberPrecision(0);
  QFont axisTickFont;
  axisTickFont.setBold(false);
  customPlot->xAxis->setTickLabelFont(QFont(axisTickFont));

  customPlot->xAxis->setLabelFont(axisTickFont);
  customPlot->xAxis->setTickLabelFont(QFont(axisTickFont));

  //  customPlot->xAxis->setAutoTicks(true);
  customPlot->yAxis->setAutoTicks(true);

  //  customPlot->xAxis->setAutoTickStep(true);
  customPlot->yAxis->setAutoTickStep(true);

  //  customPlot->xAxis->setAutoTickLabels(true);
  customPlot->yAxis->setAutoTickLabels(true);

  QCPGraph *currentResultGraph;

  if(customPlot->graph(testId) == 0){
    //no graph for this test result
    //create one
    qDebug()<<"adding new graph";
    currentResultGraph = customPlot->addGraph(customPlot->xAxis, customPlot->yAxis);
  }
  else{
    //a graph for this test result already exists
    //add the new data to it
    currentResultGraph = customPlot->graph(testId);
  }

  QPen pen;//(QColor("red"));
  switch(testId){
  case 0: pen.setColor("green");
    break;
  case 1: pen.setColor("blue");
    break;
  case 2: pen.setColor("black");
    break;
  case 3: pen.setColor("red");
    break;
  case 4: pen.setColor("cyan");
    break;
  case 5: pen.setColor("magenta");
    break;
  case 6: pen.setColor("gray");
    break;
  case 7: pen.setColor("yellow");
    break;
  case 8: pen.setColor("pink");
    break;
  default: pen.setColor("orange");
  }
  currentResultGraph->setName(benchmarkName);
  currentResultGraph->addData( key, value );
  currentResultGraph->rescaleAxes(true);
  currentResultGraph->setPen(pen);
  currentResultGraph->setLineStyle(QCPGraph::lsLine);
  currentResultGraph->setScatterStyle(QCPScatterStyle::ssCrossSquare);

  customPlot->rescaleAxes();

  //  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();
}

//main result diplay function
//x and y axis are swapped to achieve horizontal bar display
void MainWindow::plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot){
  customPlot->axisRect()->setAutoMargins(QCP::msLeft);
  QMargins margins = customPlot->axisRect()->margins();
  margins.setRight(60);//reserve space for the largest result
  margins.setBottom(35);//reserve space for measure label

  customPlot->axisRect()->setMargins( margins );
  QVector<double> currentTickVector = customPlot->yAxis->tickVector();
  QVector<QString> currentTickVectorLabels =  customPlot->yAxis->tickVectorLabels();

  double currentValue = value;
  double currentKey = currentTickVector.size();

  QCPBars *resultBar = new QCPBars(customPlot->yAxis, customPlot->xAxis);
  resultBar->setPen(QPen(Qt::NoPen));
  resultBar->setName(benchmarkName);
  resultBar->addData(currentKey, currentValue);

  currentTickVector.append(currentKey);
  currentTickVectorLabels.append(benchmarkName);

  customPlot->yAxis->setTickVector(currentTickVector);
  customPlot->yAxis->setTickVectorLabels(currentTickVectorLabels);

  customPlot->addPlottable(resultBar);

  customPlot->rescaleAxes();

  QCPItemText *text = new QCPItemText(customPlot);

  text->setClipToAxisRect(false);//allow this item to flow over the main plot rectangle, hence showing it when it can't fit the plot rectangle

  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(  currentValue , currentKey );
  text->setFont(QFont(font().family(), 10, QFont::Bold)); // make font a bit larger

  //Add a whitespace in front of the result value to separate it from the result bar
  //Prolly could've also used margins, but meh
  //And format the result number to two decimals
  text->setText( QString(" ") + QString::number( currentValue, 'f', 2  ));

  text->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  customPlot->addItem(text);

  QFont textFont;
  textFont.family();
  text->setFont(QFont(font().family(), 10, QFont::Bold)); // make font a bit larger

  customPlot->yAxis->setRangeLower(-0.5);
  customPlot->replot();
}

void MainWindow::plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot){
  //  Plot mapping
  //  Vector - 1
  //  Sparse - 2
  //  Copy - 3
  //  Blas3 - 4
  QCPBarData currentData;
  currentData.value = value;
  if(benchmarkName == "Blas3"){
    currentData.key = 4;
  }
  else if(benchmarkName == "Copy"){
    currentData.key = 3;
  }
  else if(benchmarkName == "Qr"){
    currentData.key = 4;
  }
  else if(benchmarkName == "Scheduler"){
    currentData.key = 0;
    currentData.value = 0;
  }
  else if(benchmarkName == "Solver"){
    currentData.key = 3;
  }
  else if(benchmarkName == "Sparse"){
    currentData.key = 2;
  }
  else if(benchmarkName == "Vector"){
    currentData.key = 1;
  }
  else{
    qDebug()<<"Error parsing benchmark name";
  }

  QCPBars *resultBar = new QCPBars(customPlot->yAxis, customPlot->xAxis);
  resultBar->setName(benchmarkName);
  resultBar->addData(currentData);
  resultBar->setPen(QPen(Qt::NoPen));

  customPlot->addPlottable(resultBar);

  QCPItemText *text = new QCPItemText(customPlot);

  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  text->setClipToAxisRect(false);//allow this item to flow over the main plot rectangle, hence showing it when it can't fit the plot rectangle
  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(  currentData.value , currentData.key );
  text->setFont(QFont(font().family(), 10, QFont::Bold)); // make font a bit larger

  //Add a whitespace in front of the result value to separate it from the result bar
  //Prolly could've also used margins, but meh
  //And format the result number to two decimals
  text->setText(QString(" ") + QString::number( currentData.value, 'f', 2  ));

  text->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  customPlot->addItem(text);

  customPlot->xAxis->rescale();
  customPlot->replot();
}

MainWindow::~MainWindow()
{
  delete ui;
}

