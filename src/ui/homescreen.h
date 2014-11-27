#ifndef HOMESCREEN_H
#define HOMESCREEN_H

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
#include <QPushButton>
#include <QScrollArea>

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class HomeScreen;
}

/*! \class HomeScreen
 * \brief Homescreen widget is created with this class.
 */
class HomeScreen : public QWidget
{
  Q_OBJECT

public:
  explicit HomeScreen(QWidget *parent = 0);
  ~HomeScreen();
  QPushButton* quickStartButton; ///< Exposes the quickstart button for usage in other widgets

private:
  Ui::HomeScreen *ui;
};

#endif // HOMESCREEN_H
