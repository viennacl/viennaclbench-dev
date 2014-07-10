/* =========================================================================
   Copyright (c) 2010-2014, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.
   Portions of this software are copyright by UChicago Argonne, LLC.

                            -----------------
                  ViennaCL - The Vienna Computing Library
                            -----------------

   Project Head:    Karl Rupp                   rupp@iue.tuwien.ac.at

   (A list of authors and contributors can be found in the PDF manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

/*
*
*   Benchmark:   Performance of viennacl::linalg::inplace_qr(A);
*
*/

#include "benchmark_qr.h"

Benchmark_Qr::Benchmark_Qr(QObject *parent) :
  AbstractBenchmark(parent)
{
  finalResultCounter = 0;
  finalResultValue = 0;
}

void Benchmark_Qr::run_benchmark()
{
  typedef float               ScalarType;
  typedef boost::numeric::ublas::matrix<ScalarType, boost::numeric::ublas::column_major>        MatrixType;
  typedef boost::numeric::ublas::vector<ScalarType>                   VectorType;
  Timer timer;
  double elapsed;

  std::size_t rows = 1800;
  std::size_t cols = 1800;
  double num_ops_qr = 2.0 * cols * cols * (rows - cols/3.0);
  double num_ops_recovery = 4.0 * (rows*rows*cols - rows*cols*cols + cols*cols*cols);

  MatrixType A(rows, cols);
  MatrixType Q(rows, rows);
  MatrixType R(rows, cols);

  for (std::size_t i=0; i<rows; ++i)
  {
    for (std::size_t j=0; j<cols; ++j)
    {
      A(i,j) = 1.0 + (i + 1)*(j+1);
      R(i,j) = 0.0;
    }
    for (std::size_t j=0; j<rows; ++j)
    {
      Q(i,j) = 0.0;
    }
  }

  //std::cout << "A: " << A << std::endl;
  timer.start();
  //    std::vector<ScalarType> betas = viennacl::linalg::block_qr(A);
  //  std::cout << "init betas" << std::endl;
  std::vector<ScalarType> betas;
  //  std::cout << "init betas done" << std::endl;
  //  std::cout << "time before inplace_qr:" << QTime::currentTime().toString().toStdString() << std::endl;
  betas = viennacl::linalg::inplace_qr(A);
  //  std::cout << "inplace_qr done" << std::endl;
  //  std::cout << "time after inplace_qr:" << QTime::currentTime().toString().toStdString() << std::endl;
  elapsed = timer.get();
  std::cout << "Time for QR on CPU: " << elapsed << std::endl;
  std::cout << "Estimated GFLOPs: " << 2e-9 * num_ops_qr/ elapsed << std::endl;
  emit resultSignal("QR on CPU", 2e-9 * num_ops_qr/ elapsed );
  finalResultValue += 2e-9 * num_ops_qr/ elapsed;
  finalResultCounter++;



  //std::cout << "Inplace QR-factored A: " << A << std::endl;

  timer.start();
  viennacl::linalg::recoverQ(A, betas, Q, R);
  elapsed = timer.get();
  std::cout << "Time for Q-recovery on CPU: " << elapsed << std::endl;
  std::cout << "Estimated GFLOPs: " << 2e-9 * num_ops_recovery / elapsed << std::endl;
  emit resultSignal("Q-recovery on CPU", 2e-9 * num_ops_recovery / elapsed );
  finalResultValue += 2e-9 * num_ops_recovery / elapsed;
  finalResultCounter++;

  /*std::cout << "R after recovery: " << R << std::endl;
    std::cout << "Q after recovery: " << Q << std::endl;
    std::cout << "Q*Q^T: " << prod(Q, trans(Q)) << std::endl;

    std::cout << "Q * R: " << prod(Q, R) << std::endl;*/

  emit finalResultSignal("Qr", finalResultValue/finalResultCounter);
  emit benchmarkComplete();

}

void Benchmark_Qr::execute()
{
  emit benchmarkStarted(QR);
  emit unitMeasureSignal("GFLOPs");
  run_benchmark();
}
