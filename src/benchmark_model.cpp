#include "benchmark_model.h"
#include <QDebug>

Benchmark_Model::Benchmark_Model(QObject *parent) :
  QObject(parent)
{
}

void Benchmark_Model::processBenchmarkInstance(BenchmarkInstance instance)
{
  saveResults(instance);
  uploadResults(instance);
}

//save results to local file in json format
void Benchmark_Model::saveResults(BenchmarkInstance instance)
{
  qDebug()<<"saving results";
  generateJson( instance );
  //  updateBenchmarkData(QString benchmarkName, double bandwidthValue)
}

void Benchmark_Model::uploadResults(BenchmarkInstance instance)
{
  //todo
  //upload benchmark results/info to server
}

QVariant Benchmark_Model::generateJson(BenchmarkInstance instance){
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
  QJsonObject rootObject;

  rootObject["mode"] = instance.mode;
  rootObject["full"] = instance.full;
  rootObject["totalScore"] = instance.totalScore;
  rootObject["precision"] = instance.precision;

  QJsonObject blas3Object;
  blas3Object["result"] = instance.blas3Result;
  blas3Object["mSize"] = instance.settings.blas3MatSizeA;
  blas3Object["nSize"] = instance.settings.blas3MatSizeB;
  blas3Object["kSize"] = instance.settings.blas3MatSizeC;

  rootObject["blas3"] = blas3Object;

  QJsonObject copyObject;
  copyObject["result"] = instance.copyResult;
  copyObject["min"] = instance.settings.copyMinVectorSize;
  copyObject["max"] = instance.settings.copyMaxVectorSize;
  copyObject["inc"] = instance.settings.copyIncFactor;

  rootObject["copy"] = copyObject;

  QJsonObject sparseObject;
  sparseObject["result"] = instance.sparseResult;
  sparseObject["sizeM"] = instance.settings.sparseMatSizeA;
  sparseObject["sizeN"] = instance.settings.sparseMatSizeB;
  sparseObject["customMatrix"] = instance.settings.sparseCustomMatrix;

  rootObject["sparse"] = sparseObject;

  QJsonObject vectorObject;
  vectorObject["result"] = instance.vectorResult;
  vectorObject["min"] = instance.settings.vectorMinVectorSize;
  vectorObject["max"] = instance.settings.vectorMaxVectorSize;
  vectorObject["inc"] = instance.settings.vectorIncFactor;

  rootObject["vector"] = vectorObject;

  QJsonDocument jsonDoc(rootObject);
  qDebug()<<"---toJson---";
  qDebug()<<jsonDoc.toJson(QJsonDocument::Indented);
  //  return jsonDoc.toJson();
  return jsonDoc.toVariant();
#else
  qDebug()<<"JSON is not supported is Qt4";
  return QVariant();
#endif
}
