#ifndef BENCHMARKLISTWIDGET_H
#define BENCHMARKLISTWIDGET_H

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
};

#endif // BENCHMARKLISTWIDGET_H
