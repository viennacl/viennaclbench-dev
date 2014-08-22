#ifndef RESULTHISTORY_H
#define RESULTHISTORY_H
/*
 * Handles result history
 * Loads all result files found in userHomeFolder/ViennaCL-Benchmark/benchmarkHistory
 * And shows them in the table widget
 * */
#include <QWidget>
#include <QAbstractItemModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTableWidgetItem>
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#else
#endif

namespace Ui {
  class ResultHistory;
}

class ResultHistory : public QWidget
{
  Q_OBJECT

public:
  explicit ResultHistory(QWidget *parent = 0);
  ~ResultHistory();

#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
  void readResultFile(const QJsonObject &jsonRoot);
#else
#endif

public slots:
  void loadHistory();
private:
  Ui::ResultHistory *ui;
};

#endif // RESULTHISTORY_H
