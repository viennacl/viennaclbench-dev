#include "systeminfoscreen.h"
#include "ui_systeminfoscreen.h"

SystemInfoScreen::SystemInfoScreen(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SystemInfoScreen)
{
  ui->setupUi(this);

#ifdef VIENNACL_WITH_OPENCL
  QHBoxLayout *systemInfoLayout = new QHBoxLayout();

  typedef std::vector< viennacl::ocl::platform > platforms_type;
  platforms_type platforms = viennacl::ocl::get_platforms();
  bool is_first_element = true;

  //---PLATFORMS---
//  QVBoxLayout *platformsLayout = new QVBoxLayout();
  for(platforms_type::iterator platform_iter = platforms.begin(); platform_iter != platforms.end(); ++platform_iter){
    QGroupBox *platformBox = new QGroupBox(QString::fromStdString(platform_iter->info()) );

    typedef std::vector<viennacl::ocl::device> devices_type;
    devices_type devices = platform_iter->devices(CL_DEVICE_TYPE_ALL);

    if (is_first_element)
    {
      platformBox->setTitle(platformBox->title()+" (default)");
      is_first_element = false;
    }
    //---DEVICES---
    int deviceCounter = 0;
//    QVBoxLayout *devicesLayout = new QVBoxLayout();
    QSplitter *platformSplitter = new QSplitter();
    platformSplitter->setOrientation(Qt::Vertical);
    for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
    {
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
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->exec_capabilities_to_string(iter->execution_capabilities())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Extensions:                    ")) );
      QString extensions = QString::fromStdString( iter->extensions());
      extensions.replace(" ","\n");
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( extensions ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Global Mem Cache Size:         ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->global_mem_cache_size() ) + QString( " Bytes") ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Global Mem Cache Type:         ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->mem_cache_type_to_string(iter->global_mem_cache_type())) ) );

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
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->local_mem_type_to_string(iter->local_mem_type())) ) );

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
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->convert_to_string(iter->max_work_item_sizes())) ) );

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
      /* cl_platform_id conversion complications */
      std::ostringstream oss;
      oss << iter->platform();
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( oss.str()) ) );

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
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->queue_properties_to_string(iter->queue_properties())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Single FP Config:              ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->fp_config_to_string(iter->single_fp_config())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Type:                          ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->device_type_to_string(iter->type())) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Vendor:                        ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->vendor()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Vendor ID:                     ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::number( iter->vendor_id()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Version:                       ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->version()) ) );

      deviceInfoTable->setItem(row, cProp, new QTableWidgetItem( QString("Driver Version:                ")) );
      deviceInfoTable->setItem(row++, cVal, new QTableWidgetItem( QString::fromStdString( iter->driver_version()) ) );
      deviceInfoTable->setContextMenuPolicy(Qt::NoContextMenu);
      deviceInfoTable->setSelectionBehavior(QAbstractItemView::SelectItems);
      deviceInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);
      deviceInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
      deviceInfoTable->horizontalHeader()->setStretchLastSection(true);
      deviceInfoTable->resizeColumnsToContents();
      deviceInfoTable->resizeRowsToContents();
      deviceInfoTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

      platformSplitter->addWidget(deviceInfoTable);

    }//---DEVICES---END
    deviceCounter = 0;
    systemInfoLayout->addWidget(platformSplitter);

  }//---PLATFORMS---END

  delete ui->systemInfo_Box->layout();
  ui->systemInfo_Box->setLayout(systemInfoLayout);
#endif

}

SystemInfoScreen::~SystemInfoScreen()
{
  delete ui;
}
