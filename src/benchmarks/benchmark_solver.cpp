///* =========================================================================
//   Copyright (c) 2010-2014, Institute for Microelectronics,
//                            Institute for Analysis and Scientific Computing,
//                            TU Wien.
//   Portions of this software are copyright by UChicago Argonne, LLC.

//                            -----------------
//                  ViennaCL - The Vienna Computing Library
//                            -----------------

//   Project Head:    Karl Rupp                   rupp@iue.tuwien.ac.at

//   (A list of authors and contributors can be found in the PDF manual)

//   License:         MIT (X11), see file LICENSE in the base directory
//============================================================================= */

///*
//*
//*   Benchmark:  Iterative solver tests (solver.cpp and solver.cu are identical, the latter being required for compilation using CUDA nvcc)
//*
//*/

//#include "benchmark_solver.h"

//Benchmark_Solver::Benchmark_Solver(QObject *parent) :
//  AbstractBenchmark(parent)
//{
//  finalResultCounter = 0;
//  finalResultValue = 0;
//}

//template <typename ScalarType>
//ScalarType Benchmark_Solver::diff_inf(ublas::vector<ScalarType> & v1, viennacl::vector<ScalarType> & v2)
//{
//  ublas::vector<ScalarType> v2_cpu(v2.size());
//  viennacl::copy(v2.begin(), v2.end(), v2_cpu.begin());

//  for (unsigned int i=0;i<v1.size(); ++i)
//  {
//    if ( std::max( fabs(v2_cpu[i]), fabs(v1[i]) ) > 0 )
//      v2_cpu[i] = fabs(v2_cpu[i] - v1[i]) / std::max( fabs(v2_cpu[i]), fabs(v1[i]) );
//    else
//      v2_cpu[i] = 0.0;
//  }

//  return norm_inf(v2_cpu);
//}

//template <typename ScalarType>
//ScalarType Benchmark_Solver::diff_2(ublas::vector<ScalarType> & v1, viennacl::vector<ScalarType> & v2)
//{
//  ublas::vector<ScalarType> v2_cpu(v2.size());
//  viennacl::copy(v2.begin(), v2.end(), v2_cpu.begin());

//  return norm_2(v1 - v2_cpu) / norm_2(v1);
//}

//template <typename MatrixType, typename VectorType, typename SolverTag, typename PrecondTag>
//double Benchmark_Solver::run_solver(MatrixType const & matrix, VectorType const & rhs, VectorType const & ref_result, SolverTag const & solver, PrecondTag const & precond, long ops)
//{
//  double GFLOPs=0;
//  Timer timer;
//  VectorType result(rhs);
//  VectorType residual(rhs);
//  viennacl::backend::finish();
//  std::cout << "starting timer"<< std::endl;

//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//  {
//    result = viennacl::linalg::solve(matrix, rhs, solver, precond);
//  }
//  viennacl::backend::finish();

//  double exec_time = timer.get();
//  std::cout << "Exec. time: " << exec_time << std::endl;
//  std::cout << "Est. "; GFLOPs = printOps(static_cast<double>(ops), exec_time / BENCHMARK_RUNS);
//  residual -= viennacl::linalg::prod(matrix, result);
//  std::cout << "Relative residual: " << viennacl::linalg::norm_2(residual) / viennacl::linalg::norm_2(rhs) << std::endl;
//  std::cout << "Estimated rel. residual: " << solver.error() << std::endl;
//  std::cout << "Iterations: " << solver.iters() << std::endl;
//  result -= ref_result;
//  std::cout << "Relative deviation from result: " << viennacl::linalg::norm_2(result) / viennacl::linalg::norm_2(ref_result) << std::endl;
//  return GFLOPs;
//}

//template<typename ScalarType>
//void  Benchmark_Solver::run_benchmark(viennacl::context ctx)
//{
//  Timer timer;
//  double exec_time;

//  ScalarType std_factor1 = static_cast<ScalarType>(3.1415);
//  ScalarType std_factor2 = static_cast<ScalarType>(42.0);
//  viennacl::scalar<ScalarType> vcl_factor1(std_factor1, ctx);
//  viennacl::scalar<ScalarType> vcl_factor2(std_factor2, ctx);

//  ublas::vector<ScalarType> ublas_vec1;
//  ublas::vector<ScalarType> ublas_vec2;
//  ublas::vector<ScalarType> ublas_result;
//  unsigned int solver_iters = 100;
//  unsigned int solver_krylov_dim = 20;
//  double solver_tolerance = 1e-6;

