#ifndef BENCHMARKINSTANCE_H
#define BENCHMARKINSTANCE_H
/*
 * Holds all information related to a benchmark run
 * For now it's just benchmark results and settings
 * */
//#include <QObject>
#include "benchmarksettings.h"

class BenchmarkInstance// : public QObject
{
//  Q_OBJECT
public:
  explicit BenchmarkInstance(/*QObject *parent = 0*/);

  BenchmarkSettings settings;
  double blas3Result;
  double copyResult;
  double sparseResult;
  double vectorResult;
  QString mode;
  bool full;
  double totalScore;
  QString precision;
  void setSettings(BenchmarkSettings s);
//signals:

//public slots:

};

#endif // BENCHMARKINSTANCE_H
