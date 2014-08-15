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
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSplitter>
#include <QMap>
#include <QMultiMap>

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

  void initExpertView();
  void initBasicView();
  void initHomeScreen();
  void initMatrixMarket();
  void plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot);
  void plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId);
  void plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot);
  void resetAllPlots();
  bool getPrecision();
  void initSystemInfo();
  void interconnectViews();
  void initPlatformDeviceChooser();
private:
  QMap<int, QString> contextMap;
  Ui::MainWindow *ui;
  Benchmark_Controller benchmarkController;
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
  QString jsString;

public slots:
  void resetPlotData(QCustomPlot *benchmarkGraph);
  void parseBenchmarkResult(QString benchmarkName, double key, double resultValue, int graphType, int testId);
  void updateBenchmarkUnitMeasure(QString unitMeasureName, int axis);
  void startBenchmarkExecution();
  void updateBenchmarkListWidget(QListWidgetItem* item);
  void updateFinalResultPlot(QString benchmarkName, double finalResult);
  void setActiveBenchmarkPlot(int benchmarkIdNumber);
  void quickstartFullBenchmark();
  void modifyMatrixMarketWeb();
  void updateSinglePrecisionButtons();
  void updateDoublePrecisionButtons();
  void stopBenchmarkExecution();
  void showBenchmarkStartButton();
  void graphClicked(QCPAbstractPlottable *plottable);
  void updateBenchProgress();
  void switchContext(int contextNumber);
  void legendClicked(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
};

#endif // MAINWINDOW_H
