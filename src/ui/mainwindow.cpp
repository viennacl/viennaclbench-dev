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

  //setup benchmark plots
  initHomeScreen();
  initBasicView();
  initExpertView();
  initMatrixMarket();
  initSystemInfo();


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
  //stop benchmark button
  connect(ui->basic_StopBenchmarkButton, SIGNAL(clicked()), this, SLOT(stopBenchmarkExecution()) );

  //route incoming benchmark result info to appropriate plots
  connect(&benchmarkController, SIGNAL(benchmarkStarted(int)), this, SLOT(setActiveBenchmarkPlot(int)) );
  //set the benchmark result unit measure(GB/s, GFLOPs, seconds...)
  connect(&benchmarkController, SIGNAL(unitMeasureSignal(QString)), this, SLOT(updateBenchmarkUnitMeasure(QString)) );
  //received a benchmark result -> parse it and show it on the graph
  connect(&benchmarkController, SIGNAL(resultSignal(QString,double)), this, SLOT(parseBenchmarkResult(QString,double)) );
  //final benchmark result
  connect(&benchmarkController, SIGNAL(finalResultSignal(QString, double)), this, SLOT(updateFinalResultPlot(QString,double)) );
  //show the start button once all benchmarks are done
  connect(&benchmarkController, SIGNAL(emptyBenchmarkQ()), this, SLOT(showBenchmarkStartButton()) );

  connect(ui->basic_DoubleButton, SIGNAL(clicked()), this, SLOT(updateDoublePrecisionButtons()) );
  connect(ui->basic_SingleButton, SIGNAL(clicked()), this, SLOT(updateSinglePrecisionButtons()) );

}

void MainWindow::showBenchmarkStartButton(){
  ui->basic_StopBenchmarkButton->hide();
  ui->basic_StartBenchmarkButton->show();
}

void MainWindow::addInfoItem(int row, int col, QTableWidgetItem *item){
  //  ui->systemInfo_TableWidget->setItem(row, col, item);
}

void MainWindow::initSystemInfo(){


  // USING TABLEWIDGET
#ifdef VIENNACL_WITH_OPENCL
  QHBoxLayout *systemInfoLayout = new QHBoxLayout();

  typedef std::vector< viennacl::ocl::platform > platforms_type;
  platforms_type platforms = viennacl::ocl::get_platforms();
  bool is_first_element = true;

  //---PLATFORMS---
  QVBoxLayout *platformsLayout = new QVBoxLayout();
  for(platforms_type::iterator platform_iter = platforms.begin(); platform_iter != platforms.end(); ++platform_iter){
    QGroupBox *platformBox = new QGroupBox(QString::fromStdString(platform_iter->info()) );

    typedef std::vector<viennacl::ocl::device> devices_type;
    devices_type devices = platform_iter->devices(CL_DEVICE_TYPE_ALL);

    if (is_first_element)
    {
      //      std::cout << "# ViennaCL uses this OpenCL platform by default." << std::endl;
      platformBox->setTitle(platformBox->title()+" (default)");
      is_first_element = false;
    }
    //---DEVICES---
    int deviceCounter = 0;
    QVBoxLayout *devicesLayout = new QVBoxLayout();
    QSplitter *platformSplitter = new QSplitter();
    platformSplitter->setOrientation(Qt::Vertical);
    for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
    {
      //      devicesLayout->addWidget(new QLabel (QString("---Device #" + QString::number(++deviceCounter) )) );

      QTableWidget *deviceInfoTable = new QTableWidget();
      deviceInfoTable->setColumnCount(2);
      deviceInfoTable->setRowCount(60);
      deviceInfoTable->verticalHeader()->hide();
      deviceInfoTable->horizontalHeader()->hide();


      int row = 0;//row index counter
      int cProp = 0;//column property index
      int cVal = 1;//column value index

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Name:                          ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->name()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Address Bits:                  ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->address_bits()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Available:                     ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->available()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Compiler Available:            ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->compiler_available()) ) );
#ifdef CL_DEVICE_DOUBLE_FP_CONFIG

      deviceInfoTable->setItem(row, columnProperty, new QTableWidgetItem( QString("Double FP Config:              ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->fp_config_to_string(iter->double_fp_config())) ) );
#endif

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Endian Little:                 ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->endian_little()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Error Correction Support:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->error_correction_support()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Execution Capabilities:        ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->exec_capabilities_to_string(iter->execution_capabilities())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Extensions:                    ")) );
      QString extensions = QString::fromStdString( iter->extensions());
      extensions.replace(" ","\n");
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( extensions ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Global Mem Cache Size:         ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->global_mem_cache_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Global Mem Cache Type:         ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->mem_cache_type_to_string(iter->global_mem_cache_type())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Global Mem Cacheline Size:     ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->global_mem_cacheline_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Global Mem Size:               ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->global_mem_size() ) + QString( " Bytes") ) );
#ifdef CL_DEVICE_HALF_FP_CONFIG

      deviceInfoTable->setItem(row, columnProperty, new QTableWidgetItem( QString("Half PF Config:                ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->fp_config_to_string(iter->half_fp_config())) ) );
#endif
#ifdef CL_DEVICE_HOST_UNIFIED_MEMORY

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Host Unified Memory:           ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->host_unified_memory()) ) );
#endif

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Image Support:                 ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->image_support()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Image2D Max Height:            ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->image2d_max_height()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Image2D Max Width:             ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->image2d_max_width()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Image3D Max Depth:             ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->image3d_max_depth()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Image3D Max Height:            ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->image3d_max_height()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Image3D Max Width:             ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->image3d_max_width()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Local Mem Size:                ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->local_mem_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Local Mem Type:                ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->local_mem_type_to_string(iter->local_mem_type())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Clock Frequency:           ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_clock_frequency() ) + QString( " MHz") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Compute Units:             ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_compute_units()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Constant Args:             ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_constant_args()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Constant Buffer Size:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_constant_buffer_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Mem Alloc Size:            ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_mem_alloc_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Parameter Size:            ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_parameter_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Read Image Args:           ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_read_image_args()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Samplers:                  ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_samplers()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Work Group Size:           ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_work_group_size()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Work Item Dimensions:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_work_item_dimensions()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Work Item Sizes:           ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->convert_to_string(iter->max_work_item_sizes())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Max Write Image Args:          ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->max_write_image_args()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Mem Base Addr Align:           ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->mem_base_addr_align()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Min Data Type Align Size:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->min_data_type_align_size() ) + QString( " Bytes") ) );

