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

  //setup benchmark plots
  initBasicView();
  initExpertView();
  initHomeScreen();

  //    Benchmark_Sparse s; //working
  //  Benchmark_Copy s; //working
  //    Benchmark_Vector s; working
  //  Benchmark_Solver s; //working
  //  Benchmark_Scheduler s; //working
  //    Benchmark_Blas3 s; //working
  //  Benchmark_Qr s; //working
  //    s.execute();


  //run benchmark button clicked -> execute benchmark
  connect(ui->basic_StartBenchmarkButton, SIGNAL(clicked()), this, SLOT(startBenchmarkExecution()) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString)), this, SLOT(updateBenchmarkUnitMeasure(QString)) );
  //received a benchmark result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString,double)), this, SLOT(parseBenchmarkResult(QString,double)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(finalResultSignal(QString, double)), this, SLOT(updateFinalResultPlot(QString,double)) );
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

  //  bool is_first_element = true;
  //  for(platforms_type::iterator platform_iter = platforms.begin(); platform_iter != platforms.end(); ++platform_iter){

  //    typedef std::vector<viennacl::ocl::device> devices_type;
  //    devices_type devices = platform_iter->devices(CL_DEVICE_TYPE_ALL);
  //    std::cout << "# Vendor and version: " << platform_iter->info() << std::endl;

  //    if (is_first_element)
  //    {
  //      std::cout << "# ViennaCL uses this OpenCL platform by default." << std::endl;
  //      is_first_element = false;
  //    }
  //    std::cout << "# Available Devices: " << std::endl;
  //    for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
  //    {
  //        std::cout << std::endl;
  //std::cout << " !!!!!!!!!!!" << std::endl;
  //        std::cout << "  -----------------------------------------" << std::endl;
  //        std::cout << iter->full_info();
  ////        std::cout << iter
  //        std::cout << "  -----------------------------------------" << std::endl;

  //        systemInfoString.append("Name: ");
  //        systemInfoString.append(QString::fromStdString(iter->name( )) );
  //        systemInfoString.append("\n");
  //    }

  //  }



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
  //normalize size of each list menu item
  for ( int i = 0; i < ui->menuListWidget->count(); i++ ) {
    ui->menuListWidget->item(i)->setSizeHint(ui->menuListWidget->itemSizeHint());
  }

  basic_DetailedPlotTab = new QTabWidget(ui->basic_BenchmarkTab);
  basic_DetailedPlotTab->setStyleSheet("QTabBar::tab{width: 75px;height: 25px;}");

  blas3_DetailedPlot = new QCustomPlot();
  copy_DetailedPlot = new QCustomPlot();
  qr_DetailedPlot = new QCustomPlot();
  solver_DetailedPlot = new QCustomPlot();
  sparse_DetailedPlot = new QCustomPlot();
  vector_DetailedPlot = new QCustomPlot();

  basic_DetailedPlotTab->addTab(blas3_DetailedPlot,"Blas3");
  basic_DetailedPlotTab->addTab(copy_DetailedPlot,"Copy");
  basic_DetailedPlotTab->addTab(qr_DetailedPlot,"Qr");
  basic_DetailedPlotTab->addTab(solver_DetailedPlot,"Solver");
  basic_DetailedPlotTab->addTab(sparse_DetailedPlot,"Sparse");
  basic_DetailedPlotTab->addTab(vector_DetailedPlot,"Vector");

  ui->basic_CollapseWidget->setChildWidget(basic_DetailedPlotTab);
  ui->basic_CollapseWidget->setText("Detailed Test Results");

  ui->basic_FinalResultPlot->axisRect()->setAutoMargins(QCP::msNone);
  ui->basic_FinalResultPlot->axisRect()->setMargins(QMargins( 100, 35, 0, 25 ));
  ui->basic_FinalResultPlot->axisRect()->setupFullAxesBox();
  ui->basic_FinalResultPlot->setInteractions(QCP::iSelectPlottables);
  ui->basic_FinalResultPlot->legend->setVisible(false);

  //  Plot mapping
  //  Vector - 1
  //  Sparse - 2
  //  Solver - 3
  //  Qr - 4
  //  Copy - 5
  //  Blas3 - 6

  QVector<QString> finalResultPlotLabels;
  finalResultPlotLabels.append("Vector - GFLOPs");
  finalResultPlotLabels.append("Sparse - GFLOPs");
  finalResultPlotLabels.append("Solver - GFLOPs");
  finalResultPlotLabels.append("Qr - GFLOPs");
  finalResultPlotLabels.append("Copy - GB/s");
  finalResultPlotLabels.append("Blas3 - GFLOPs");

  QVector<double> finalResultPlotTicks;
  finalResultPlotTicks.append(1);
  finalResultPlotTicks.append(2);
  finalResultPlotTicks.append(3);
  finalResultPlotTicks.append(4);
  finalResultPlotTicks.append(5);
  finalResultPlotTicks.append(6);

  //xAxis bottom
  //yAxis left
  //xAxis2 top
  //yAxis2 right

  //Disable secondary axes
  ui->basic_FinalResultPlot->yAxis2->setVisible(false);
  ui->basic_FinalResultPlot->xAxis2->setVisible(false);

  ui->basic_FinalResultPlot->yAxis->setAutoTickLabels(false);
  ui->basic_FinalResultPlot->yAxis->setAutoTicks(false);
  ui->basic_FinalResultPlot->yAxis->setTickVectorLabels(finalResultPlotLabels);
  ui->basic_FinalResultPlot->yAxis->setTickVector(finalResultPlotTicks);
  ui->basic_FinalResultPlot->yAxis->setSubTickCount( 0 );
  ui->basic_FinalResultPlot->yAxis->setTickLength( 0, 2);
  ui->basic_FinalResultPlot->yAxis->setRange( 0.5, 7.0);
  ui->basic_FinalResultPlot->yAxis->grid()->setVisible(true);
  ui->basic_FinalResultPlot->yAxis->setTickLabelRotation( 0 );

  ui->basic_FinalResultPlot->xAxis->setAutoTickLabels(true);
  ui->basic_FinalResultPlot->xAxis->setAutoTicks(true);
  ui->basic_FinalResultPlot->xAxis->setAutoTickStep(true);
  ui->basic_FinalResultPlot->xAxis->setRange(0,1);

  barCounter = 1 ;

  QColor backgroundColor(240,240,240);
  QBrush backgroundBrush(backgroundColor);
  ui->basic_FinalResultPlot->setBackground(backgroundBrush);

  ui->basic_FinalResultPlot->replot();

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
  resetData(ui->basic_FinalResultPlot);
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
  if(ui->basic_BenchmarkListWidget->row(item) == ALL){
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
      ui->basic_BenchmarkListWidget->item(ALL)->setSelected(false);
      ui->basic_BenchmarkListWidget->item(ALL)->setIcon(QIcon(":/icons/icons/checkFalse.png"));
      item->setIcon(QIcon(":/icons/icons/checkFalse.png"));
    }
  }
}

