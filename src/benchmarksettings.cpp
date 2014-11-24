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

  blas3MatSizeA = 2048;
  blas3MatSizeB = 2048;
  blas3MatSizeC = 2048;

  sparseMatSizeA = 200;
  sparseMatSizeB = 200;
  sparseCustomMatrix = QString("");//empty string for default matrix (generated on the fly)
}

/*!
 * \brief Leads settings from another object
 * \param s Settings from which to load
 */
void BenchmarkSettings::setSettings(BenchmarkSettings s){
  this->blas3MatSizeA = s.blas3MatSizeA;
  this->blas3MatSizeB = s.blas3MatSizeB;
  this->blas3MatSizeC = s.blas3MatSizeC;

  this->copyIncFactor = s.copyIncFactor;
  this->copyMaxVectorSize = s.copyMaxVectorSize;
  this->copyMinVectorSize = s.copyMinVectorSize;

  this->vectorMinVectorSize = s.vectorMinVectorSize;
  this->vectorMaxVectorSize = s.vectorMaxVectorSize;
  this->vectorIncFactor = s.vectorIncFactor;

  this->sparseMatSizeA = s.sparseMatSizeA;
  this->sparseMatSizeB = s.sparseMatSizeB;
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
 * \param matA Matrix size1
 * \param matB Matrix size2
 * \param matC Matrix size3
 */
void BenchmarkSettings::setBlas3Settings(int matA, int matB, int matC)
{
  blas3MatSizeA = matA;
  blas3MatSizeB = matB;
  blas3MatSizeC = matC;
}

/*!
 * \brief Sets Sparse benchmark settings
 * \param matA Matrix size x
 * \param matB Matrix size y
 * \param pathToCustomMatrix Absolute path to cusotm matrix
 */
void BenchmarkSettings::setSparseSettings(int matA, int matB, QString pathToCustomMatrix)
{
  sparseMatSizeA = matA;
  sparseMatSizeB = matB;
  sparseCustomMatrix = pathToCustomMatrix;
}
