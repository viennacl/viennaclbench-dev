#ifndef BENCHMARKCOPY_H
#define BENCHMARKCOPY_H

#include <QObject>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QVariant>
#include <QStringList>
#include <QString>

class BenchmarkCopy : public QObject
{
  Q_OBJECT
public:
  explicit BenchmarkCopy(QObject *parent = 0);

  template<typename ScalarType>
  void run_benchmark();
private:
  QStringList benchmarkNames;
  QList<double> dataPoints;
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();
public slots:
  void execute();
  void updateBenchmarkData(QString benchmarkName, double bandwidthValue);
  QVariant getJsonData();
};

#endif // BENCHMARKCOPY_H
