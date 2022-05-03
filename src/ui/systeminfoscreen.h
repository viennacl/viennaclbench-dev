#ifndef SYSTEMINFOSCREEN_H
#define SYSTEMINFOSCREEN_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSpacerItem>
#include <QSplitter>
#include <QHeaderView>

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class SystemInfoScreen;
}

/*! \class SystemInfoScreen
 * \brief System Info widget is created with this class.
 */
class SystemInfoScreen : public QWidget
{
  Q_OBJECT

public:
  explicit SystemInfoScreen(QWidget *parent = 0);
  ~SystemInfoScreen();

private:
  Ui::SystemInfoScreen *ui;
};

#endif // SYSTEMINFOSCREEN_H