//  //create paths to data files, qt automatically takes care of / and \ characters
//  QString  absoluteAppRootPath = QDir::currentPath();
//  QString rhsPathString = absoluteAppRootPath + "/testdata/rhs65025.txt";
//  QString resultPathString = absoluteAppRootPath + "/testdata/result65025.txt";
//  QString matrixPathString = absoluteAppRootPath + "/testdata/mat65k.mtx";

//  if (!readVectorFromFile<ScalarType>( rhsPathString , ublas_vec1))
//  {
//    std::cout << "Error reading RHS file" << std::endl;
//    return;
//  }

//  std::cout << "done reading rhs" << std::endl;
//  ublas_vec2 = ublas_vec1;

//  if (!readVectorFromFile<ScalarType>( resultPathString , ublas_result))
//  {
//    std::cout << "Error reading result file" << std::endl;
//    return;
//  }
//  std::cout << "done reading result" << std::endl;

//  viennacl::compressed_matrix<ScalarType> vcl_compressed_matrix(ublas_vec1.size(), ublas_vec1.size(), ctx);
//  viennacl::coordinate_matrix<ScalarType> vcl_coordinate_matrix(ublas_vec1.size(), ublas_vec1.size(), ctx);
//  viennacl::ell_matrix<ScalarType> vcl_ell_matrix(ctx);
//  viennacl::hyb_matrix<ScalarType> vcl_hyb_matrix(ctx);

//  viennacl::vector<ScalarType> vcl_vec1(ublas_vec1.size(), ctx);
//  viennacl::vector<ScalarType> vcl_vec2(ublas_vec1.size(), ctx);
//  viennacl::vector<ScalarType> vcl_result(ublas_vec1.size(), ctx);

//  ublas::compressed_matrix<ScalarType> ublas_matrix;

//  long matrixFileReadStatus = viennacl::io::read_matrix_market_file(ublas_matrix, matrixPathString.toStdString() );

//  if ( matrixFileReadStatus != 0)
//  {
//    std::cout << "matrix file read successfully, lines read:"<< matrixFileReadStatus << std::endl;
//  }
//  else
//  {
//    std::cout << "Error reading Matrix file, status:" << matrixFileReadStatus << std::endl;
//  }

//  std::cout << "done reading matrix" << std::endl;

//  //cpu to gpu:
//  viennacl::copy(ublas_matrix, vcl_compressed_matrix);
//  viennacl::copy(ublas_matrix, vcl_coordinate_matrix);
//  viennacl::copy(ublas_matrix, vcl_ell_matrix);
//  viennacl::copy(ublas_matrix, vcl_hyb_matrix);
//  viennacl::copy(ublas_vec1, vcl_vec1);
//  viennacl::copy(ublas_vec2, vcl_vec2);
//  viennacl::copy(ublas_result, vcl_result);


//  std::cout << "------- Jacobi preconditioner ----------" << std::endl;
//  viennacl::linalg::jacobi_precond< ublas::compressed_matrix<ScalarType> >    ublas_jacobi(ublas_matrix, viennacl::linalg::jacobi_tag());
//  viennacl::linalg::jacobi_precond< viennacl::compressed_matrix<ScalarType> > vcl_jacobi_csr(vcl_compressed_matrix, viennacl::linalg::jacobi_tag());
//  viennacl::linalg::jacobi_precond< viennacl::coordinate_matrix<ScalarType> > vcl_jacobi_coo(vcl_coordinate_matrix, viennacl::linalg::jacobi_tag());

//  std::cout << "------- Row-Scaling preconditioner ----------" << std::endl;
//  viennacl::linalg::row_scaling< ublas::compressed_matrix<ScalarType> >    ublas_row_scaling(ublas_matrix, viennacl::linalg::row_scaling_tag(1));
//  viennacl::linalg::row_scaling< viennacl::compressed_matrix<ScalarType> > vcl_row_scaling_csr(vcl_compressed_matrix, viennacl::linalg::row_scaling_tag(1));
//  viennacl::linalg::row_scaling< viennacl::coordinate_matrix<ScalarType> > vcl_row_scaling_coo(vcl_coordinate_matrix, viennacl::linalg::row_scaling_tag(1));

