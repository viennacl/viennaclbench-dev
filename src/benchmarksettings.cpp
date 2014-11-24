#include "benchmarksettings.h"


/*!
 * \brief Default constructor. Loads default settings from \ref BenchmarkSettings::initBasicDefaults()
 */
BenchmarkSettings::BenchmarkSettings()
{
  initBasicDefaults();
}

/*!
 * \brief The default benchmark settings are initialized here.
 */
void BenchmarkSettings::initBasicDefaults(){
  copyMinVectorSize = 1000; //
  copyMaxVectorSize = 5120000;//
  copyIncFactor = 2; //size x2 each step

  vectorMinVectorSize = 1000; //
  vectorMaxVectorSize = 5120000;//
  vectorIncFactor = 2; //size x2 each step

  blas3MinSize = 128;
  blas3MaxSize = 2048;
  blas3IncFactor = 2;

  sparseMatSize = 300*300; // 90k unknowns should be sufficiently large to cover kernel launch latencies
  sparseCustomMatrix = QString("");//empty string for default matrix (generated on the fly)
}

/*!
 * \brief Leads settings from another object
 * \param s Settings from which to load
 */
void BenchmarkSettings::setSettings(BenchmarkSettings s){
  this->blas3MinSize = s.blas3MinSize;
  this->blas3MaxSize = s.blas3MaxSize;
  this->blas3IncFactor = s.blas3IncFactor;

  this->copyIncFactor = s.copyIncFactor;
  this->copyMaxVectorSize = s.copyMaxVectorSize;
  this->copyMinVectorSize = s.copyMinVectorSize;

  this->vectorMinVectorSize = s.vectorMinVectorSize;
  this->vectorMaxVectorSize = s.vectorMaxVectorSize;
  this->vectorIncFactor = s.vectorIncFactor;

  this->sparseMatSize = s.sparseMatSize;
  this->sparseCustomMatrix = s.sparseCustomMatrix;
}

/*!
 * \brief Sets Copy benchmark settings
 * \param min Vector min size
 * \param max Vector max size
 * \param increment Increment factor
 */
void BenchmarkSettings::setCopySettings(int min, int max, int increment)
{
  copyMinVectorSize = min;
  copyMaxVectorSize = max;
  copyIncFactor = increment;
}

/*!
 * \brief Sets Vector benchmark settings
 * \param min Vector min size
 * \param max Vector max size
 * \param increment Increment factor
 */
void BenchmarkSettings::setVectorSettings(int min, int max, int increment)
{
  vectorMinVectorSize = min;
  vectorMaxVectorSize = max;
  vectorIncFactor = increment;
}

/*!
 * \brief Sets Blas3 benchmark settings
 * \param minSize   Minimum matrix size
 * \param maxSize   Maximum matrix size
 * \param incFactor Increment factor (typically 2)
 */
void BenchmarkSettings::setBlas3Settings(int minSize, int maxSize, int incFactor)
{
  blas3MinSize = minSize;
  blas3MaxSize = maxSize;
  blas3IncFactor = incFactor;
}

/*!
 * \brief Sets Sparse benchmark settings
 * \param size  Approximate size of the sparse matrix (will be rounded to next square number)
 * \param pathToCustomMatrix Absolute path to cusotm matrix
 */
void BenchmarkSettings::setSparseSettings(int size, QString pathToCustomMatrix)
{
  sparseMatSize = size;
  sparseCustomMatrix = pathToCustomMatrix;
}
