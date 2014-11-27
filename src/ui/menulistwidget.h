#ifndef MENULISTWIDGET_H
#define MENULISTWIDGET_H

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

/*! \class MenuListWidget
 * \brief Main menu list widget. Customized to display menu items with icons in a proper centered way, among other things.
 */
class MenuListWidget : public QListWidget
{
  Q_OBJECT

public:
  explicit MenuListWidget(QWidget *parent = 0);
  virtual QSize sizeHint() const;
  QSize itemSizeHint() const;

signals:

public slots:
  void mouseMoveEvent(QMouseEvent *e);
};

#endif // MENULISTWIDGET_H
