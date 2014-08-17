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
#include <QListWidgetItem>
#include <QDebug>
#include <QThread>
#include <QSpacerItem>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSplitter>
#include <QMap>
#include <QMultiMap>
#include <QPushButton>
#include <QProgressBar>
#include <QComboBox>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#else
#include <QtWebKitWidgets/QWebView>
#include <QWebFrame>
#endif

#include <cstdlib>
#include <ctime>

#include "../benchmarksettings.h"
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

  void initMatrixMarket();
  void plotBarResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot);
  void plotLineResult(QString benchmarkName, double key, double value, QCustomPlot *customPlot, int testId);
  void plotFinalResult(QString benchmarkName, double value, QCustomPlot *customPlot);
  void resetAllPlots();
  bool getBasicPrecision();
  bool getExpertPrecision();
  void interconnectViews();
  void initPlatformDeviceChooser();
private:
  QMap<int, QString> contextMap;
  Ui::MainWindow *ui;
  Benchmark_Controller benchmarkController;
  int currentBenchProgress;
  int maximumBenchProgress;


  BenchmarkListWidget *basicBenchmarkListWidget;
  QPushButton *basicStartBenchmarkButton;
  QPushButton *basicStopBenchmarkButton;
  QProgressBar *basicProgressBar;
  QComboBox *basicContextComboBox;
  QPushButton  *basicSinglePrecisionButton;

  BenchmarkListWidget *expertBenchmarkListWidget;
  QPushButton *expertStartBenchmarkButton;
  QPushButton *expertStopBenchmarkButton;
  QProgressBar *expertProgressBar;
  QComboBox *expertContextComboBox;
  QPushButton  *expertSinglePrecisionButton;


  QString jsString;

public slots:
  //BASIC MODE SLOTS
  void startBasicBenchmarkExecution();
  void quickstartFullBenchmark();
  void modifyMatrixMarketWeb();
  void switchContext(int contextNumber);
  void startMatrixMarketBenchmark(QString filename);
  void startExpertBenchmarkExecution();
};

#endif // MAINWINDOW_H
