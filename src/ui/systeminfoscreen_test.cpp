#include "systeminfoscreen_test.h"
#include "ui_systeminfoscreen_test.h"

SystemInfoScreen_Test::SystemInfoScreen_Test(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SystemInfoScreen_Test)
{
  ui->setupUi(this);
}

SystemInfoScreen_Test::~SystemInfoScreen_Test()
{
  delete ui;
}
