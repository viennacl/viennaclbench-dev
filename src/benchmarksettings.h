#ifndef BENCHMARKSETTINGS_H
#define BENCHMARKSETTINGS_H

#include <QString>

/*! \class BenchmarkSettings
 * \brief Container class for all benchmarks' settings
 */
class BenchmarkSettings// : public QObject
{
//  Q_OBJECT
public:
//  explicit BenchmarkSettings(QObject *parent = 0);
  explicit BenchmarkSettings();

  int copyMinVectorSize; ///< MAX_BENCHMARK_VECTOR_SIZE
  int copyMaxVectorSize; ///< MIN_BENCHMARK_VECTOR_SIZE
  int copyIncFactor; ///< INCREMENT_FACTOR

  int vectorMinVectorSize; ///< MIN_BENCHMARK_VECTOR_SIZE
  int vectorMaxVectorSize; ///< MAX_BENCHMARK_VECTOR_SIZE
  int vectorIncFactor; ///< INCREMENT_FACTOR

  int blas3MatSizeA; ///< blas3MatrixSizeA
  int blas3MatSizeB; ///< blas3MatrixSizeB
  int blas3MatSizeC; ///< blas3MatrixSizeC

  int sparseMatSizeA; ///< xPoints
  int sparseMatSizeB; ///< yPoints
  QString sparseCustomMatrix; ///< customSparseMatrixPath

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
