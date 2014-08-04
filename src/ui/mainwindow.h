#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QObject>
#include <QTabWidget>
#include <QDebug>
#include <QListWidgetItem>
#include <QThread>
#include <QSpacerItem>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#else
#include <QtWebKitWidgets/QWebView>
#include <QWebFrame>
#endif

#include <cstdlib>
#include <ctime>

#include "qcustomplot.h"
#include "../benchmark_controller.h"
#include "benchmarklistwidget.h"
#include "collapsewidget.h"
#include "matrixmarket_widget.h"

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void plotResult(QString benchmarkName, double value, QCustomPlot *customPlot);
  void initExpertView();
  void initBasicView();
  void initHomeScreen();
  void plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot);
  void resetAllPlots();
  void initMatrixMarket();
  bool getPrecision();
private:
  Ui::MainWindow *ui;
  Benchmark_Controller benchmarkController;
  int activeBenchmark;
//  int barCounter;//counts the total number of benchmarks
//  QVector<double> barData;//benchmark bandwidth values
//  QVector<QString> labels;//tracks the names of benchmarks
//  QVector<double> ticks;//defines the number and order in which to show benchmark name labels on the yAxis

  QTabWidget *basic_DetailedPlotTab;
  QVector<QCustomPlot*> basic_DetailedPlotsVector;
  QCustomPlot *blas3_DetailedPlot;
  QCustomPlot *copy_DetailedPlot;
//  QCustomPlot *qr_DetailedPlot;
//  QCustomPlot *solver_DetailedPlot;
  QCustomPlot *sparse_DetailedPlot;
  QCustomPlot *vector_DetailedPlot;
  QString jsString;

public slots:
  void resetPlotData(QCustomPlot *benchmarkGraph);
  void parseBenchmarkResult(QString benchmarkName, double resultValue);
  void updateBenchmarkUnitMeasure(QString unitMeasureName);
  void startBenchmarkExecution();
  void updateBenchmarkListWidget(QListWidgetItem* item);
  void updateFinalResultPlot(QString benchmarkName, double finalResult);
  void setActiveBenchmarkPlot(int benchmarkIdNumber);
  void quickstartFullBenchmark();
  void modifyMatrixMarketWeb();
  void updateSinglePrecisionButtons();
  void updateDoublePrecisionButtons();
};

#endif // MAINWINDOW_H
