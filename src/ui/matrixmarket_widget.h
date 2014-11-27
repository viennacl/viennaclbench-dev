#ifndef MATRIXMARKET_WIDGET_H
#define MATRIXMARKET_WIDGET_H

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
#include "matrixmarket_webview.h"

namespace Ui {
  class MatrixMarket_Widget;
}

/*! \class MatrixMarket_Widget
 * \brief MatrixMarket widget is implemented here.
 */
class MatrixMarket_Widget : public QWidget
{
  Q_OBJECT

public:
  explicit MatrixMarket_Widget(QWidget *parent = 0);
  ~MatrixMarket_Widget();
  MatrixMarket_WebView *webView; ///< The WebView used for rendering HTML pages.

public slots:
  void updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal);
  void loadHomePage();
private:
  Ui::MatrixMarket_Widget *ui;
};

#endif // MATRIXMARKET_WIDGET_H
