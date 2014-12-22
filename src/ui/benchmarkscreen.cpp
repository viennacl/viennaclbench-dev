/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include "benchmarkscreen.h"
#include "ui_benchmarkscreen.h"

#include <QToolTip>

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
BenchmarkScreen::BenchmarkScreen(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::BenchmarkScreen)
{
  ui->setupUi(this);
  expertConfigPlaceholder = ui->expert_configBoxPlaceholder;

  startBenchmarkButton = ui->startBenchmarkButton;
  stopBenchmarkButton = ui->stopBenchmarkButton;
  progressBar = ui->progressBar;
  benchmarkListWidget = ui->benchmarkListWidget;
  contextComboBox = ui->contextComboBox;
  singlePrecisionButton = ui->singlePrecisionButton;

  maximumBenchProgress = 0;
  currentBenchProgress = 0;

  connect(ui->doublePrecisionButton, SIGNAL(clicked()), this, SLOT(updateDoublePrecisionButtons()) );
  connect(ui->singlePrecisionButton, SIGNAL(clicked()), this, SLOT(updateSinglePrecisionButtons()) );
  connect(ui->toggleFullscreenButton, SIGNAL(clicked()), this, SLOT(toggleFullscreenPlots()) );
  init();
}

/*!
 * \brief Destructor
 */
BenchmarkScreen::~BenchmarkScreen()
{
  delete ui;
}

/*!
 * \brief Initializes all benchmark plots.
 */
