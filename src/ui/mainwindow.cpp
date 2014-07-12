#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->actionAbout,SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
  connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()) );
  connect(ui->menuListWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)) );
  ui->menuListWidget->setCurrentRow(0);

  //setup benchmark plots
  initHomeScreen();
  initBasicView();
  initExpertView();
  initMatrixMarket();

  //    Benchmark_Sparse s; //working
  //  Benchmark_Copy s; //working
  //    Benchmark_Vector s; working
  //  Benchmark_Solver s; //working
  //  Benchmark_Scheduler s; //working
  //    Benchmark_Blas3 s; //working
  //  Benchmark_Qr s; //working
  //    s.execute();

  //connect quickstart button
  connect(ui->homeQuickStartButon, SIGNAL(clicked()), this, SLOT(quickstartFullBenchmark()) );
  //run benchmark button clicked -> execute benchmark
  connect(ui->basic_StartBenchmarkButton, SIGNAL(clicked()), this, SLOT(startBenchmarkExecution()) );
  //route incoming benchmark result info to appropriate plots
  connect(&benchmarkController, SIGNAL(benchmarkStarted(int)), this, SLOT(setActiveBenchmarkPlot(int)) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString)), this, SLOT(updateBenchmarkUnitMeasure(QString)) );
  //received a benchmark result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString,double)), this, SLOT(parseBenchmarkResult(QString,double)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(finalResultSignal(QString, double)), this, SLOT(updateFinalResultPlot(QString,double)) );

}

void MainWindow::quickstartFullBenchmark(){
  ui->basic_BenchmarkListWidget->selectAllItems();
  ui->menuListWidget->setCurrentRow(1);
  startBenchmarkExecution();
}

void MainWindow::initMatrixMarket(){
//  QThread *workerThread = new QThread();
//  ui->webView->moveToThread(workerThread);
//  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()) );
//  workerThread->start();
  ui->webView->load(QUrl("http://www.cise.ufl.edu/research/sparse/matrices/"));
}

void MainWindow::setActiveBenchmarkPlot(int benchmarkIdNumber){
  basic_DetailedPlotTab->setCurrentIndex(benchmarkIdNumber);
  activeBenchmark = benchmarkIdNumber;
}

void MainWindow::updateFinalResultPlot(QString benchmarkName, double finalResult){
  plotFinalResult(benchmarkName, finalResult, ui->basic_FinalResultPlot);
}