//  ///////////////////////////////////////////////////////////////////////////////
//  //////////////////////  Incomplete Cholesky preconditioner   //////////////////
//  ///////////////////////////////////////////////////////////////////////////////
//  std::cout << "------- ICHOL0 on CPU (ublas) ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::ichol0_precond< ublas::compressed_matrix<ScalarType> >    ublas_ichol0(ublas_matrix, viennacl::linalg::ichol0_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time: " << exec_time << std::endl;

//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    ublas_ichol0.apply(ublas_vec1);
//  exec_time = timer.get();
//  std::cout << "ublas time: " << exec_time << std::endl;

//  std::cout << "------- ICHOL0 with ViennaCL ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::ichol0_precond< viennacl::compressed_matrix<ScalarType> > vcl_ichol0(vcl_compressed_matrix, viennacl::linalg::ichol0_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time: " << exec_time << std::endl;

//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_ichol0.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL time: " << exec_time << std::endl;


//  ///////////////////////////////////////////////////////////////////////////////
//  //////////////////////           ILU preconditioner         //////////////////
//  ///////////////////////////////////////////////////////////////////////////////
//  std::cout << "------- ILU0 on with ublas ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::ilu0_precond< ublas::compressed_matrix<ScalarType> >    ublas_ilu0(ublas_matrix, viennacl::linalg::ilu0_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time (no level scheduling): " << exec_time << std::endl;
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    ublas_ilu0.apply(ublas_vec1);
//  exec_time = timer.get();
//  std::cout << "ublas ILU0 substitution time (no level scheduling): " << exec_time << std::endl;


//  std::cout << "------- ILU0 with ViennaCL ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::ilu0_precond< viennacl::compressed_matrix<ScalarType> > vcl_ilu0(vcl_compressed_matrix, viennacl::linalg::ilu0_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time (no level scheduling): " << exec_time << std::endl;

//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_ilu0.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL ILU0 substitution time (no level scheduling): " << exec_time << std::endl;

//  timer.start();
//  viennacl::linalg::ilu0_tag ilu0_with_level_scheduling; ilu0_with_level_scheduling.use_level_scheduling(true);
//  viennacl::linalg::ilu0_precond< viennacl::compressed_matrix<ScalarType> > vcl_ilu0_level_scheduling(vcl_compressed_matrix, ilu0_with_level_scheduling);
//  exec_time = timer.get();
//  std::cout << "Setup time (with level scheduling): " << exec_time << std::endl;

//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_ilu0_level_scheduling.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL ILU0 substitution time (with level scheduling): " << exec_time << std::endl;



//  ////////////////////////////////////////////

//  std::cout << "------- Block-ILU0 with ublas ----------" << std::endl;

//  ublas_vec1 = ublas_vec2;
//  viennacl::copy(ublas_vec1, vcl_vec1);

//  timer.start();
//  viennacl::linalg::block_ilu_precond< ublas::compressed_matrix<ScalarType>,
//      viennacl::linalg::ilu0_tag>          ublas_block_ilu0(ublas_matrix, viennacl::linalg::ilu0_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time: " << exec_time << std::endl;

//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    ublas_block_ilu0.apply(ublas_vec1);
//  exec_time = timer.get();
//  std::cout << "ublas time: " << exec_time << std::endl;

//  std::cout << "------- Block-ILU0 with ViennaCL ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::block_ilu_precond< viennacl::compressed_matrix<ScalarType>,
//      viennacl::linalg::ilu0_tag>          vcl_block_ilu0(vcl_compressed_matrix, viennacl::linalg::ilu0_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time: " << exec_time << std::endl;

//  //vcl_block_ilu0.apply(vcl_vec1);  //warm-up
//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_block_ilu0.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL time: " << exec_time << std::endl;

//  ////////////////////////////////////////////

//  std::cout << "------- ILUT with ublas ----------" << std::endl;

//  ublas_vec1 = ublas_vec2;
//  viennacl::copy(ublas_vec1, vcl_vec1);

//  timer.start();
//  viennacl::linalg::ilut_precond< ublas::compressed_matrix<ScalarType> >    ublas_ilut(ublas_matrix, viennacl::linalg::ilut_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time (no level scheduling): " << exec_time << std::endl;
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    ublas_ilut.apply(ublas_vec1);
//  exec_time = timer.get();
//  std::cout << "ublas ILUT substitution time (no level scheduling): " << exec_time << std::endl;