void BenchmarkScreen::init(){
  //  connect(ui->benchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), ui->benchmarkListWidget, SLOT() );
  //  connect(ui->benchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );

  QColor backgroundColor(240,240,240);
  QBrush backgroundBrush(backgroundColor);

  detailedPlotTab = ui->fullscreenPlotsWidget;
  detailedPlotTab->setStyleSheet("QTabBar::tab{width: 200px;height: 25px;}");

  blas3_DetailedPlot = new QCustomPlot();
  init_plot(blas3_DetailedPlot, 100, 10000, true, .01, 10000, true);
  copy_DetailedPlot = new QCustomPlot();
  init_plot(copy_DetailedPlot, 1000, 11000000, true, .01, 100, true);
  //  qr_DetailedPlot = new QCustomPlot();
  //  solver_DetailedPlot = new QCustomPlot();
  sparse_DetailedPlot = new QCustomPlot();
  sparse_DetailedPlot->setBackground(backgroundBrush);
  sparse_DetailedPlot->yAxis->setTickLength( 0, 2);
  sparse_DetailedPlot->yAxis->grid()->setVisible(true);
  sparse_DetailedPlot->yAxis->setTickLabelRotation( 0 );
  sparse_DetailedPlot->yAxis->setAutoSubTicks(false);
  sparse_DetailedPlot->yAxis->setAutoTickLabels(false);
  sparse_DetailedPlot->yAxis->setAutoTicks(false);
  sparse_DetailedPlot->yAxis->setAutoTickStep(false);
  QVector<double> emptyTickVector;
  sparse_DetailedPlot->yAxis->setTickVector(emptyTickVector);
  QVector<QString> emptyTickVectorLabels;
  sparse_DetailedPlot->yAxis->setTickVectorLabels(emptyTickVectorLabels);

  vector_DetailedPlot = new QCustomPlot();
  init_plot(vector_DetailedPlot, 100, 11000000, true, .01, 1000, true);

  detailedPlotsVector.insert(BLAS3, blas3_DetailedPlot);
  detailedPlotsVector.insert(COPY, copy_DetailedPlot);
  //  DetailedPlotsVector.insert(QR, qr_DetailedPlot);
  //  DetailedPlotsVector.insert(SOLVER, solver_DetailedPlot);
  detailedPlotsVector.insert(SPARSE, sparse_DetailedPlot);
  detailedPlotsVector.insert(VECTOR, vector_DetailedPlot);

  detailedPlotTab->insertTab(BLAS3, blas3_DetailedPlot,"Dense Matrix-Matrix-Products");
  detailedPlotTab->insertTab(COPY, copy_DetailedPlot,"Host-Device Copy");
  //  DetailedPlotTab->insertTab(QR, qr_DetailedPlot,"Qr");
  //  DetailedPlotTab->insertTab(SOLVER, solver_DetailedPlot,"Solver");
  detailedPlotTab->insertTab(SPARSE, sparse_DetailedPlot,"Sparse Matrix-Vector Product");
  detailedPlotTab->insertTab(VECTOR, vector_DetailedPlot,"Vector Operations");

  //xAxis bottom
  //yAxis left
  //xAxis2 top
  //yAxis2 right

  foreach(QCustomPlot* plot, detailedPlotsVector)
  {
    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(plot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

    //filter each item selection
    connect(plot, SIGNAL( selectionChangedByUser()), this, SLOT(selectionChanged()) );

    // display value of ticks
    connect(plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(showHoverPointToolTip(QMouseEvent*)));

    plot->replot();
  }


  //ui->finalResultPlot->axisRect()->setAutoMargins(QCP::msNone);
  //ui->finalResultPlot->axisRect()->setMargins(QMargins( 100, 15, 60, 40 ));
  ui->finalResultPlot->axisRect()->setupFullAxesBox();
  //Disable secondary axes & legend
  ui->finalResultPlot->yAxis2->setVisible(false);
  ui->finalResultPlot->xAxis2->setVisible(false);
  ui->finalResultPlot->legend->setVisible(false);
  //Enable selecting plots
  ui->finalResultPlot->setInteractions(QCP::iSelectPlottables);

  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->finalResultPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

  QVector<QString> finalResultPlotLabels;
  finalResultPlotLabels.append("Vector - GB/sec");
  finalResultPlotLabels.append("Sparse - GFLOPs");
  //  finalResultPlotLabels.append("Solver - GFLOPs");
  //  finalResultPlotLabels.append("Qr - GFLOPs");
  finalResultPlotLabels.append("Copy - GB/sec");
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

  ui->finalResultPlot->yAxis->setAutoTickLabels(false);
  ui->finalResultPlot->yAxis->setAutoTicks(false);
  ui->finalResultPlot->yAxis->setTickVectorLabels(finalResultPlotLabels);
  ui->finalResultPlot->yAxis->setTickVector(finalResultPlotTicks);
  ui->finalResultPlot->yAxis->setSubTickCount( 0 );
  ui->finalResultPlot->yAxis->setTickLength( 0, 2);
  ui->finalResultPlot->yAxis->setRange( 0.5, 5.0);
  ui->finalResultPlot->yAxis->grid()->setVisible(true);
  ui->finalResultPlot->yAxis->setTickLabelRotation( 0 );


  ui->finalResultPlot->xAxis->grid()->setSubGridVisible(true);
  ui->finalResultPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
  ui->finalResultPlot->xAxis->setScaleLogBase(10);
  ui->finalResultPlot->xAxis->setNumberFormat("gb"); // e = exponential, b = beautiful decimal powers
  ui->finalResultPlot->xAxis->setNumberPrecision(0);
  ui->finalResultPlot->xAxis->setRange( 0.1, 10000.0);
  ui->finalResultPlot->xAxis->setAutoTickStep(false);
  ui->finalResultPlot->xAxis->setSubTickCount(8);

  ui->finalResultPlot->setBackground(backgroundBrush);

  ui->stopBenchmarkButton->hide();

  connect(ui->benchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  connect(ui->benchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  for ( int i = 0; i < ui->benchmarkListWidget->count(); i++ ) {
    ui->benchmarkListWidget->item(i)->setSelected(true);
  }
}

/*!
 * \brief Shows the start button, hides the stop button, writes "Done" in progressbar.
 * Used after a benchmark seesion is completed.
 */
void BenchmarkScreen::showBenchmarkStartButton(){
  ui->stopBenchmarkButton->hide();
  ui->startBenchmarkButton->show();
  ui->progressBar->setFormat("Done");
  currentBenchProgress = 0;
}

/*!
 * \brief Changes state of the precision buttons. Single precision - checked, double - unchecked.
 */
void BenchmarkScreen::updateSinglePrecisionButtons(){
  ui->singlePrecisionButton->setChecked(true);
  ui->singlePrecisionButton->setIcon(QIcon(":/icons/icons/checkTrue.png"));

  ui->doublePrecisionButton->setChecked(false);
  ui->doublePrecisionButton->setIcon(QIcon(":/icons/icons/empty.png"));

}

/*!
 * \brief Changes state of the precision buttons. Double precision - checked, single - unchecked.
 */
void BenchmarkScreen::updateDoublePrecisionButtons(){
  ui->doublePrecisionButton->setChecked(true);
  ui->doublePrecisionButton->setIcon(QIcon(":/icons/icons/checkTrue.png"));

  ui->singlePrecisionButton->setChecked(false);
  ui->singlePrecisionButton->setIcon(QIcon(":/icons/icons/empty.png"));

}

/*!
 * \brief Changes focus of detailed plots to the currently running benchmark.
 * \param benchmarkIdNumber Id number of the currently active benchmark
 */
void BenchmarkScreen::setActiveBenchmarkPlot(int benchmarkIdNumber){
  detailedPlotTab->setCurrentIndex(benchmarkIdNumber);
  activeBenchmark = benchmarkIdNumber;
}

/*!
 * \brief Adds a final benchmark result to the final result plot.
 * \param benchmarkName Benchmark name
 * \param finalResult Benchmark result
 */
void BenchmarkScreen::updateFinalResultPlot(QString benchmarkName, double finalResult){
  plotFinalResult(benchmarkName, finalResult, ui->finalResultPlot);
}

/*!
 * \brief Shows the appropriate detailed graph when a result was clicked on in the final plot.
 * \param plottable Plot result that was clicked
 */
void BenchmarkScreen::graphClicked(QCPAbstractPlottable *plottable)
{
  QString clickedBenchmarkBar = plottable->name();
  if(clickedBenchmarkBar == "Blas3"){
    detailedPlotTab->setCurrentIndex(BLAS3);
  }
  else if(clickedBenchmarkBar == "Copy"){
    detailedPlotTab->setCurrentIndex(COPY);
  }
  else if(clickedBenchmarkBar == "Qr"){
    detailedPlotTab->setCurrentIndex(QR);
  }
  else if(clickedBenchmarkBar == "Scheduler"){
    detailedPlotTab->setCurrentIndex(SCHEDULER);
  }
  else if(clickedBenchmarkBar == "Solver"){
    detailedPlotTab->setCurrentIndex(SOLVER);
  }
  else if(clickedBenchmarkBar == "Sparse"){
    detailedPlotTab->setCurrentIndex(SPARSE);
  }
  else if(clickedBenchmarkBar == "Vector"){
    detailedPlotTab->setCurrentIndex(VECTOR);
  }
}

/*!
 * \brief Detailed plot selection event filter. Highlights a clicked plot on the legend and vice versa.
 */
void BenchmarkScreen::selectionChanged()
{
  int currentPlotIndex = detailedPlotTab->currentIndex();
  QCustomPlot *currentPlot = detailedPlotsVector[currentPlotIndex];
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

/*!
 * \brief Draws a tooltip over a hovered graph point, showing the data values at that point
 */
void BenchmarkScreen::showHoverPointToolTip(QMouseEvent *event)
{
  int currentPlotIndex = detailedPlotTab->currentIndex();
  QCustomPlot *currentPlot = detailedPlotsVector[currentPlotIndex];
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
  QCPAbstractPlottable *plottable = currentPlot->plottableAt(event->posF());//get the plottable object under the mouse
#else
  QCPAbstractPlottable *plottable = currentPlot->plottableAt(event->localPos());//get the plottable object under the mouse
#endif

  if(plottable)
  {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    double x = currentPlot->xAxis->pixelToCoord(event->posF().x());
#else
    double x = currentPlot->xAxis->pixelToCoord(event->localPos().x());
#endif

    QCPGraph *graph = qobject_cast<QCPGraph*>(plottable);//we want a graph, not a bar

    if(graph)
    {
      double key = 0;
      double value = 0;

      QList<double> graphKeys = graph->data()->keys();//get all the keys (x values of all points) of the hovered graph

      bool ok = false;
      double closestKey = 0;
      double closestDistance = std::numeric_limits<double>::max();

      //iterate through all points and find the one that's closest to our mouse
      //we are looking at a single graph,
      //thus we only need to look at the x-axis values and calculate their distance to our mouse cursor x-axis value;
      for(int i = 0; i < graphKeys.length(); i++){
        double distance = qAbs(graphKeys[i] - x);
        if(distance < closestDistance){
          closestDistance = distance;
          closestKey = graphKeys[i];
          ok = true;
        }
      }

      if(ok)
      {
        //a valid graph point was found
        key = closestKey;
        value = graph->data()->value(key).value;//get the y-axis value

        QToolTip::showText(event->globalPos(),
                           tr("<table>"
                              "<tr><th colspan=\"2\">%L1</th></tr>"
                              "<tr><td>%L2:</td><td>%L3</td></tr>"
                              "<tr><td>%L4:</td>" "<td>%L5</td></tr>"
                              "</table>").
                           arg(graph->name().isEmpty() ? "..." : graph->name()).
                           arg(graph->keyAxis()->label()).
                           arg(key).
                           arg(graph->valueAxis()->label()).
                           arg(value),
                           currentPlot, this->rect());

      }//end if(ok)

    }//end if(graph)
  }//end if(plottable)
}

/*!
 * \brief Draws a tooltip onto a plot, highlighting the respective data set
 */
void BenchmarkScreen::showPointToolTip(QMouseEvent *event)
{
  int currentPlotIndex = detailedPlotTab->currentIndex();
  QCustomPlot *currentPlot = detailedPlotsVector[currentPlotIndex];

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
  double x = currentPlot->xAxis->pixelToCoord(event->posF().x());
  double y = currentPlot->yAxis->pixelToCoord(event->posF().y());
#else
  double x = currentPlot->xAxis->pixelToCoord(event->localPos().x());
  double y = currentPlot->yAxis->pixelToCoord(event->localPos().y());
#endif

  QCPGraph *closestGraph = NULL;
  double key = 0;
  double value = 0;
  double min_distance = std::numeric_limits<double>::max();

  for (int i=0; i < currentPlot->graphCount(); ++i)
  {
    QCPGraph *graph = currentPlot->graph(i);

    foreach(QCPData data, graph->data()->values())
    {
      double current_distance = qAbs(x - data.key) + qAbs(y - data.value);

      if (current_distance < min_distance && qAbs(x - data.key) / qAbs(data.key) < 0.5)
      {
        closestGraph = graph;
        min_distance = current_distance;
        key   = data.key;
        value = data.value;
      }
    }
  }

  if (closestGraph)
  {
    QToolTip::hideText();
    QToolTip::showText(event->globalPos(),
                       tr("<table>"
                            "<tr><th colspan=\"2\">%L1</th></tr>"
                            "<tr><td>%L2:</td><td>%L3</td></tr>"
                            "<tr><td>%L4:</td>" "<td>%L5</td></tr>"
                          "</table>").
                          arg(closestGraph->name().isEmpty() ? "..." : closestGraph->name()).
                          arg(closestGraph->keyAxis()->label()).
                          arg(key).
                          arg(closestGraph->valueAxis()->label()).
                          arg(value),
                       this, this->rect());
  }
}

/*!
 * \brief Updates the progressbar.
 */
void BenchmarkScreen::updateBenchProgress(){
  currentBenchProgress++;
  ui->progressBar->setValue(currentBenchProgress);
  ui->progressBar->setFormat("Running Benchmark %v of %m");
}

/*!
 * \brief Handles list widget's selection events. Called whenever a list item was selected. Checks all items and updates their selected/deselected status appropriately.
 * \param item The item that was clicked.
 */
void BenchmarkScreen::updateBenchmarkListWidget(QListWidgetItem *item)
{
  //item(0) is the 'All' benchmarks selection option
  if(ui->benchmarkListWidget->row(item) == 0){
    if(item->isSelected()){
      ui->benchmarkListWidget->selectAllItems();
    }
    else{
      ui->benchmarkListWidget->deselectAllItems();
    }
  }
  else{
    if(item->isSelected()){
      item->setIcon(QIcon(":/icons/icons/checkTrue.png"));
      ui->benchmarkListWidget->checkSelectedItems();
    }
    else{
      ui->benchmarkListWidget->item(0)->setSelected(false);
      ui->benchmarkListWidget->item(0)->setIcon(QIcon(":/icons/icons/checkFalse.png"));
      item->setIcon(QIcon(":/icons/icons/checkFalse.png"));
    }
  }
}

/*!
 * \brief Resets plot data and tick vectors of all plots.
 */
void BenchmarkScreen::resetAllPlots(){
  resetPlotData(ui->finalResultPlot);
  //reset all plots
  foreach(QCustomPlot* plot, detailedPlotsVector){
    resetPlotData(plot);
    plot->yAxis->setTickVector(QVector<double>() ); // for BarPlots
    plot->yAxis->setTickVectorLabels(QVector<QString>() ); // for BarPlots
    plot->replot();
  }
}

/*!
 * \brief Determines if there is enough available video memory to complete the benchmark with selected settings.
 * \return Returns true if there is enough memory, false if there isn't.
 */
bool BenchmarkScreen::estimateRequiredVideoMemory(){
//  std::cout << "current device max video memory: " << viennacl::ocl::current_context().current_device().global_mem_size() << std::endl;
  cl_ulong videoMemory = viennacl::ocl::current_context().current_device().global_mem_size();
  //TODO calculate memory requirements for each benchmark
  return true;
}

/*!
 * \brief Hides the stop button, shows the start button. Kinda unnecessary, but oh well.
 */
void BenchmarkScreen::hideStopButton()
{
  ui->stopBenchmarkButton->hide();
  ui->startBenchmarkButton->show();
}

/*!
 * \brief Resets plot data of a selected graph.
 * \param benchmarkGraph The graph to be reset
 */
void BenchmarkScreen::resetPlotData(QCustomPlot *benchmarkGraph)
{
  benchmarkGraph->clearGraphs();
  benchmarkGraph->clearPlottables();
  benchmarkGraph->clearItems();
  benchmarkGraph->replot();
}

/*!
 * \brief Sets the unit measure for the currently active benchmark.
 * \param unitMeasureName Measure name
 * \param axis Axis on which to show the measure
 */
void BenchmarkScreen::updateBenchmarkUnitMeasure(QString unitMeasureName, int axis)
{
  switch(axis){
  case Qt::XAxis:
    qDebug()<<""<<unitMeasureName;
    detailedPlotsVector[activeBenchmark]->xAxis->setLabel(unitMeasureName);
    break;
  case Qt::YAxis:
    qDebug()<<""<<unitMeasureName;
    detailedPlotsVector[activeBenchmark]->yAxis->setLabel(unitMeasureName);
    break;
  default:
    break;
  }

}

/*!
 * \brief Parses the received benchmark result signal and displays it using the selected graph type.
 * \param benchmarkName Benchmark name
 * \param key Key value
 * \param resultValue Result value
 * \param graphType See \ref GraphType
 * \param testId Test id number
 */
void BenchmarkScreen::parseBenchmarkResult(QString benchmarkName, double key, double resultValue, int graphType, int testId){
  if(graphType == BAR_GRAPH){
    plotBarResult(benchmarkName, key, resultValue, detailedPlotsVector[activeBenchmark]);
  }
  else if(graphType == LINE_GRAPH){
    plotLineResult(benchmarkName, key, resultValue, detailedPlotsVector[activeBenchmark], testId);
  }
}

/*!
 * \brief Draws received test results using a line graph on the selected plot.
 * Adds a legend entry for each result graph.
 * \param benchmarkName Benchmark test name
 * \param key Key value on the x-axis (e.g. vector size)
 * \param value Result value on the y-axis (e.g. the actual result for a given vector size)
 * \param customPlot Which plot is to be used to draw the graph
 * \param testId Id of the graph to be plotted, used to give different colors to graphs.
 */
void BenchmarkScreen::plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId){
  if(customPlot->legend->visible() == false){
    customPlot->plotLayout()->addElement(0,1, customPlot->legend);
    customPlot->legend->setVisible(true);
    customPlot->legend->setSelectableParts( QCPLegend::spItems );
    customPlot->legend->setMaximumSize( 110, QWIDGETSIZE_MAX );
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setRowSpacing(-3);
    QColor backgroundColor(240,240,240);
    QBrush backgroundBrush(backgroundColor);
    customPlot->legend->setBrush(backgroundBrush);
  }

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
  pen.setWidth(2);
  double scatter_size = 10;
  switch(testId){
  case 0:
    pen.setColor("black");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, scatter_size));
    break;
  case 1:
    pen.setColor("red");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, scatter_size));
    break;
  case 2:
    pen.setColor("green");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, scatter_size));
    break;
  case 3:
    pen.setColor("orange");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle, scatter_size));
    break;
  case 4:
    pen.setColor("magenta");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, scatter_size));
    break;
  case 5:
    pen.setColor("darkCyan");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, scatter_size));
    break;
  case 6:
    pen.setColor("brown");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, scatter_size));
    break;
  case 7:
    pen.setColor("yellow");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangleInverted, scatter_size));
    break;
  case 8:
    pen.setColor("pink");
    currentResultGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, scatter_size));
    break;
  default: pen.setColor("cyan");
  }
  currentResultGraph->setName(benchmarkName);
  currentResultGraph->addData( key, value );
  currentResultGraph->setPen(pen);
  currentResultGraph->setLineStyle(QCPGraph::lsLine);

  customPlot->replot();
}

