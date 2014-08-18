#ifndef RESULTHISTORY_H
#define RESULTHISTORY_H

#include <QWidget>
#include <QAbstractItemModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QTableWidgetItem>
#include <QJsonObject>
#include <QJsonValue>

namespace Ui {
  class ResultHistory;
}

class ResultHistory : public QWidget
{
  Q_OBJECT

public:
  explicit ResultHistory(QWidget *parent = 0);
  ~ResultHistory();

public slots:
  void loadHistory();
  void readResultFile(const QJsonObject &jsonRoot);
private:
  Ui::ResultHistory *ui;
};

#endif // RESULTHISTORY_H
