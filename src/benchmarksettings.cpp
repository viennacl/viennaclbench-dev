#include "benchmarksettings.h"

//BenchmarkSettings::BenchmarkSettings() :
//  QObject(parent)
BenchmarkSettings::BenchmarkSettings()
{
  initBasicDefaults();
}

void BenchmarkSettings::initBasicDefaults(){
  copyMinVectorSize = 10000; //
  copyMaxVectorSize = 10240000;//
  copyIncFactor = 2; //size x2 each step

  vectorMinVectorSize = 10000; //
  vectorMaxVectorSize = 10240000;//
  vectorIncFactor = 2; //size x2 each step

  blas3MatSizeA = 1920;
  blas3MatSizeB = 1920;
  blas3MatSizeC = 1920;

  sparseMatSizeA = 200;
  sparseMatSizeB = 200;
  sparseCustomMatrix = QString("");//empty string for default matrix (generated on the fly)
}

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

BenchmarkSettings& BenchmarkSettings::operator =(const BenchmarkSettings& other)
{
  this->blas3MatSizeA = other.blas3MatSizeA;
  this->blas3MatSizeB = other.blas3MatSizeB;
  this->blas3MatSizeC = other.blas3MatSizeC;

  this->copyIncFactor = other.copyIncFactor;
  this->copyMaxVectorSize = other.copyMaxVectorSize;
  this->copyMinVectorSize = other.copyMinVectorSize;

  this->vectorMinVectorSize = other.vectorMinVectorSize;
  this->vectorMaxVectorSize = other.vectorMaxVectorSize;
  this->vectorIncFactor = other.vectorIncFactor;

  this->sparseMatSizeA = other.sparseMatSizeA;
  this->sparseMatSizeB = other.sparseMatSizeB;
  this->sparseCustomMatrix = other.sparseCustomMatrix;
  return *this;
}

void BenchmarkSettings::setCopySettings(int min, int max, int increment)
{
  copyMinVectorSize = min;
  copyMaxVectorSize = max;
  copyIncFactor = increment;
}

void BenchmarkSettings::setVectorSettings(int min, int max, int increment)
{
  vectorMinVectorSize = min;
  vectorMaxVectorSize = max;
  vectorIncFactor = increment;
}

void BenchmarkSettings::setBlas3Settings(int matA, int matB, int matC)
{
  blas3MatSizeA = matA;
  blas3MatSizeB = matB;
  blas3MatSizeC = matC;
}

void BenchmarkSettings::setSparseSettings(int matA, int matB, QString pathToCustomMatrix)
{
  sparseMatSizeA = matA;
  sparseMatSizeB = matB;
  sparseCustomMatrix = pathToCustomMatrix;
}
