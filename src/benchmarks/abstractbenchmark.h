#ifndef ABSTRACTBENCHMARK_H
#define ABSTRACTBENCHMARK_H

#include <QObject>
#include <QString>

/*
 * Abstract benchmark interface class for easier handling of benchmark classes
 * */

enum{ BLAS3, COPY, SPARSE, VECTOR, QR, SOLVER, SCHEDULER, ALL };//benchmarkIdNumber

enum{ SINGLE_PRECISION, DOUBLE_PRECISION };//benchmark precision

enum{ BAR_GRAPH, LINE_GRAPH };//graph type to be used when plotting test results

class AbstractBenchmark : public QObject
{
  Q_OBJECT
public:
  explicit AbstractBenchmark(QObject *parent = 0);
private:
  bool precision;
signals:
  void benchmarkStarted(int benchmarkIdNumber);
  void finalResultSignal(QString benchmarkName, double finalValue);
  void resultSignal(QString benchmarkName, double key, double value, int graphType, int testId);
  void benchmarkComplete();
  void unitMeasureSignal(QString unitMeasureName);
  void testProgress();
public slots:
  void setPrecision(bool p);
  bool getPrecision();
  virtual void execute() = 0;
};

#endif // ABSTRACTBENCHMARK_H
