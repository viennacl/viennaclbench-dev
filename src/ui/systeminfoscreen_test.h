#ifndef SYSTEMINFOSCREEN_TEST_H
#define SYSTEMINFOSCREEN_TEST_H

#include <QWidget>

namespace Ui {
  class SystemInfoScreen_Test;
}

class SystemInfoScreen_Test : public QWidget
{
  Q_OBJECT

public:
  explicit SystemInfoScreen_Test(QWidget *parent = 0);
  ~SystemInfoScreen_Test();

private:
  Ui::SystemInfoScreen_Test *ui;
};

#endif // SYSTEMINFOSCREEN_TEST_H