//  std::cout << "------- ILUT with ViennaCL ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::ilut_precond< viennacl::compressed_matrix<ScalarType> > vcl_ilut(vcl_compressed_matrix, viennacl::linalg::ilut_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time (no level scheduling): " << exec_time << std::endl;

//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_ilut.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL ILUT substitution time (no level scheduling): " << exec_time << std::endl;

//  timer.start();
//  viennacl::linalg::ilut_tag ilut_with_level_scheduling; ilut_with_level_scheduling.use_level_scheduling(true);
//  viennacl::linalg::ilut_precond< viennacl::compressed_matrix<ScalarType> > vcl_ilut_level_scheduling(vcl_compressed_matrix, ilut_with_level_scheduling);
//  exec_time = timer.get();
//  std::cout << "Setup time (with level scheduling): " << exec_time << std::endl;

//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_ilut_level_scheduling.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL ILUT substitution time (with level scheduling): " << exec_time << std::endl;


//  ////////////////////////////////////////////

//  std::cout << "------- Block-ILUT with ublas ----------" << std::endl;

//  ublas_vec1 = ublas_vec2;
//  viennacl::copy(ublas_vec1, vcl_vec1);

//  timer.start();
//  viennacl::linalg::block_ilu_precond< ublas::compressed_matrix<ScalarType>,
//      viennacl::linalg::ilut_tag>          ublas_block_ilut(ublas_matrix, viennacl::linalg::ilut_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time: " << exec_time << std::endl;

//  //ublas_block_ilut.apply(ublas_vec1);
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    ublas_block_ilut.apply(ublas_vec1);
//  exec_time = timer.get();
//  std::cout << "ublas time: " << exec_time << std::endl;

//  std::cout << "------- Block-ILUT with ViennaCL ----------" << std::endl;

//  timer.start();
//  viennacl::linalg::block_ilu_precond< viennacl::compressed_matrix<ScalarType>,
//      viennacl::linalg::ilut_tag>          vcl_block_ilut(vcl_compressed_matrix, viennacl::linalg::ilut_tag());
//  exec_time = timer.get();
//  std::cout << "Setup time: " << exec_time << std::endl;

//  //vcl_block_ilut.apply(vcl_vec1);  //warm-up
//  viennacl::backend::finish();
//  timer.start();
//  for (int runs=0; runs<BENCHMARK_RUNS; ++runs)
//    vcl_block_ilut.apply(vcl_vec1);
//  viennacl::backend::finish();
//  exec_time = timer.get();
//  std::cout << "ViennaCL time: " << exec_time << std::endl;


//  ///////////////////////////////////////////////////////////////////////////////
//  //////////////////////              CG solver                //////////////////
//  ///////////////////////////////////////////////////////////////////////////////
//  double totalGFLOPs=0;
//  int counter=0;
//  long cg_ops = static_cast<long>(solver_iters * (ublas_matrix.nnz() + 6 * ublas_vec2.size()));

//  viennacl::linalg::cg_tag cg_solver(solver_tolerance, solver_iters);

//  std::cout << "------- CG solver (no preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, viennacl::linalg::no_precond(), cg_ops);
//  counter++;

//  std::cout << "------- CG solver (no preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, viennacl::linalg::no_precond(), cg_ops);
//  counter++;


//#ifdef VIENNACL_WITH_OPENCL
//  if (sizeof(ScalarType) == sizeof(double))
//  {
//    std::cout << "------- CG solver, mixed precision (no preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//    viennacl::linalg::mixed_precision_cg_tag mixed_precision_cg_solver(solver_tolerance, solver_iters);

//    totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, mixed_precision_cg_solver, viennacl::linalg::no_precond(), cg_ops);
//    counter++;

//    totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, mixed_precision_cg_solver, viennacl::linalg::no_precond(), cg_ops);
//    counter++;

//  }
//#endif

//  std::cout << "------- CG solver (no preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, cg_solver, viennacl::linalg::no_precond(), cg_ops);
//  counter++;


//  std::cout << "------- CG solver (no preconditioner) via ViennaCL, ell_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_ell_matrix, vcl_vec2, vcl_result, cg_solver, viennacl::linalg::no_precond(), cg_ops);
//  counter++;


//  std::cout << "------- CG solver (no preconditioner) via ViennaCL, hyb_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_hyb_matrix, vcl_vec2, vcl_result, cg_solver, viennacl::linalg::no_precond(), cg_ops);
//  counter++;


