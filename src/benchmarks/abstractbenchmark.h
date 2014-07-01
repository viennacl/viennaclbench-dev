#ifndef ABSTRACTBENCHMARK_H
#define ABSTRACTBENCHMARK_H

#include <QObject>

/*
 * Abstract benchmark interface class for easier handling of benchmark classes
 * */

class AbstractBenchmark : public QObject
{
  Q_OBJECT
public:
  explicit AbstractBenchmark(QObject *parent = 0);
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();
  void unitMeasureSignal(QString unitMeasureName);
public slots:
  virtual void execute() = 0;
};

#endif // ABSTRACTBENCHMARK_H