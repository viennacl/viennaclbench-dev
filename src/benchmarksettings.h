#ifndef BENCHMARKSETTINGS_H
#define BENCHMARKSETTINGS_H

/*
 * Holds all settings of selected benchmarks
 * */
#include <QString>

//#include <QObject>
//inheriting from QObject prevents usage of assignment operator
class BenchmarkSettings// : public QObject
{
//  Q_OBJECT
public:
//  explicit BenchmarkSettings(QObject *parent = 0);
  explicit BenchmarkSettings();

  int copyMinVectorSize;
  int copyMaxVectorSize;
  int copyIncFactor;

  int vectorMinVectorSize;
  int vectorMaxVectorSize;
  int vectorIncFactor;

  int blas3MatSizeA;
  int blas3MatSizeB;
  int blas3MatSizeC;

  int sparseMatSizeA;
  int sparseMatSizeB;
  QString sparseCustomMatrix;

  BenchmarkSettings& operator =(const BenchmarkSettings &other);

  void setCopySettings(int min, int max, int increment);
  void setVectorSettings(int min, int max, int increment);
  void setBlas3Settings(int matA, int matB, int matC);
  void setSparseSettings(int matA, int matB, QString pathToCustomMatrix);
  void initBasicDefaults();
//signals:

//public slots:

  void setSettings(BenchmarkSettings s);
};

#endif // BENCHMARKSETTINGS_H