//  std::cout << "------- CG solver (ICHOL0 preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_ichol0, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (ICHOL0 preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_ichol0, cg_ops);
//  counter++;



//  std::cout << "------- CG solver (ILU0 preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_ilu0, cg_ops);
//  counter++;

//  //The Heisenbug
//  std::cout << "------- CG solver (ILU0 preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_ilu0, cg_ops);
//  counter++;



//  std::cout << "------- CG solver (Block-ILU0 preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_block_ilu0, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (Block-ILU0 preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_block_ilu0, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (ILUT preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_ilut, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (ILUT preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_ilut, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (ILUT preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, cg_solver, vcl_ilut, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (Block-ILUT preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_block_ilut, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (Block-ILUT preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_block_ilut, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (Jacobi preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_jacobi, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (Jacobi preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_jacobi_csr, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (Jacobi preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, cg_solver, vcl_jacobi_coo, cg_ops);
//  counter++;



//  std::cout << "------- CG solver (row scaling preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, cg_solver, ublas_row_scaling, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (row scaling preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, cg_solver, vcl_row_scaling_csr, cg_ops);
//  counter++;


//  std::cout << "------- CG solver (row scaling preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, cg_solver, vcl_row_scaling_coo, cg_ops);
//  counter++;

//  emit resultSignal("CG solver - average", totalGFLOPs/(double)counter);
//  finalResultValue += totalGFLOPs/(double)counter;
//  finalResultCounter++;

//  ///////////////////////////////////////////////////////////////////////////////
//  //////////////////////           BiCGStab solver             //////////////////
//  ///////////////////////////////////////////////////////////////////////////////

//  totalGFLOPs += 0;
//  counter = 0;

//  long bicgstab_ops = static_cast<long>(solver_iters * (2 * ublas_matrix.nnz() + 13 * ublas_vec2.size()));

//  viennacl::linalg::bicgstab_tag bicgstab_solver(solver_tolerance, solver_iters);

//  std::cout << "------- BiCGStab solver (no preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, bicgstab_solver, viennacl::linalg::no_precond(), bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (no preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, bicgstab_solver, viennacl::linalg::no_precond(), bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (no preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, bicgstab_solver, viennacl::linalg::no_precond(), bicgstab_ops);
//  counter++;


//  std::cout << "------- BiCGStab solver (ILUT preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, bicgstab_solver, ublas_ilut, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (ILUT preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_ilut, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (Block-ILUT preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, bicgstab_solver, ublas_block_ilut, bicgstab_ops);
//  counter++;

//#ifdef VIENNACL_WITH_OPENCL
//  std::cout << "------- BiCGStab solver (Block-ILUT preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_block_ilut, bicgstab_ops);
//  counter++;
//#endif

//  std::cout << "------- BiCGStab solver (ILUT preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_ilut, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (Jacobi preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, bicgstab_solver, ublas_jacobi, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (Jacobi preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_jacobi_csr, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (Jacobi preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_jacobi_coo, bicgstab_ops);
//  counter++;


//  std::cout << "------- BiCGStab solver (row scaling preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, bicgstab_solver, ublas_row_scaling, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (row scaling preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_row_scaling_csr, bicgstab_ops);
//  counter++;

//  std::cout << "------- BiCGStab solver (row scaling preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, bicgstab_solver, vcl_row_scaling_coo, bicgstab_ops);
//  counter++;

//  emit resultSignal("BiCGStab solver - average", totalGFLOPs/(double)counter);
//  finalResultValue += totalGFLOPs/(double)counter;
//  finalResultCounter++;

//  ///////////////////////////////////////////////////////////////////////////////
//  ///////////////////////            GMRES solver             ///////////////////
//  ///////////////////////////////////////////////////////////////////////////////

//  totalGFLOPs += 0;
//  counter = 0;
//  long gmres_ops = static_cast<long>(solver_iters * (ublas_matrix.nnz() + (solver_iters * 2 + 7) * ublas_vec2.size()));

//  viennacl::linalg::gmres_tag gmres_solver(solver_tolerance, solver_iters, solver_krylov_dim);

//  std::cout << "------- GMRES solver (no preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, gmres_solver, viennacl::linalg::no_precond(), gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (no preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, gmres_solver, viennacl::linalg::no_precond(), gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (no preconditioner) on GPU, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, gmres_solver, viennacl::linalg::no_precond(), gmres_ops);
//  counter++;


