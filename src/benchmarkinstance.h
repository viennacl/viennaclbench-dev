#ifndef BENCHMARKINSTANCE_H
#define BENCHMARKINSTANCE_H
/*
 * Holds all information related to a benchmark run
 * For now it's just benchmark results and settings
 * */
#include <QObject>

class BenchmarkInstance : public QObject
{
  Q_OBJECT
public:
  explicit BenchmarkInstance(QObject *parent = 0);

signals:

public slots:

};

#endif // BENCHMARKINSTANCE_H
