#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  initQCustomPlotGraph();

  //  Benchmark_Sparse s; //working
  //  Benchmark_Copy s; //working
  //  Benchmark_Vector s; working
  //  Benchmark_Solver s; //working in debug only for Qt>=5
  //  Benchmark_Scheduler s; //working
  //  Benchmark_Blas3 s; //working (extremely slow)
  //  Benchmark_Qr s; //working (extremely slow in debug only)
  //  s.execute();

  //connect reset button
//  connect(ui->buttonRunBenchmark, SIGNAL(clicked()), this, SLOT(resetData()) );
  //run benchmark button clicked -> execute benchmark
  connect(ui->buttonRunBenchmark, SIGNAL(clicked()), this, SLOT(startBenchmarkExecution()) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString)), this, SLOT(updateBenchmarkUnitMeasure(QString)) );
  //received a benchmark result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString,double)), this, SLOT(parseBenchmarkResult(QString,double)) );

}

//execute the currently selected benchmark
void MainWindow::startBenchmarkExecution(){
  resetData();
  benchmarkController.executeSelectedBenchmark(ui->comboBox->currentText() );
}

//initialize the graph
void MainWindow::initQCustomPlotGraph(){
  ui->comboBox->addItem("Blas3");
  ui->comboBox->addItem("Copy");
  ui->comboBox->addItem("Scheduler");
  ui->comboBox->addItem("Solver");
  ui->comboBox->addItem("Sparse");
  ui->comboBox->addItem("Vector");
  ui->comboBox->addItem("Qr");

  barCounter = 1 ;

  ui->benchmarkGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

  ui->benchmarkGraph->xAxis->setRange(0,0.1);
  ui->benchmarkGraph->yAxis->setRange(0,0);
  ui->benchmarkGraph->axisRect()->setupFullAxesBox();

  ui->benchmarkGraph->plotLayout()->insertRow(0);
  ui->benchmarkGraph->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->benchmarkGraph, "QCustomPlot"));

  ui->benchmarkGraph->xAxis->setLabel("GB/s");
  ui->benchmarkGraph->yAxis->setLabel("BENCHMARK");
  ui->benchmarkGraph->legend->setVisible(false);

  ui->benchmarkGraph->yAxis->setAutoTicks(false);
  ui->benchmarkGraph->yAxis->setAutoTickLabels(false);
  ui->benchmarkGraph->yAxis->setSubTickCount(0);
  ui->benchmarkGraph->yAxis->setTickLength(0, 2);
  ui->benchmarkGraph->yAxis->grid()->setVisible(true);
  ui->benchmarkGraph->yAxis->setTickLabelRotation(0);


  QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->benchmarkGraph->legend->setFont(legendFont);
  ui->benchmarkGraph->legend->setSelectedFont(legendFont);
  ui->benchmarkGraph->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
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
  qDebug()<<"inside parseBenchmarkResults SLOT";
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

//graph the result
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

//predefined graph sample with random data
//unused
void MainWindow::graphData(){
  int n = 50; // number of points in graph
  double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
  double yOffset = (rand()/(double)RAND_MAX - 0.5)*5;
  double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
  QVector<double> x(n), y(n);
  for (int i=0; i<n; i++)
  {
    x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
    y[i] = (sin(x[i]*r1*5)*sin(cos(x[i]*r2)*r4*3)+r3*cos(sin(x[i])*r4*2))*yScale + yOffset;
  }

  ui->benchmarkGraph->addGraph();
  ui->benchmarkGraph->graph()->setName(QString("New graph %1").arg(ui->benchmarkGraph->graphCount()-1));
  ui->benchmarkGraph->graph()->setData(x, y);
  ui->benchmarkGraph->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
  if (rand()%100 > 75)
    ui->benchmarkGraph->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%9+1)));
  QPen graphPen;
  graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
  ui->benchmarkGraph->graph()->setPen(graphPen);
  ui->benchmarkGraph->replot();
}


MainWindow::~MainWindow()
{
  delete ui;
}

