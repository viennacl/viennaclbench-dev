#include "homescreen.h"
#include "ui_homescreen.h"

/*!
 * \brief Default constructor. Inits the UI and creates the system info box with platforms and devices.
 * \param parent Optional parent object.
 */
HomeScreen::HomeScreen(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::HomeScreen)
{
  ui->setupUi(this);
  this->quickStartButton = ui->homeQuickStartButon;

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
      platformBox->setTitle(platformBox->title());
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
      memoryString.append("Global Memory Size: " + QString::number( ((uint)iter->global_mem_size()/(1024*1024)) ) + " MB" );
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

/*!
 * \brief Destructor.
 */
HomeScreen::~HomeScreen()
{
  delete ui;
}
