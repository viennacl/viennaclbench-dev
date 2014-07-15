#ifndef MATRIXMARKET_WIDGET_H
#define MATRIXMARKET_WIDGET_H

#include <QWidget>
#include "matrixmarket_webview.h"

namespace Ui {
  class MatrixMarket_Widget;
}

class MatrixMarket_Widget : public QWidget
{
  Q_OBJECT

public:
  explicit MatrixMarket_Widget(QWidget *parent = 0);
  ~MatrixMarket_Widget();
  MatrixMarket_WebView *webView;

public slots:
  void updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal);
  void loadHomePage();
private:
  Ui::MatrixMarket_Widget *ui;
};

#endif // MATRIXMARKET_WIDGET_H