//reset the graph
void MainWindow::resetData(QCustomPlot *benchmarkGraph)
{
  barCounter = 1;
  barData.clear();
  ticks.clear();
  labels.clear();
  benchmarkGraph->clearGraphs();
  benchmarkGraph->clearPlottables();
  benchmarkGraph->clearItems();
  benchmarkGraph->xAxis->setRange(0,1);
  //  benchmarkGraph->yAxis->setTickVector(ticks);
  //  benchmarkGraph->yAxis->setTickVectorLabels(labels);
  //  benchmarkGraph->yAxis->setRange(0,1);
  benchmarkGraph->replot();
}

//parse the received benchmark result name and value
void MainWindow::parseBenchmarkResult(QString benchmarkName, double bandwidthValue){
  //  barData.append(bandwidthValue);
  //  ticks.append(barCounter++);
  //  labels.append(benchmarkName);
  //  showResult(bandwidthValue, ui->basic_FinalResultPlot);
}

//set the benchmark's unit measure
void MainWindow::updateBenchmarkUnitMeasure(QString unitMeasureName)
{
  //  ui->basicFinalResultPlot->xAxis->setLabel(unitMeasureName);
}


void MainWindow::plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot){
  //  Plot mapping
  //  Vector - 1
  //  Sparse - 2
  //  Solver - 3
  //  Qr - 4
  //  Copy - 5
  //  Blas3 - 6
  QCPBarData currentData;
  currentData.value = value;
  if(benchmarkName == "Blas3"){
    currentData.key = 6;
  }
  else if(benchmarkName == "Copy"){
    currentData.key = 5;
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



  //  qDebug()<<"showResult";
  QCPBars *resultBar = new QCPBars(customPlot->yAxis, customPlot->xAxis);
  resultBar->addData(currentData);
  //  qDebug()<<"initialized resultBar";
  customPlot->addPlottable(resultBar);

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

//main result diplay function
//x and y axis are swapped to achieve horizontal bar display
void MainWindow::plotResult(double value, QCustomPlot *customPlot){
  //  customPlot->yAxis->setAutoTicks(false);
  //  customPlot->yAxis->setAutoTickLabels(false);
  //  customPlot->yAxis->setTickLabelRotation(60);
  //  customPlot->yAxis->setTickVector(ticks);
  //  customPlot->yAxis->setTickVectorLabels(labels);
  //  customPlot->yAxis->setSubTickCount(0);
  //  customPlot->yAxis->setTickLength(0, 2);
  //  customPlot->yAxis->grid()->setVisible(true);
  //  customPlot->yAxis->setTickLabelRotation(0);


  //  ui->benchmarkGraph->yAxis->setTickVector(ticks);
  //  ui->benchmarkGraph->yAxis->setTickVectorLabels(labels);
  customPlot->yAxis->setTickVector(ticks);
  customPlot->yAxis->setTickVectorLabels(labels);
  customPlot->xAxis->setAutoTickStep(false);
  //increase xAxis scale to fit new result, if necessary
  qDebug()<<"y axis max range"<<customPlot->xAxis->range().upper;
  if(customPlot->xAxis->range().upper<value){
    customPlot->xAxis->setRange(0,value*1.1);
  }
  customPlot->xAxis->setTickStep( ((customPlot->xAxis->range().upper)/1.1)  /10);
  //increase yAxis scale to fit new benchmark result
  qDebug()<<"x axis max range"<<customPlot->yAxis->range().upper;
  if(customPlot->yAxis->range().upper<barCounter){
    customPlot->yAxis->setRange(0,barCounter);
  }

  //  qDebug()<<"showResult";
  QCPBars *resultBar = new QCPBars(customPlot->yAxis, customPlot->xAxis);
  //  qDebug()<<"initialized resultBar";
  customPlot->addPlottable(resultBar);
  //  qDebug()<<"setting name";
  //  qDebug()<<"setting data";
  resultBar->setData(ticks, barData );
  //  qDebug()<<"add set";

  double currentValue = barData.last();
  double currentKey = ticks.last();

  QCPItemText *text = new QCPItemText(ui->basic_FinalResultPlot);
  ui->basic_FinalResultPlot->addItem(text);

  qDebug()<<"last key"<<currentKey;
  qDebug()<<"last value"<<currentValue;

  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(  currentValue , currentKey );
  text->setText(QString::number( currentValue ));

  //  text->setFont(QFont(font().family(), 12)); // make font a bit larger
  text->setPen(QPen(Qt::black)); // show black border around text



  customPlot->replot();
}


MainWindow::~MainWindow()
{
  delete ui;
}

