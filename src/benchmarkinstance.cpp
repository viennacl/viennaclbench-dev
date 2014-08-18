#include "benchmarkinstance.h"

//BenchmarkInstance::BenchmarkInstance() :
//  QObject(parent)
BenchmarkInstance::BenchmarkInstance()
{
  blas3Result = 0;
  copyResult = 0;
  sparseResult = 0;
  vectorResult = 0;
  mode = "Standard";
  full = true;
  totalScore = 0;
  precision = "Double";
}

void BenchmarkInstance::setSettings(BenchmarkSettings s)
{
  this->settings.blas3MatSizeA = s.blas3MatSizeA;
  this->settings.blas3MatSizeB = s.blas3MatSizeB;
  this->settings.blas3MatSizeC = s.blas3MatSizeC;

  this->settings.copyIncFactor = s.copyIncFactor;
  this->settings.copyMaxVectorSize = s.copyMaxVectorSize;
  this->settings.copyMinVectorSize = s.copyMinVectorSize;

  this->settings.vectorMinVectorSize = s.vectorMinVectorSize;
  this->settings.vectorMaxVectorSize = s.vectorMaxVectorSize;
  this->settings.vectorIncFactor = s.vectorIncFactor;

  this->settings.sparseMatSizeA = s.sparseMatSizeA;
  this->settings.sparseMatSizeB = s.sparseMatSizeB;
  this->settings.sparseCustomMatrix = s.sparseCustomMatrix;

}
