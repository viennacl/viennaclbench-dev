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

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class ExpertBenchmark;
}

/*! \class ExpertBenchmark
 * \brief The expert (advanced) benchmark user interface is implemented here.
 * Handles benchmark selecting, progress updating, result plotting, starting\stopping, and changing benchmark settings.
 */
class ExpertBenchmark : public QWidget
{
  Q_OBJECT

public:
  explicit ExpertBenchmark(QWidget *parent = 0);
  ~ExpertBenchmark();

  void initExpert();
  void plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot);
  void plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId);
  void plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot);
  void resetAllPlots();
  bool validateSettings();
  bool estimateRequiredVideoMemory();
  BenchmarkSettings getExpertSettings();

  QPushButton *startBenchmarkButton; ///< Exposes the start button
  QPushButton *stopBenchmarkButton; ///< Exposes the stop button
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
  void setCustomSparseMatrixPath();
  void loadDefaultSettings();
  void setupCustomSparseMatrix(QString matrixFilePath);
  void toggleFullscreenPlots();
  void showHoverPointToolTip(QMouseEvent *event);
private:
  int activeBenchmark;
  int currentBenchProgress;
  int maximumBenchProgress;

  QTabWidget *expert_DetailedPlotTab;
  QVector<QCustomPlot*> expert_DetailedPlotsVector;
  QCustomPlot *blas3_DetailedPlot;
  QCustomPlot *copy_DetailedPlot;
  //  QCustomPlot *qr_DetailedPlot;
  //  QCustomPlot *solver_DetailedPlot;
  QCustomPlot *sparse_DetailedPlot;
  QCustomPlot *vector_DetailedPlot;

  Ui::ExpertBenchmark *ui;
};

#endif // EXPERTBENCHMARK_H
