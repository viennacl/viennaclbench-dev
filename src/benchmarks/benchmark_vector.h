#ifndef BENCHMARK_VECTOR_H
#define BENCHMARK_VECTOR_H

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/linalg/inner_prod.hpp"
#include "viennacl/linalg/norm_2.hpp"

#include <iostream>
#include <vector>
#include "benchmark-utils.hpp"


#include <QObject>

class Benchmark_Vector : public QObject
{
  Q_OBJECT
public:
  explicit Benchmark_Vector(QObject *parent = 0);

  template<typename ScalarType>
  void run_benchmark();
signals:
  void resultSignal(QString benchmarkName, double bandwidthValue);
  void benchmarkComplete();
public slots:
  void execute();

};

#endif // BENCHMARK_VECTOR_H
