#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QVector>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QPainter>
#include <QObject>

#include <QDebug>
#include <QListWidgetItem>
#include "qcustomplot.h"
#include "../benchmark_controller.h"
#include <ctime>
#include "benchmarklistwidget.h"
#include "collapsewidget.h"

namespace Ui {
  class MainWindow;
}

enum{ ALL, BLAS3, COPY, QR, SOLVER, SPARSE, VECTOR };

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void showResult(double value, QCustomPlot *customPlot);
  void initExpertView();
  void initBasicView();
  void initHomeScreen();
private:
  Ui::MainWindow *ui;
  Benchmark_Controller benchmarkController;
  int barCounter;//counts the total number of benchmarks
  QVector<double> barData;//benchmark bandwidth values
  QVector<QString> labels;//tracks the names of benchmarks
  QVector<double> ticks;//defines the number and order in which to show benchmark name labels on the yAxis


private slots:

public slots:
  void resetData();
  void parseBenchmarkResult(QString benchmarkName, double bandwidthValue);
  void updateBenchmarkUnitMeasure(QString unitMeasureName);
  void startBenchmarkExecution();
  void updateBenchmarkListWidget(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
