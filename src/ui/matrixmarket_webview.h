#ifndef MATRIXMARKET_WEBVIEW_H
#define MATRIXMARKET_WEBVIEW_H

#include <QWebView>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMouseEvent>
#include <QDir>

class MatrixMarket_WebView : public QWebView
{
  Q_OBJECT
public:
  explicit MatrixMarket_WebView(QWidget *parent = 0);
  QNetworkReply *currentDownload;
private:
  QNetworkAccessManager *downloadManager;

signals:
  void currentDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
  void downloadSlot(QNetworkRequest request);
  void unsupportedContentSlot(QNetworkReply *reply);
  void mouseReleaseEvent(QMouseEvent *e);
  void processDownloadedFile(QNetworkReply *reply);
  void currentDownloadProgressSlot(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // MATRIXMARKET_WEBVIEW_H
