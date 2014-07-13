#ifndef MATRIXMARKET_WEBVIEW_H
#define MATRIXMARKET_WEBVIEW_H

#include <QWebView>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMouseEvent>

class MatrixMarket_WebView : public QWebView
{
  Q_OBJECT
public:
  explicit MatrixMarket_WebView(QWidget *parent = 0);

signals:

public slots:
  void downloadSlot(QNetworkRequest request);
  void unsupportedContentSlot(QNetworkReply *reply);
  void mouseReleaseEvent(QMouseEvent *e);
};

#endif // MATRIXMARKET_WEBVIEW_H