/*!
 * \brief Draws received test results using a bar graph on the selected plot.
 * X and Y axis are swapped to achieve horizontal bar display
 * \param benchmarkName Benchmark test name
 * \param key Graph's position on Yaxis - not used. Y-axis positioning of graphs is calculated automatically to avoid wrong grpah placement.
 * \param value Result value
 * \param customPlot Plot on which the graph is to be drawn
 */
void BenchmarkScreen::plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot){

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

  customPlot->yAxis->setRangeLower(-0.5);
  customPlot->replot();
}

/*!
 * \brief Draws a final benchmark result using a bar graph on the final result plot.
 * \param benchmarkName Benchmark name
 * \param value Result value
 * \param customPlot Plot on which to draw the graph
 */
void BenchmarkScreen::plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot){
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

  //customPlot->xAxis->rescale();
  customPlot->replot();
}

/*!
 * \brief Toggles upper benchmark screen UI visibility. Used to make th detailed plot graphs fullscreen.
 */
void BenchmarkScreen::toggleFullscreenPlots(){
  if(ui->upperContainer->isVisible()){
    ui->upperContainer->hide();
  }
  else{
    ui->upperContainer->show();
  }
}

/*!
 * \brief Selects only sparse benchmark.
 */
void BenchmarkScreen::selectSparseBenchmark(){
  ui->benchmarkListWidget->deselectAllItems();
  ui->benchmarkListWidget->setSelected(3);//only run the sparse benchmark
}