//  std::cout << "------- GMRES solver (ILUT preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, gmres_solver, ublas_ilut, gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (ILUT preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, gmres_solver, vcl_ilut, gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (ILUT preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, gmres_solver, vcl_ilut, gmres_ops);
//  counter++;


//  std::cout << "------- GMRES solver (Jacobi preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, gmres_solver, ublas_jacobi, gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (Jacobi preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, gmres_solver, vcl_jacobi_csr, gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (Jacobi preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, gmres_solver, vcl_jacobi_coo, gmres_ops);
//  counter++;


//  std::cout << "------- GMRES solver (row scaling preconditioner) using ublas ----------" << std::endl;
//  totalGFLOPs += run_solver(ublas_matrix, ublas_vec2, ublas_result, gmres_solver, ublas_row_scaling, gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (row scaling preconditioner) via ViennaCL, compressed_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_compressed_matrix, vcl_vec2, vcl_result, gmres_solver, vcl_row_scaling_csr, gmres_ops);
//  counter++;

//  std::cout << "------- GMRES solver (row scaling preconditioner) via ViennaCL, coordinate_matrix ----------" << std::endl;
//  totalGFLOPs += run_solver(vcl_coordinate_matrix, vcl_vec2, vcl_result, gmres_solver, vcl_row_scaling_coo, gmres_ops);
//  counter++;

//  emit resultSignal("GMRES solver - average", totalGFLOPs/(double)counter);
//  finalResultValue += totalGFLOPs/(double)counter;
//  finalResultCounter++;

//  //  return EXIT_SUCCESS;
//}

//void Benchmark_Solver::execute()
//{
//  emit benchmarkStarted(SOLVER);
//  emit unitMeasureSignal("GFLOPs");
//  std::cout << std::endl;
//  std::cout << "----------------------------------------------" << std::endl;
//  std::cout << "               Device Info" << std::endl;
//  std::cout << "----------------------------------------------" << std::endl;

//#ifdef VIENNACL_WITH_OPENCL
//  viennacl::ocl::platform pf = viennacl::ocl::get_platforms()[0];
//  std::vector<viennacl::ocl::device> const & devices = pf.devices();

//  // Set first device to first context:
//  viennacl::ocl::setup_context(0, devices[0]);

//  // Set second device for second context (use the same device for the second context if only one device available):
//  if (devices.size() > 1)
//    viennacl::ocl::setup_context(1, devices[1]);
//  else
//    viennacl::ocl::setup_context(1, devices[0]);

//  std::cout << viennacl::ocl::current_device().info() << std::endl;
//  viennacl::context ctx(viennacl::ocl::get_context(1));
//#else
//  viennacl::context ctx;
//#endif

//  std::cout << "---------------------------------------------------------------------------" << std::endl;
//  std::cout << "---------------------------------------------------------------------------" << std::endl;
//  std::cout << " Benchmark for Execution Times of Iterative Solvers provided with ViennaCL " << std::endl;
//  std::cout << "---------------------------------------------------------------------------" << std::endl;
//  std::cout << " Note that the purpose of this benchmark is not to run solvers until" << std::endl;
//  std::cout << " convergence. Instead, only the execution times of a few iterations are" << std::endl;
//  std::cout << " recorded. Residual errors are only printed for information." << std::endl << std::endl;


//  std::cout << std::endl;
//  std::cout << "----------------------------------------------" << std::endl;
//  std::cout << "----------------------------------------------" << std::endl;
//  std::cout << "## Benchmark :: Solver" << std::endl;
//  std::cout << "----------------------------------------------" << std::endl;
//  std::cout << std::endl;
//  std::cout << "   -------------------------------" << std::endl;
//  std::cout << "   # benchmarking single-precision" << std::endl;
//  std::cout << "   -------------------------------" << std::endl;
//  run_benchmark<float>(ctx);
//#ifdef VIENNACL_WITH_OPENCL
//  if( viennacl::ocl::current_device().double_support() )
//#endif
//  {
//    std::cout << std::endl;
//    std::cout << "   -------------------------------" << std::endl;
//    std::cout << "   # benchmarking double-precision" << std::endl;
//    std::cout << "   -------------------------------" << std::endl;
//    run_benchmark<double>(ctx);
//  }
//  emit finalResultSignal("Solver", finalResultValue/finalResultCounter);
//  emit benchmarkComplete();
//}
