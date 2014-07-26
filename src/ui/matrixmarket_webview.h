#ifndef MATRIXMARKET_WEBVIEW_H
#define MATRIXMARKET_WEBVIEW_H

#include <QObject>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtWebKit/QWebView>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#else
#include <QtWebKitWidgets/QWebView>
#include <QNetworkReply>
#include <QNetworkRequest>
#endif

#include <QDesktopServices>
#include <QMouseEvent>
#include <QDir>
#include "../archiveextractor.h"

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
  void linkClickedSlot(QUrl url);
};

#endif // MATRIXMARKET_WEBVIEW_H
