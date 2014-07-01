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
#include "src/ui/qcustomplot.h"
#include "src/benchmark_controller.h"
#include <ctime>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void showResult(double value, QCustomPlot *customPlot);
  void initQCustomPlotGraph();
private:
  Ui::MainWindow *ui;
  Benchmark_Controller benchmarkController;
  int barCounter;//counts the total number of benchmarks
  QVector<double> barData;//benchmark bandwidth values
  QVector<QString> labels;//tracks the names of benchmarks
  QVector<double> ticks;//defines the number and order in which to show benchmark name labels on the yAxis

  double rx_value, key_x, range_y_min , range_y_max;//ued by the predefined sample graph


private slots:

public slots:
  void resetData();
  void graphData();
  void parseBenchmarkResult(QString benchmarkName, double bandwidthValue);
  void updateBenchmarkUnitMeasure(QString unitMeasureName);
  void startBenchmarkExecution();
};

#endif // MAINWINDOW_H
