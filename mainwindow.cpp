#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  //  initQChartGraph();


  initQCustomPlotGraph();

  //  Benchmark_Sparse s; //working in debug
  //  Benchmark_Copy s; //working
  //  Benchmark_Vector s; working
  //  Benchmark_Solver s; //working in debug
  //  Benchmark_Scheduler s; //working
  //  Benchmark_Blas3 s; //working (extremely slow)
//     Benchmark_Qr s; //testing
//    s.execute();

  //connect reset button
  connect(ui->buttonRunBenchmark, SIGNAL(clicked()), this, SLOT(resetData()) );
  //run benchmark button clicked -> execute benchmark
  connect(ui->buttonRunBenchmark, SIGNAL(clicked()), this, SLOT(startBenchmarkExecution()) );
  //got a benchmark result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString,double)), this, SLOT(parseBenchmarkResult(QString,double)) );

}

void MainWindow::startBenchmarkExecution(){/*
  //run benchmark button clicked -> execute benchmark
  connect(ui->buttonRunBenchmark, SIGNAL(clicked()), &benchmarkController, SLOT(execute()) );*/
  benchmarkController.executeSelectedBenchmark(ui->comboBox->currentText() );

}
void MainWindow::initQCustomPlotGraph(){
  ui->comboBox->addItem("Blas3");
  ui->comboBox->addItem("Copy");
  ui->comboBox->addItem("Scheduler");
  ui->comboBox->addItem("Solver");
  ui->comboBox->addItem("Sparse");
  ui->comboBox->addItem("Vector");

  barCounter = 1 ;

  ui->benchmarkGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

  ui->benchmarkGraph->xAxis->setRange(0,0.1);
  ui->benchmarkGraph->yAxis->setRange(0,0);
  ui->benchmarkGraph->axisRect()->setupFullAxesBox();

  ui->benchmarkGraph->plotLayout()->insertRow(0);
  ui->benchmarkGraph->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->benchmarkGraph, "QCustomPlot"));

  ui->benchmarkGraph->xAxis->setLabel("Gb/s");
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

//void MainWindow::initQChartGraph(){
//  QQuickView *view =  new QQuickView();

//  Benchmark_Copy *benchmarkCopyQML = new Benchmark_Copy(this);
//  //connect our benchmarkCopy variable with QML
//  view->engine()->rootContext()->setContextProperty("benchmarkCopy",benchmarkCopyQML);
//  //load our QML file
//  view->setSource(QUrl("qrc:/sourceFiles/main.qml") );

//  //add the loaded QML to an existing C++ GUI Widget
//  //    ui->qmlWidget = QWidget::createWindowContainer(view, static_cast<QWidget*>(ui->qmlWidget->parent()) );
//  ui->qmlWidget = QWidget::createWindowContainer(view, ui->tabQChart );

//  //    ui->qmlWidget->setMinimumSize(ui->qmlWidget->parentWidget()->size() );
//  ui->qmlWidget->setMinimumSize(view->size());

//get the root object//not necessary
//    QObject *rootObject = static_cast<QObject*>( view.rootObject());
//    QObject *qmlRoot = dynamic_cast<QObject*>(view.rootObject());

//}

void MainWindow::resetData()
{
  barCounter = 1;
  barData.clear();
  ticks.clear();
  labels.clear();
  ui->benchmarkGraph->clearGraphs();
  ui->benchmarkGraph->clearPlottables();
  ui->benchmarkGraph->clearItems();
  ui->benchmarkGraph->xAxis->setRange(0,001);
  ui->benchmarkGraph->yAxis->setTickVector(ticks);
  ui->benchmarkGraph->yAxis->setTickVectorLabels(labels);
  ui->benchmarkGraph->yAxis->setRange(0,001);
  ui->benchmarkGraph->replot();
}



void MainWindow::parseBenchmarkResult(QString benchmarkName, double bandwidthValue){
  qDebug()<<"inside parseBenchmarkResults SLOT";
  qDebug()<<"benchmarkName:"<<benchmarkName;
  qDebug()<<"bandwidthValue:"<<bandwidthValue;
  barData.append(bandwidthValue);
  ticks.append(barCounter++);
  labels.append(benchmarkName);
  showResult(bandwidthValue, ui->benchmarkGraph);
}

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