void MainWindow::initHomeScreen(){
  ui->homeSystemInfoLabel->clear();
#ifdef VIENNACL_WITH_OPENCL
  QString systemInfoString;
  typedef std::vector< viennacl::ocl::platform > platforms_type;
  platforms_type platforms = viennacl::ocl::get_platforms();

    bool is_first_element = true;
    for(platforms_type::iterator platform_iter = platforms.begin(); platform_iter != platforms.end(); ++platform_iter){

      typedef std::vector<viennacl::ocl::device> devices_type;
      devices_type devices = platform_iter->devices(CL_DEVICE_TYPE_ALL);
      std::cout << "# Vendor and version: " << platform_iter->info() << std::endl;

      if (is_first_element)
      {
        std::cout << "# ViennaCL uses this OpenCL platform by default." << std::endl;
        is_first_element = false;
      }
      std::cout << "# Available Devices: " << std::endl;
      for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
      {
          std::cout << std::endl;
  std::cout << " !!!!!!!!!!!" << std::endl;
          std::cout << "  -----------------------------------------" << std::endl;
          std::cout << iter->full_info();
  //        std::cout << iter
          std::cout << "  -----------------------------------------" << std::endl;

          systemInfoString.append("Name: ");
          systemInfoString.append(QString::fromStdString(iter->name( )) );
          systemInfoString.append("\n");
      }

    }



  systemInfoString.append("Type: ");
  cl_device_type localDeviceType = viennacl::ocl::current_device().type();
  if(localDeviceType & CL_DEVICE_TYPE_GPU){
    systemInfoString.append("CPU");
  }
  else if(localDeviceType & CL_DEVICE_TYPE_CPU){
    systemInfoString.append("CPU");
  }
  else if(localDeviceType & CL_DEVICE_TYPE_ACCELERATOR){
    systemInfoString.append("Accelerator");
  }
  else if(localDeviceType & CL_DEVICE_TYPE_DEFAULT){
    systemInfoString.append("(default)");
  }
  systemInfoString.append("\n");

  systemInfoString.append("Name: ");
  systemInfoString.append(QString::fromStdString(viennacl::ocl::current_device().name( )) );
  systemInfoString.append("\n");

  systemInfoString.append("Vendor: ");
  systemInfoString.append(QString::fromStdString(viennacl::ocl::current_device().vendor() ) );
  systemInfoString.append("\n");

  systemInfoString.append("Global Memory Size: ");
  systemInfoString.append(QString::number( ((uint64_t)viennacl::ocl::current_device().global_mem_size()/(1024*1024) ) ) );
  systemInfoString.append(" MB");
  systemInfoString.append("\n");

  systemInfoString.append("Clock Frequency: ");
  systemInfoString.append(QString::number(viennacl::ocl::current_device().max_clock_frequency()) );
  systemInfoString.append(" MHz");
  systemInfoString.append("\n");

#ifdef CL_DEVICE_OPENCL_C_VERSION
  systemInfoString.append("OpenCL C Version: ");
  systemInfoString.append(QString::fromStdString(viennacl::ocl::current_device().opencl_c_version() ) );
  systemInfoString.append("\n");
#endif

  systemInfoString.append("Version: ");
  systemInfoString.append(QString::fromStdString(viennacl::ocl::current_device().version() ) );
  systemInfoString.append("\n");

  systemInfoString.append("Driver Version: ");
  systemInfoString.append(QString::fromStdString(viennacl::ocl::current_device().driver_version() ) );
  systemInfoString.append("\n");

  ui->homeSystemInfoLabel->setText(systemInfoString);
#endif
}

