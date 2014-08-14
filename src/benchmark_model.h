#ifndef BENCHMARK_MODEL_H
#define BENCHMARK_MODEL_H
/*
 * Handles benchmark result persistence
 * Saves results to JSON files
 * Loads results from JSON files
 * Uploads results in JSON format to remote servers using HTTP
 * Fetches resuls in JSON format from remote servers using HTTP
 * */
#include <QObject>

class Benchmark_Model : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Model(QObject *parent = 0);

signals:

public slots:

};

#endif // BENCHMARK_MODEL_H
