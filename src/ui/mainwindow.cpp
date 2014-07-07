#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "qtcategorybutton.h"
#include <QTabWidget>

#include <cstdlib>

//include ViennaCL headers
#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->actionAbout,SIGNAL(triggered()), qApp, SLOT(aboutQt()) );

  {
    ui->collapseWidget->setText("Individual Test Results");
//    ui->collapseWidget->setChildWidget(new QCustomPlot(this));

    ui->collapseWidget->setChildWidget(new QTabWidget(this));
  }
  //normalize size of each list menu item
  for ( int i = 0; i < ui->menuListWidget->count(); i++ ) {
      ui->menuListWidget->item(i)->setSizeHint(ui->menuListWidget->itemSizeHint());
  }
  for ( int i = 0; i < ui->menuListWidget_2->count(); i++ ) {
      ui->menuListWidget_2->item(i)->setSizeHint(ui->menuListWidget_2->itemSizeHint());
  }
  //setup plot graph widget
  initBasicView();
  initExpertView();
  initHomeScreen();

//    Benchmark_Sparse s; //working
  //  Benchmark_Copy s; //working
//    Benchmark_Vector s; working
  //  Benchmark_Solver s; //working in debug only for Qt>=5
  //  Benchmark_Scheduler s; //working
    Benchmark_Blas3 s; //working (extremely slow)
  //  Benchmark_Qr s; //working (extremely slow in debug only)
    s.execute();


  //run benchmark button clicked -> execute benchmark
//  connect(ui->buttonRunBenchmark, SIGNAL(clicked()), this, SLOT(startBenchmarkExecution()) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString)), this, SLOT(updateBenchmarkUnitMeasure(QString)) );
  //received a benchmark result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString,double)), this, SLOT(parseBenchmarkResult(QString,double)) );

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
    QVector<QString> temp;
    temp.append("Vector");
    temp.append("Sparse");
    temp.append("Solver");
    temp.append("Qr");
    temp.append("Copy");
    temp.append("Blas3");

    QVector<double> ticksTemp;
    ticksTemp.append(1);
    ticksTemp.append(2);
    ticksTemp.append(3);
    ticksTemp.append(4);
    ticksTemp.append(5);
    ticksTemp.append(6);

    ui->benchmarkGraph->yAxis->setTickVectorLabels(temp);
    ui->benchmarkGraph->yAxis->setTickVector(ticksTemp);

    ui->benchmarkGraph->yAxis->setAutoTickLabels(false);
    ui->benchmarkGraph->yAxis->setAutoTicks(false);

    ui->benchmarkGraph->xAxis->setAutoTickLabels(false);
    ui->benchmarkGraph->xAxis->setAutoTicks(false);
    ui->benchmarkGraph->xAxis->setTickVector(ticksTemp);
    ui->benchmarkGraph->xAxis->setTickVectorLabels(temp);
    ui->benchmarkGraph->replot();

    barCounter = 1 ;

    ui->benchmarkGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

    ui->benchmarkGraph->xAxis->setRange(0,0.1);
    ui->benchmarkGraph->yAxis->setRange(0,0);
    ui->benchmarkGraph->axisRect()->setupFullAxesBox();

    ui->benchmarkGraph->xAxis->setLabel("GB/s");
    ui->benchmarkGraph->legend->setVisible(false);

    ui->benchmarkGraph->yAxis->setAutoTicks(false);
    ui->benchmarkGraph->yAxis->setAutoTickLabels(false);
    ui->benchmarkGraph->yAxis->setSubTickCount(0);
    ui->benchmarkGraph->yAxis->setTickLength(0, 2);
    ui->benchmarkGraph->yAxis->grid()->setVisible(true);
    ui->benchmarkGraph->yAxis->setTickLabelRotation(0);

    QColor backgroundColor(240,240,240);
    QBrush backgroundBrush(backgroundColor);
    ui->benchmarkGraph->setBackground(backgroundBrush);

    connect(ui->basicBenchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
    connect(ui->basicBenchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
    for ( int i = 0; i < ui->basicBenchmarkListWidget->count(); i++ ) {
      ui->basicBenchmarkListWidget->item(i)->setSelected(true);
    }
}



void MainWindow::initExpertView(){
}

//execute the currently selected benchmark
void MainWindow::startBenchmarkExecution(){
  resetData();
  //  benchmarkController.executeSelectedBenchmark(ui->comboBox->currentText() );
}

void MainWindow::updateBenchmarkListWidget(QListWidgetItem *item)
{
  if(ui->basicBenchmarkListWidget->row(item) == ALL){
    if(item->isSelected()){
      ui->basicBenchmarkListWidget->selectAll();
    }
    else{
      ui->basicBenchmarkListWidget->clearSelection();
    }
  }
  else{
    if(item->isSelected()){
      item->setIcon(QIcon(":/icons/icons/checkTrue.png"));
    }
    else{
      item->setIcon(QIcon(":/icons/icons/checkFalse.png"));
    }
  }
}

//reset the graph
void MainWindow::resetData()
{
  qDebug()<<"resetting data";
  barCounter = 1;
  barData.clear();
  ticks.clear();
  labels.clear();
  ui->benchmarkGraph->clearGraphs();
  ui->benchmarkGraph->clearPlottables();
  ui->benchmarkGraph->clearItems();
  ui->benchmarkGraph->xAxis->setRange(0,1);
//  ui->benchmarkGraph->yAxis->setTickVector(ticks);
//  ui->benchmarkGraph->yAxis->setTickVectorLabels(labels);
  ui->benchmarkGraph->yAxis->setRange(0,1);
  ui->benchmarkGraph->replot();
}

//parse the received benchmark result name and value
void MainWindow::parseBenchmarkResult(QString benchmarkName, double bandwidthValue){
//  qDebug()<<"inside parseBenchmarkResults SLOT";
  qDebug()<<"benchmarkName:"<<benchmarkName;
  qDebug()<<"bandwidthValue:"<<bandwidthValue;
  barData.append(bandwidthValue);
  ticks.append(barCounter++);
  labels.append(benchmarkName);
  showResult(bandwidthValue, ui->benchmarkGraph);
}

//set the benchmark's unit measure
void MainWindow::updateBenchmarkUnitMeasure(QString unitMeasureName)
{
  ui->benchmarkGraph->xAxis->setLabel(unitMeasureName);
}

//main result diplay function
//x and y axis are swapped to achieve horizontal bar display
void MainWindow::showResult(double value, QCustomPlot *customPlot){
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

  QCPItemText *text = new QCPItemText(ui->benchmarkGraph);
  ui->benchmarkGraph->addItem(text);

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

