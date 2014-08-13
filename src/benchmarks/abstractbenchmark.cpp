#include "abstractbenchmark.h"

AbstractBenchmark::AbstractBenchmark(QObject *parent) :
  QObject(parent)
{
}

void AbstractBenchmark::setPlatform(cl_platform_id)
{

}

void AbstractBenchmark::setDevice(cl_device_id)
{

}

void AbstractBenchmark::setPrecision(bool p)
{
  this->precision = p;
}

bool AbstractBenchmark::getPrecision()
{
  return this->precision;
}


//void AbstractBenchmark::run_benchmark()
//{

//}

//void AbstractBenchmark::test1()
//{

//}

//void AbstractBenchmark::execute()
//{

//}
