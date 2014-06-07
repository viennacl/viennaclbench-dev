#ifndef BENCHMARK_COPY_H
#define BENCHMARK_COPY_H

#include <QObject>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QVariant>
#include <QStringList>
#include <QString>

class Benchmark_Copy : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Copy(QObject *parent = 0);

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

#endif // BENCHMARK_COPY_H
