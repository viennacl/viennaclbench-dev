#ifndef MATRIXMARKET_ALTERNATIVE_H
#define MATRIXMARKET_ALTERNATIVE_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QMap>
#include <QNetworkReply>
#include <QFile>
#include <QStandardItemModel>
#include "../archiveextractor.h"
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#else
#endif

namespace Ui {
  class MatrixMarket_Alternative;
}

class MatrixMarket_Alternative : public QWidget
{
  Q_OBJECT

public:
  explicit MatrixMarket_Alternative(QWidget *parent = 0);
  ~MatrixMarket_Alternative();

  void createMatrixTable();
  void loadJsonMatrixData();
  void createMatrixTableView();
private:
  Ui::MatrixMarket_Alternative *ui;

  QStandardItemModel *matrixModel;
  QMap<QNetworkReply*, int> thumbMap;
  QMap<QNetworkReply*, int> graphMap;
  QMap<QNetworkReply*, QStandardItem*> thumbMap2;
  QMap<QNetworkReply*, QStandardItem*> graphMap2;
  QMap<QPushButton*, QString> downloadUrlMap;
  QNetworkAccessManager *thumbNetManager;
  QNetworkAccessManager *graphNetManager;
  QNetworkAccessManager *thumbNetManager2;
  QNetworkAccessManager *graphNetManager2;
  QNetworkAccessManager *matrixFileNetManager;
  QJsonArray jsonArray;
signals:
  void currentDownloadProgress(qint64 bytesReceived, qint64 bytesTotal); ///< Emits current download progress status
  void fileReadyForBenchmark(QString filename); ///< Emitted after a matrix file is downloaded, saved and ready for benchmarking
public slots:
  void graphDownloadFinished(QNetworkReply *reply);
  void thumbnailDownloadFinished(QNetworkReply *reply);
  void graphDownloadFinished2(QNetworkReply *reply);
  void thumbnailDownloadFinished2(QNetworkReply *reply);
  void getMatrixUrl();
  void processDownloadedFile(QNetworkReply *reply);
  void currentDownloadProgressSlot(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // MATRIXMARKET_ALTERNATIVE_H