#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width char:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_char()) ) );
#endif
#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width short:     ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_short()) ) );
#endif
#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_INT

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width int:       ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_int()) ) );
#endif
#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width long:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_long()) ) );
#endif
#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width float:     ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_float()) ) );
#endif
#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width double:    ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_double()) ) );
#endif
#ifdef CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Native Vector Width half:      ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->native_vector_width_half()) ) );
#endif
#ifdef CL_DEVICE_OPENCL_C_VERSION

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("OpenCL C Version:              ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->opencl_c_version()) ) );
#endif

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Platform:                      ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->platform()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width char:   ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_char()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width short:  ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_short()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width int:    ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_int()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width long:   ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_long()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width float:  ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_float()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width double: ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_double()) ) );
#ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Preferred Vector Width half:   ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->preferred_vector_width_half()) ) );
#endif

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Profile:                       ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->profile()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Profiling Timer Resolution:    ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->profiling_timer_resolution() ) + QString( " ns") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Queue Properties:              ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->queue_properties_to_string(iter->queue_properties())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Single FP Config:              ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->fp_config_to_string(iter->single_fp_config())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Type:                          ")) );
      row++;
      //    deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->device_type_to_string(iter->type())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Vendor:                        ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->vendor()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Vendor ID:                     ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->vendor_id()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Version:                       ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->version()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Driver Version:                ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->driver_version()) ) );
      qDebug()<<"total rows" <<row;
      //      deviceInfoTable->setRowHeight(5, 200);
      deviceInfoTable->setContextMenuPolicy(Qt::NoContextMenu);
      deviceInfoTable->setSelectionBehavior(QAbstractItemView::SelectItems);
      deviceInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);
      deviceInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
      deviceInfoTable->horizontalHeader()->setStretchLastSection(true);
      deviceInfoTable->resizeColumnsToContents();
      deviceInfoTable->resizeRowsToContents();
      deviceInfoTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

      //      devicesLayout->addWidget( deviceInfoTable );
      platformSplitter->addWidget(deviceInfoTable);

    }//---DEVICES---END
    deviceCounter = 0;
    //    devicesLayout->insertStretch(-1,1); //add a spacer at the end
    //    platformSplitter->setLayout(devicesLayout);
    //    platformBox->setLayout(devicesLayout);
    //    platformSplitter->addWidget(platformBox);
    systemInfoLayout->addWidget(platformSplitter);

  }//---PLATFORMS---END
  delete ui->systemInfo_Box->layout();
  ui->systemInfo_Box->setLayout(systemInfoLayout);
