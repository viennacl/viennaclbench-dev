#ifndef COLLAPSEWIDGET_H
#define COLLAPSEWIDGET_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <QWidget>

namespace Ui {
  class CollapseWidget;
}

/*! \class CollapseWidget
 * \brief A simple widget for displaying a collapsible child widget.
 */
class CollapseWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CollapseWidget(QWidget *parent = 0);
  ~CollapseWidget();

  void setText(QString textToShow);
  void setChildWidget(QWidget *child);
private:
  Ui::CollapseWidget *ui;

signals:
  void toggleChildVisibility();
public slots:
  void updateChildVisibility();
};

#endif // COLLAPSEWIDGET_H
