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

/*! \class MainWindow
 * \brief Main UI class. This is where the magic happens. Root and anchor of every other UI component in this program.
 * Inits its UI and all other UI widget components.
 * Communicates events & signals between child widgets and the controller.
 * Keeps track of available ViennaCL contexts and switches contexts before starting benchmarks.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void initMatrixMarket();
  bool getBasicPrecision();
  bool getExpertPrecision();
  void initPlatformDeviceChooser();
private:
  QMap<int, QString> contextMap; ///< Maps available contexts for easier switching.
  Ui::MainWindow *ui;
  Benchmark_Controller benchmarkController; ///< The main controller object
  int currentBenchProgress; ///< Current benchmark progress
  int maximumBenchProgress; ///< Maximum benchmark progress

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

  QString jsString; ///< A simple JavaScript script that filters all links on a page to remove non .tar.gz file links in .mtx format. Not used anymore.

public slots:
  void startBasicBenchmarkExecution();
  void quickstartFullBenchmark();
  void modifyMatrixMarketWeb();
  void switchContext(int contextNumber);
  void startMatrixMarketBenchmark(QString filename);
  void startExpertBenchmarkExecution();
  void showErrorMessageBox(QString message);
};

#endif // MAINWINDOW_H