#endif

}//END initSystemInfo()

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
  //  QThread *workerThread = new QThread();
  //  ui->webView->moveToThread(workerThread);
  //  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()) );
  //  workerThread->start();
  //enable cache
  QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled, true);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
  ui->matrixMarket_Widget->webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
  //  ui->matrixMarket_Widget->webView->settings()->enablePersistentStorage(QDir::homePath());
  ui->matrixMarket_Widget->webView->settings()->enablePersistentStorage(QDir::currentPath());
  qDebug()<<"Saving web cache in: "<<QDir::currentPath();
  ui->matrixMarket_Widget->webView->settings()->setMaximumPagesInCache(10);
  //web page with all matrices contains around 2700 matrices...
  //needs MOAR cache
  ui->matrixMarket_Widget->webView->settings()->setOfflineWebApplicationCacheQuota(22111000);
  //lead the matrix market web page
  //  ui->matrixMarket_Widget->webView->load(QUrl("http://www.cise.ufl.edu/research/sparse/matrices/"));
  ui->matrixMarket_Widget->webView->load(QUrl("qrc:///mmFiles/matrixmarket/index.html"));
  //    ui->matrixMarket_Widget->webView->load(QUrl("http://localhost/MatrixMarket/index.html"));

  //  connect(ui->matrixMarket_Widget->webView, SIGNAL( loadFinished(bool)), this, SLOT(modifyMatrixMarketWeb()) );
  connect(ui->matrixMarket_Widget->webView, SIGNAL(loadProgress(int)), this, SLOT(modifyMatrixMarketWeb()) );
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

