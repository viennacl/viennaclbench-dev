#ifndef BENCHMARKLISTWIDGET_H
#define BENCHMARKLISTWIDGET_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <QListWidget>
#include <QMouseEvent>

/*! \class BenchmarkListWidget
 * \brief A list widget for displaying a list of selectable benchmarks.
 */
class BenchmarkListWidget : public QListWidget
{
  Q_OBJECT

public:
  explicit BenchmarkListWidget(QWidget *parent = 0);
signals:

public slots:
  void mouseMoveEvent(QMouseEvent *e);
  void selectAllItems();
  void deselectAllItems();
  void checkSelectedItems();
  void setSelected(int itemRow);
  void setDeselected(int itemRow);
};

#endif // BENCHMARKLISTWIDGET_H
