#ifndef BENCHMARKLISTWIDGET_H
#define BENCHMARKLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>

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

};

#endif // BENCHMARKLISTWIDGET_H
