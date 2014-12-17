#ifndef BASICBENCHMARK_H
#define BASICBENCHMARK_H

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

#include <cstdlib>
#include <ctime>

#include "src/qcustomplot/qcustomplot.h"
#include "../benchmark_controller.h"
#include "benchmarklistwidget.h"
#include "collapsewidget.h"
#include "matrixmarket_widget.h"

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class BasicBenchmark;
}

/*! \class BasicBenchmark
 * \brief The basic (standard) benchmark user interface is implemented here.
 * Handles benchmark selecting, progress updating, result plotting, and starting\stopping.
 */
class BasicBenchmark : public QWidget
{
  Q_OBJECT

public:
  explicit BasicBenchmark(QWidget *parent = 0);
  ~BasicBenchmark();

  void initBasic();
  void plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot);
  void plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId);
  void plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot);
  void resetAllPlots();

  QPushButton *startBenchmarkButton; ///<  Exposes the start button
  QPushButton *stopBenchmarkButton; ///<  Exposes the stop button
  QPushButton  *singlePrecisionButton; ///< Exposes the single precision button
  QProgressBar *progressBar; ///< Exposes the progressbar
  BenchmarkListWidget *benchmarkListWidget; ///< Exposes the benchmark selection list widget
  QComboBox *contextComboBox; ///< Exposes the context chooser

public slots:
  void hideStopButton();
  void showBenchmarkStartButton();
  void updateSinglePrecisionButtons();
  void updateDoublePrecisionButtons();
  void setActiveBenchmarkPlot(int benchmarkIdNumber);
  void updateFinalResultPlot(QString benchmarkName, double finalResult);
  void graphClicked(QCPAbstractPlottable *plottable);
  void selectionChanged();
  void showPointToolTip(QMouseEvent *event);
  void updateBenchProgress();
  void updateBenchmarkListWidget(QListWidgetItem *item);
  void resetPlotData(QCustomPlot *benchmarkGraph);
  void updateBenchmarkUnitMeasure(QString unitMeasureName, int axis);
  void parseBenchmarkResult(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void toggleFullscreenPlots();
  void showHoverPointToolTip(QMouseEvent *event);
private:
  int activeBenchmark;
  int currentBenchProgress;
  int maximumBenchProgress;

  QTabWidget *basic_DetailedPlotTab;
  QVector<QCustomPlot*> basic_DetailedPlotsVector;
  QCustomPlot *blas3_DetailedPlot;
  QCustomPlot *copy_DetailedPlot;
  //  QCustomPlot *qr_DetailedPlot;
  //  QCustomPlot *solver_DetailedPlot;
  QCustomPlot *sparse_DetailedPlot;
  QCustomPlot *vector_DetailedPlot;
  Ui::BasicBenchmark *ui;
};

#endif // BASICBENCHMARK_H
