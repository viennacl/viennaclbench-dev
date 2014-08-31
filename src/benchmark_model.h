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

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QList>
#include <QVariant>

#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#else
#endif

#include "benchmarkinstance.h"
#include "benchmarksettings.h"

/*! \class Benchmark_Model
 * \brief Responsible for saving and uploading benchmark results
 * */
class Benchmark_Model : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Model(QObject *parent = 0);
  void processBenchmarkInstance(BenchmarkInstance instance);
  void saveResults(BenchmarkInstance instance);
  void uploadResults(BenchmarkInstance instance);

#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
  QJsonDocument generateJson(BenchmarkInstance instance);
#else
#endif

signals:

public slots:
};

#endif // BENCHMARK_MODEL_H
