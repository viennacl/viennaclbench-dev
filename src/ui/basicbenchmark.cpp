#include "basicbenchmark.h"
#include "ui_basicbenchmark.h"

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
BasicBenchmark::BasicBenchmark(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::BasicBenchmark)
{
  ui->setupUi(this);
  ui->basic_FinalResultLabel->hide();
  startBenchmarkButton = ui->basic_StartBenchmarkButton;
  stopBenchmarkButton = ui->basic_StopBenchmarkButton;
  progressBar = ui->basic_ProgressBar;
  benchmarkListWidget = ui->basic_BenchmarkListWidget;
  contextComboBox = ui->basic_contextComboBox;
  singlePrecisionButton = ui->basic_SingleButton;

  maximumBenchProgress = 0;
  currentBenchProgress = 0;
  connect(ui->basic_DoubleButton, SIGNAL(clicked()), this, SLOT(updateDoublePrecisionButtons()) );
  connect(ui->basic_SingleButton, SIGNAL(clicked()), this, SLOT(updateSinglePrecisionButtons()) );
  initBasic();
}

/*!
 * \brief Destructor
 */
BasicBenchmark::~BasicBenchmark()
{
  delete ui;
}

/*!
 * \brief Initializes all benchmark plots.
 */
void BasicBenchmark::initBasic(){
  basic_DetailedPlotTab = new QTabWidget(this);
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

    plot->setInteractions(QCP::iSelectPlottables | QCP::iSelectLegend);
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
  ui->basic_FinalResultPlot->setInteractions(QCP::iSelectPlottables);

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

/*!
 * \brief Shows the start button, hides the stop button, writes "Done" in progressbar.
 * Used after a benchmark seesion is completed.
 */
void BasicBenchmark::showBenchmarkStartButton(){
  ui->basic_StopBenchmarkButton->hide();
  ui->basic_StartBenchmarkButton->show();
  ui->basic_ProgressBar->setFormat("Done");
}

/*!
 * \brief Changes state of the precision buttons. Single precision - checked, double - unchecked.
 */
void BasicBenchmark::updateSinglePrecisionButtons(){
  ui->basic_SingleButton->setChecked(true);
  ui->basic_SingleButton->setIcon(QIcon(":/icons/icons/checkTrue.png"));

  ui->basic_DoubleButton->setChecked(false);
  ui->basic_DoubleButton->setIcon(QIcon(":/icons/icons/empty.png"));
}

/*!
 * \brief Changes state of the precision buttons. Double precision - checked, single - unchecked.
 */
void BasicBenchmark::updateDoublePrecisionButtons(){
  ui->basic_DoubleButton->setChecked(true);
  ui->basic_DoubleButton->setIcon(QIcon(":/icons/icons/checkTrue.png"));

  ui->basic_SingleButton->setChecked(false);
  ui->basic_SingleButton->setIcon(QIcon(":/icons/icons/empty.png"));
}

/*!
 * \brief Changes focus of detailed plots to the currently running benchmark.
 * \param benchmarkIdNumber Id number of the currently active benchmark
 */
void BasicBenchmark::setActiveBenchmarkPlot(int benchmarkIdNumber){
  basic_DetailedPlotTab->setCurrentIndex(benchmarkIdNumber);
  activeBenchmark = benchmarkIdNumber;
}

/*!
 * \brief Adds a final benchmark result to the final result plot.
 * \param benchmarkName Benchmark name
 * \param finalResult Benchmark result
 */
void BasicBenchmark::updateFinalResultPlot(QString benchmarkName, double finalResult){
  plotFinalResult(benchmarkName, finalResult, ui->basic_FinalResultPlot);
}

/*!
 * \brief Shows the appropriate detailed graph when a result was clicked on in the final plot.
 * \param plottable Plot result that was clicked
 */
void BasicBenchmark::graphClicked(QCPAbstractPlottable *plottable)
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

/*!
 * \brief Detailed plot selection event filter. Highlights a clicked plot on the legend and vice versa.
 */
void BasicBenchmark::selectionChanged(){
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

/*!
 * \brief Updates the progressbar.
 */
void BasicBenchmark::updateBenchProgress(){
  currentBenchProgress++;
  ui->basic_ProgressBar->setValue(currentBenchProgress);
  ui->basic_ProgressBar->setFormat("Running Test %v of %m");
}

/*!
 * \brief Handles list widget's selection events. Called whenever a list item was selected. Checks all items and updates their selected/deselected status appropriately.
 * \param item The item that was clicked.
 */
void BasicBenchmark::updateBenchmarkListWidget(QListWidgetItem *item)
{
  //item(0) is the 'All' benchmarks selection item
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

/*!
 * \brief Resets plot data and tick vectors of all plots.
 */
void BasicBenchmark::resetAllPlots(){
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

/*!
 * \brief Hides the stop button, shows the start button. Kinda unnecessary, but oh well.
 */
void BasicBenchmark::hideStopButton()
{
  ui->basic_StopBenchmarkButton->hide();
  ui->basic_StartBenchmarkButton->show();
}

/*!
 * \brief Resets plot data of a selected graph.
 * \param benchmarkGraph The graph to be reset
 */
void BasicBenchmark::resetPlotData(QCustomPlot *benchmarkGraph)
{
  benchmarkGraph->clearGraphs();
  benchmarkGraph->clearPlottables();
  benchmarkGraph->clearItems();
  benchmarkGraph->xAxis->setRange(0,1);
  benchmarkGraph->replot();
}

/*!
 * \brief Sets the unit measure for the currently active benchmark.
 * \param unitMeasureName Measure name
 * \param axis Axis on which to show the measure
 */
void BasicBenchmark::updateBenchmarkUnitMeasure(QString unitMeasureName, int axis)
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

/*!
 * \brief Parses the received benchmark result signal and displays it using the selected graph type.
 * \param benchmarkName Benchmark name
 * \param key Key value
 * \param resultValue Result value
 * \param graphType See \ref GraphType
 * \param testId Test id number
 */
void BasicBenchmark::parseBenchmarkResult(QString benchmarkName, double key, double resultValue, int graphType, int testId){
  if(graphType == BAR_GRAPH){
    plotBarResult(benchmarkName, key, resultValue, basic_DetailedPlotsVector[activeBenchmark]);
  }
  else if(graphType == LINE_GRAPH){
    plotLineResult(benchmarkName, key, resultValue, basic_DetailedPlotsVector[activeBenchmark], testId);
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
void BasicBenchmark::plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId){
  //add the legend if it doesnt exist already
  //make sure there is enough margin room for the legend to fit
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

/*!
 * \brief Draws received test results using a bar graph on the selected plot.
 * X and Y axis are swapped to achieve horizontal bar display
 * \param benchmarkName Benchmark test name
 * \param key Graph's position on Yaxis - not used. Y-axis positioning of graphs is calculated automatically to avoid wrong grpah placement.
 * \param value Result value
 * \param customPlot Plot on which the graph is to be drawn
 */
void BasicBenchmark::plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot){
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
void BasicBenchmark::plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot){
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