void MainWindow::initBasicView(){
  qDebug()<<"init basic view";
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

  qDebug()<<"plot vectors";
  basic_DetailedPlotsVector.insert(BLAS3, blas3_DetailedPlot);
  basic_DetailedPlotsVector.insert(COPY, copy_DetailedPlot);
//  basic_DetailedPlotsVector.insert(QR, qr_DetailedPlot);
//  basic_DetailedPlotsVector.insert(SOLVER, solver_DetailedPlot);
  basic_DetailedPlotsVector.insert(SPARSE, sparse_DetailedPlot);
  basic_DetailedPlotsVector.insert(VECTOR, vector_DetailedPlot);
  qDebug()<<"after plot vectors";


  basic_DetailedPlotTab->insertTab(BLAS3, blas3_DetailedPlot,"Blas3");
  basic_DetailedPlotTab->insertTab(COPY, copy_DetailedPlot,"Copy");
//  basic_DetailedPlotTab->insertTab(QR, qr_DetailedPlot,"Qr");
//  basic_DetailedPlotTab->insertTab(SOLVER, solver_DetailedPlot,"Solver");
  basic_DetailedPlotTab->insertTab(SPARSE, sparse_DetailedPlot,"Sparse");
  basic_DetailedPlotTab->insertTab(VECTOR, vector_DetailedPlot,"Vector");
  qDebug()<<"after plot tabs";

  ui->basic_CollapseWidget->setChildWidget(basic_DetailedPlotTab);
  ui->basic_CollapseWidget->setText("Detailed Test Results");

  //xAxis bottom
  //yAxis left
  //xAxis2 top
  //yAxis2 right
  QColor backgroundColor(240,240,240);
  QBrush backgroundBrush(backgroundColor);
  qDebug()<<" before foreach";

  foreach(QCustomPlot* plot, basic_DetailedPlotsVector){
    plot->axisRect()->setupFullAxesBox();
    //Disable secondary axes
    plot->yAxis2->setVisible(false);
    plot->xAxis2->setVisible(false);

    plot->setInteractions(QCP::iSelectPlottables | QCP::iRangeDrag | QCP::iRangeZoom);
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
  ui->basic_FinalResultPlot->axisRect()->setMargins(QMargins( 100, 35, 0, 25 ));
  ui->basic_FinalResultPlot->axisRect()->setupFullAxesBox();
  //Disable secondary axes
  ui->basic_FinalResultPlot->yAxis2->setVisible(false);
  ui->basic_FinalResultPlot->xAxis2->setVisible(false);
  ui->basic_FinalResultPlot->setInteractions(QCP::iSelectPlottables);
  ui->basic_FinalResultPlot->legend->setVisible(false);

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

  ui->basic_FinalResultPlot->xAxis->setAutoTickLabels(true);
  ui->basic_FinalResultPlot->xAxis->setAutoTicks(true);
  ui->basic_FinalResultPlot->xAxis->setAutoTickStep(true);
  ui->basic_FinalResultPlot->xAxis->setRange(0,1);

  ui->basic_FinalResultPlot->setBackground(backgroundBrush);

//  ui->basic_FinalResultPlot->replot();

  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  for ( int i = 0; i < ui->basic_BenchmarkListWidget->count(); i++ ) {
    ui->basic_BenchmarkListWidget->item(i)->setSelected(true);
  }
}



void MainWindow::initExpertView(){
}

//execute the currently selected benchmark
void MainWindow::startBenchmarkExecution(){
  resetAllPlots();
  QStringList selectedBenchmarkItems;
  for ( int i = 1; i < ui->basic_BenchmarkListWidget->count(); i++ ) {
    if(ui->basic_BenchmarkListWidget->item(i)->isSelected() ){
      selectedBenchmarkItems.append(ui->basic_BenchmarkListWidget->item(i)->text());
    }
  }
  qDebug()<<"Selected benchmarks: "<<selectedBenchmarkItems;

  benchmarkController.executeSelectedBenchmark( selectedBenchmarkItems );
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
    plot->yAxis->tickVector().clear();
    plot->yAxis->tickVectorLabels().clear();
    plot->yAxis->setRange(0,1);
    plot->xAxis->tickVector().clear();
    plot->xAxis->tickVectorLabels().clear();
    plot->xAxis->setRange(0,1);
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
void MainWindow::updateBenchmarkUnitMeasure(QString unitMeasureName)
{
    basic_DetailedPlotsVector[activeBenchmark]->xAxis->setLabel(unitMeasureName);
}

//parse the received benchmark result name and value
void MainWindow::parseBenchmarkResult(QString benchmarkName, double resultValue){
//    barData.append(bandwidthValue);
//    ticks.append(barCounter++);
//    labels.append(benchmarkName);
    plotResult(benchmarkName, resultValue, basic_DetailedPlotsVector[activeBenchmark]);
}

//main result diplay function
//x and y axis are swapped to achieve horizontal bar display
void MainWindow::plotResult(QString benchmarkName, double value, QCustomPlot *customPlot){
  //  customPlot->yAxis->setAutoTicks(false);
  //  customPlot->yAxis->setAutoTickLabels(false);
  //  customPlot->yAxis->setTickLabelRotation(60);
  //  customPlot->yAxis->setTickVector(ticks);
  //  customPlot->yAxis->setTickVectorLabels(labels);
  //  customPlot->yAxis->setSubTickCount(0);
  //  customPlot->yAxis->setTickLength(0, 2);
  //  customPlot->yAxis->grid()->setVisible(true);
  //  customPlot->yAxis->setTickLabelRotation(0);

//  //increase xAxis scale to fit new result, if necessary
//  qDebug()<<"y axis max range"<<customPlot->xAxis->range().upper;
//  if(customPlot->xAxis->range().upper<value){
//    customPlot->xAxis->setRange(0,value*1.1);
//  }
//  customPlot->xAxis->setTickStep( ((customPlot->xAxis->range().upper)/1.1)  /10);
//  //increase yAxis scale to fit new benchmark result
//  qDebug()<<"x axis max range"<<customPlot->yAxis->range().upper;
//  if(customPlot->yAxis->range().upper<barCounter){
//    customPlot->yAxis->setRange(0,barCounter);
//  }
  //  qDebug()<<"showResult";  

  QVector<double> currentTickVector = customPlot->yAxis->tickVector();
  QVector<QString> currentTickVectorLabels =  customPlot->yAxis->tickVectorLabels();

  double currentValue = value;
  double currentKey = currentTickVector.size();

  qDebug()<<"current key"<<currentKey;
  qDebug()<<"current value"<<currentValue;

  QCPBars *resultBar = new QCPBars(customPlot->yAxis, customPlot->xAxis);
  resultBar->addData(currentKey, currentValue);

  currentTickVector.append(currentKey);
  currentTickVectorLabels.append(benchmarkName);

  customPlot->yAxis->setTickVector(currentTickVector);
  customPlot->yAxis->setTickVectorLabels(currentTickVectorLabels);

  customPlot->addPlottable(resultBar);

  customPlot->rescaleAxes();

  QCPItemText *text = new QCPItemText(customPlot);
  customPlot->addItem(text);

  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(  currentValue , currentKey );
  text->setText(QString::number( currentValue ));

  //  text->setFont(QFont(font().family(), 12)); // make font a bit larger
  text->setPen(QPen(Qt::red)); // show black border around text

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

  //increase xAxis scale to fit new result, if necessary
  qDebug()<<"y axis max range"<<customPlot->xAxis->range().upper;
  if(customPlot->xAxis->range().upper<value){
    customPlot->xAxis->setRange(0,value*1.1);
    //    customPlot->xAxis->setTickStep( ((customPlot->xAxis->range().upper)/1.1)  /10);
  }



  //  QCPItemText *text = new QCPItemText(ui->basicFinalResultPlot);
  //  ui->basicFinalResultPlot->addItem(text);

  //  qDebug()<<"last key"<<currentKey;
  //  qDebug()<<"last value"<<currentValue;

  //  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  //  text->position->setType(QCPItemPosition::ptPlotCoords);
  //  text->position->setCoords(  currentValue , currentKey );
  //  text->setText(QString::number( currentValue ));

  //  //  text->setFont(QFont(font().family(), 12)); // make font a bit larger
  //  text->setPen(QPen(Qt::black)); // show black border around text

  QCPBars *resultBar = new QCPBars(customPlot->yAxis, customPlot->xAxis);
  resultBar->addData(currentData);
  customPlot->addPlottable(resultBar);

  QCPItemText *text = new QCPItemText(customPlot);
  customPlot->addItem(text);

  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(  currentData.value , currentData.key );
  text->setText(QString::number( currentData.value ));

  text->setPen(QPen(Qt::red)); // show black border around text

  //  qDebug()<<"setting name";
  //  qDebug()<<"setting data";
  //  resultBar->setData(ticks, barData );
  //  qDebug()<<"add set";

  //  double currentValue = barData.last();
  //  double currentKey = ticks.last();

  //  QCPItemText *text = new QCPItemText(ui->basicFinalResultPlot);
  //  ui->basicFinalResultPlot->addItem(text);





  //  qDebug()<<"setting name";
  //  qDebug()<<"setting data";
  //  resultBar->setData(ticks, barData );
  //  qDebug()<<"add set";

  //  double currentValue = barData.last();
  //  double currentKey = ticks.last();

  //  QCPItemText *text = new QCPItemText(ui->basicFinalResultPlot);
  //  ui->basicFinalResultPlot->addItem(text);

  //  qDebug()<<"last key"<<currentKey;
  //  qDebug()<<"last value"<<currentValue;

  //  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  //  text->position->setType(QCPItemPosition::ptPlotCoords);
  //  text->position->setCoords(  currentValue , currentKey );
  //  text->setText(QString::number( currentValue ));

  //  //  text->setFont(QFont(font().family(), 12)); // make font a bit larger
  //  text->setPen(QPen(Qt::black)); // show black border around text



  customPlot->replot();
}



MainWindow::~MainWindow()
{
  delete ui;
}

