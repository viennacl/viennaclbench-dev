#include "benchmark_model.h"
#include <QDebug>

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
Benchmark_Model::Benchmark_Model(QObject *parent) :
  QObject(parent)
{
}

/*!
 * \brief Convenience function for both saving and uploading results.
 * \param instance The benchmark instance to be processed
 */
void Benchmark_Model::processBenchmarkInstance(BenchmarkInstance instance)
{
  return; //disable result processing for 1.0.0 release
  saveResults(instance);
  uploadResults(instance);
}

/*!
 * \brief Saves results in JSON format to local file system (userHomeFolder/ViennaCL-Benchmark/benchmarkHistory).
 * \param instance The benchmark instance to be saved
 */
void Benchmark_Model::saveResults(BenchmarkInstance instance)
{
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
  QJsonDocument doc = generateJson( instance );
//  qDebug()<<"---toJson---";
//  qDebug()<<doc.toJson(QJsonDocument::Indented);

  QString saveFolderPath = QDir::home().absolutePath() + QString("/ViennaCL-Benchmark/benchmarkHistory/");
  QDir saveDir(saveFolderPath);
  if(!saveDir.exists(saveFolderPath)){
    saveDir.mkpath(saveFolderPath);
  }
  QString filename;
//  filename = saveFolderPath + QString("resultSave.json");
  filename = QDateTime::currentDateTime().toString( QString("d.M.yyyy_h-m-s")) + QString(".json");//set filename to current datetime, ensuring each has a unique name
  QString finalPath = saveFolderPath + filename;
//  qDebug()<<finalPath;
  QFile jsonFile(finalPath);
  if(!jsonFile.open(QIODevice::WriteOnly)){
    qDebug()<<"Failed to open json file.";
    return;
  }
  jsonFile.write( doc.toJson(QJsonDocument::Indented) );
  jsonFile.close();
#endif
}

/*!
 * \brief Uploads results to ViennaCL's server.
 * \param instance The benchmark instance to uploaded
 */
void Benchmark_Model::uploadResults(BenchmarkInstance instance)
{
  //todo
  //upload benchmark results/info to server
}

//Converts a given benchmark instance object to a QJsonDocument
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
/*!
 * \brief Converts a given benchmark instance object to JSON format (QJsonDocument).
 * \param instance The benchmark instance to be converted
 * \return Returns the supplied benchmark instance in JSON format
 */
QJsonDocument Benchmark_Model::generateJson(BenchmarkInstance instance){
  QJsonObject rootObject;

  rootObject["mode"] = instance.mode;
  rootObject["full"] = instance.full;
  rootObject["totalScore"] = instance.totalScore;
  rootObject["precision"] = instance.precision;

  QJsonObject blas3Object;
  blas3Object["result"] = instance.blas3Result;
  blas3Object["minSize"] = instance.settings.blas3MinSize;
  blas3Object["maxSize"] = instance.settings.blas3MaxSize;
  blas3Object["incFactor"] = instance.settings.blas3IncFactor;

  rootObject["blas3"] = blas3Object;

  QJsonObject copyObject;
  copyObject["result"] = instance.copyResult;
  copyObject["min"] = instance.settings.copyMinVectorSize;
  copyObject["max"] = instance.settings.copyMaxVectorSize;
  copyObject["inc"] = instance.settings.copyIncFactor;

  rootObject["copy"] = copyObject;

  QJsonObject sparseObject;
  sparseObject["result"] = instance.sparseResult;
  sparseObject["matrixSize"] = instance.settings.sparseMatSize;
  sparseObject["customMatrix"] = instance.settings.sparseCustomMatrix;

  rootObject["sparse"] = sparseObject;

  QJsonObject vectorObject;
  vectorObject["result"] = instance.vectorResult;
  vectorObject["min"] = instance.settings.vectorMinVectorSize;
  vectorObject["max"] = instance.settings.vectorMaxVectorSize;
  vectorObject["inc"] = instance.settings.vectorIncFactor;

  rootObject["vector"] = vectorObject;

  QJsonDocument jsonDoc(rootObject);
  return jsonDoc;
}
#endif
