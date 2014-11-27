#ifndef MATRIXMARKET_WEBVIEW_H
#define MATRIXMARKET_WEBVIEW_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <QObject>//QObject must be included in order for the next line to work
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

/*! \class MatrixMarket_WebView
 * \brief This is a customized WebView class for the MatrixMarket.
 */
class MatrixMarket_WebView : public QWebView
{
  Q_OBJECT
public:
  explicit MatrixMarket_WebView(QWidget *parent = 0);
  QNetworkReply *currentDownload; ///< Keeps track of what is currently being downloaded.
private:
  QNetworkAccessManager *downloadManager;

signals:
  void currentDownloadProgress(qint64 bytesReceived, qint64 bytesTotal); ///< Emits current download progress status
  void fileReadyForBenchmark(QString filename); ///< Emitted after a matrix file is downloaded, saved and ready for benchmarking

public slots:
  void downloadSlot(QNetworkRequest request);
  void unsupportedContentSlot(QNetworkReply *reply);
  void mouseReleaseEvent(QMouseEvent *e);
  void processDownloadedFile(QNetworkReply *reply);
  void currentDownloadProgressSlot(qint64 bytesReceived, qint64 bytesTotal);
  void linkClickedSlot(QUrl url);
};

#endif // MATRIXMARKET_WEBVIEW_H
