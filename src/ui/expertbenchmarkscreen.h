#ifndef EXPERTBENCHMARK_H
#define EXPERTBENCHMARK_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QVector>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QPainter>
#include <QTabWidget>
#include <QDebug>
#include <QThread>
#include <QSpacerItem>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QMap>
#include <QMultiMap>
#include <QPushButton>
#include <QProgressBar>
#include <QComboBox>
#include <QFileDialog>

#include <cstdlib>
#include <ctime>

#include "src/qcustomplot/qcustomplot.h"
#include "../benchmark_controller.h"
#include "widgets/benchmarklistwidget.h"
#include "widgets/collapsewidget.h"
#include "matrixmarket_widget.h"
#include "benchmarkscreen.h"

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class ExpertBenchmark;
}

/*! \class ExpertBenchmarkScreen
 * \brief Inherits from BenchmarkScreen and adds features specific to the expert(advanced) benchmark.
 * Handles the expert benchmark-specific features (expert settings config UI & functionality).
 */
class ExpertBenchmarkScreen : public BenchmarkScreen
{
  Q_OBJECT

public:
  explicit ExpertBenchmarkScreen(QWidget *parent = 0);
  ~ExpertBenchmarkScreen();

  void initExpertSpecific();
  bool validateSettings();
  BenchmarkSettings getExpertSettings();
public slots:
  void setCustomSparseMatrixPath();
  void loadDefaultSettings();
  void setupCustomSparseMatrix(QString matrixFilePath);
private:
  Ui::ExpertBenchmark *ui;
};

#endif // EXPERTBENCHMARK_H