void MainWindow::initHomeScreen(){
#ifdef VIENNACL_WITH_OPENCL
  QHBoxLayout *systemInfoLayout = new QHBoxLayout();

  typedef std::vector< viennacl::ocl::platform > platforms_type;
  platforms_type platforms = viennacl::ocl::get_platforms();
  bool is_first_element = true;

  //---PLATFORMS---
  for(platforms_type::iterator platform_iter = platforms.begin(); platform_iter != platforms.end(); ++platform_iter){
    QVBoxLayout *platformLayout = new QVBoxLayout();


    QString platformName( QString::fromStdString(platform_iter->info()) );
    QPixmap *platformIcon;

    if(platformName.contains("Advanced Micro Devices")){
      //set AMD icon
      platformIcon = new QPixmap( QString(":/icons/icons/amdLogoGreen.png") );
    }
    else if(platformName.contains("Intel(R) Corporation")){
      //set Intel icon
      platformIcon = new QPixmap( QString(":/icons/icons/intelLogo.png") );
    }
    else{
      //set nVidia icon
      platformIcon = new QPixmap( QString(":/icons/icons/nvidiaLogoBlack.png") );
    }

    QLabel *platformIconLabel = new QLabel();
    platformIconLabel->setAlignment(Qt::AlignHCenter);
    platformIconLabel->setPixmap( platformIcon->scaledToHeight(50, Qt::SmoothTransformation) );

    platformLayout->addWidget( platformIconLabel );

    QGroupBox *platformBox = new QGroupBox( platformName );

    typedef std::vector<viennacl::ocl::device> devices_type;
    devices_type devices = platform_iter->devices(CL_DEVICE_TYPE_ALL);

    if (is_first_element)
    {
      std::cout << "# ViennaCL uses this OpenCL platform by default." << std::endl;
      platformBox->setTitle(platformBox->title()+" (default)");
      is_first_element = false;
    }
    //---DEVICES---
    int deviceCounter = 0;
    QVBoxLayout *devicesLayout = new QVBoxLayout();
    for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
    {
      devicesLayout->addWidget(new QLabel (QString("---Device #" + QString::number(++deviceCounter) )) );

      QString typeString;
      typeString.append("Type: ");

      cl_device_type localDeviceType = iter->type();
      if(localDeviceType & CL_DEVICE_TYPE_GPU){
        typeString.append("GPU");
      }
      else if(localDeviceType & CL_DEVICE_TYPE_CPU){
        typeString.append("CPU");
      }
      else if(localDeviceType & CL_DEVICE_TYPE_ACCELERATOR){
        typeString.append("Accelerator");
      }
      else if(localDeviceType & CL_DEVICE_TYPE_DEFAULT){
        typeString.append("(default)");
      }
      devicesLayout->addWidget(new QLabel(typeString) );

      QString nameString;
      nameString.append("Name: " + QString::fromStdString(iter->name()) );
      devicesLayout->addWidget(new QLabel( nameString ) );

      QString vendorString;
      vendorString.append("Vendor: " + QString::fromStdString(iter->vendor()) );
      devicesLayout->addWidget(new QLabel( vendorString ) );

      QString memoryString;
      memoryString.append("Global Memory Size: " + QString::number( ((uint64_t)iter->global_mem_size()/(1024*1024)) ) + " MB" );
      devicesLayout->addWidget(new QLabel( memoryString ) );

      QString clockString;
      clockString.append("Clock Frequency: " + QString::number(iter->max_clock_frequency()) + " MHz" );
      devicesLayout->addWidget(new QLabel( clockString ) );

#ifdef CL_DEVICE_OPENCL_C_VERSION
      QString openclCString;
      openclCString.append("OpenCL C Version: " + QString::fromStdString(iter->opencl_c_version() ) );
      devicesLayout->addWidget(new QLabel( openclCString ) );
#endif

      QString openclString;
      openclString.append("Version: " + QString::fromStdString(iter->version() ) );
      devicesLayout->addWidget(new QLabel( openclString ) );

      QString driverString;
      driverString.append("Driver Version: " + QString::fromStdString(iter->driver_version() ) );
      devicesLayout->addWidget(new QLabel( driverString ) );

    }//---DEVICES---END
    deviceCounter = 0;
    devicesLayout->insertStretch(-1,1); //add a spacer at the end

    platformBox->setLayout(devicesLayout);
    platformLayout->addWidget(platformBox);
    //    systemInfoLayout->addWidget(platformBox);
    systemInfoLayout->addLayout(platformLayout);

  }//---PLATFORMS---END
  delete ui->homeSystemInfoBox->layout();
  ui->homeSystemInfoBox->setLayout(systemInfoLayout);
#endif
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


void MainWindow::initBasicView(){
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
  ui->basic_FinalResultPlot->axisRect()->setMargins(QMargins( 100, 15, 0, 40 ));
  ui->basic_FinalResultPlot->axisRect()->setupFullAxesBox();
  //Disable secondary axes
  ui->basic_FinalResultPlot->yAxis2->setVisible(false);
  ui->basic_FinalResultPlot->xAxis2->setVisible(false);
  ui->basic_FinalResultPlot->setInteractions(QCP::iSelectPlottables);
  ui->basic_FinalResultPlot->legend->setVisible(false);

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

  ui->basic_FinalResultPlot->xAxis->setAutoTickLabels(true);
  ui->basic_FinalResultPlot->xAxis->setAutoTicks(true);
  ui->basic_FinalResultPlot->xAxis->setAutoTickStep(true);
  ui->basic_FinalResultPlot->xAxis->setRange(0,1);

  ui->basic_FinalResultPlot->setBackground(backgroundBrush);

  //  ui->basic_FinalResultPlot->replot();

  ui->basic_StopBenchmarkButton->hide();

  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  connect(ui->basic_BenchmarkListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(updateBenchmarkListWidget(QListWidgetItem*)) );
  for ( int i = 0; i < ui->basic_BenchmarkListWidget->count(); i++ ) {
    ui->basic_BenchmarkListWidget->item(i)->setSelected(true);
  }
}



void MainWindow::initExpertView(){
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

  ui->basic_StopBenchmarkButton->show();
  ui->basic_StartBenchmarkButton->hide();
  benchmarkController.executeSelectedBenchmark( selectedBenchmarkItems, getPrecision() );
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
  resultBar->setName(benchmarkName);
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

  QFont textFont;
  textFont.family();
  text->setFont(QFont(font().family(), 10, QFont::Bold)); // make font a bit larger

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
  resultBar->setName(benchmarkName);
  resultBar->addData(currentData);
  customPlot->addPlottable(resultBar);

  QCPItemText *text = new QCPItemText(customPlot);

  //  text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(  currentData.value , currentData.key );
  text->setFont(QFont(font().family(), 10, QFont::Bold)); // make font a bit larger

  //Probably the dirtiest hack-around I have ever made
  //Unable to find a way to properly align the text label with the graph bar
  //I added some whitespaces in front of the result number;
  //Making it properly aligned! xD
  text->setText(QString("                    ") + QString::number( currentData.value, 'f', 2  ));


  customPlot->addItem(text);

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

