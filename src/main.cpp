#include "ui/mainwindow.h"
#include "ui/splashscreen.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>


/*! \mainpage ViennaCL Benchmark 1.0.0 source code documentation.

  \section intro Welcome to ViennaCL Benchmark
  This is a cross-platform standalone benchmarking program for the <a href="http://viennacl.sourceforge.net/" target="_blank">ViennaCL</a> library.
  It was developed in Qt and supports two build systems, CMake and QMake.
  Its development began as part of 2014 Google Summer of Code.
  The main developer is <a href="http://zalomiga.ba/" target="_blank">Namik Karovic</a>.
  The source code repository can be found on <a href="https://github.com/viennacl/viennacl-benchmark-gui" target="_blank">GitHub</a>:
  You may use ViennaCL's <a href="mailto:viennacl-support@lists.sourceforge.net">mailing list</a> for any technical questions.

  \section namings Important namings and their meanings
  Before you begin, it is advisable to study the following list of namings.
  I prefer to keep things as clear as possible, which is why I think this is neccessary:
  \li Benchmark - one of the runnable benchmark classes (blas3, copy, sparse, vector)
  \li (Benchmark) test - an actual benchmarking procedure within a benchmark. A benchmark usually consists of multiple tests.
  \li Benchmark session - benchmarks selected for execution and the subsequent execution process after the "Start" button was clicked.
  Does not have an equivalent code representation, just mentioning it for clarity.
  \li Benchmark instance - a collection of information produced by the program after completing a benchmark session,
  as portrayed in the \ref BenchmarkInstance class. Used by the model to save/upload benchmark results.
  Contains a list of which benchmarks were executed, their results, and \ref BenchmarkSettings that were used.
  \li Basic (standard) mode - All benchmarks run in this mode will use the default \ref BenchmarkSettings .
  \li Expert (advanced) mode - Allows users to specify custom \ref BenchmarkSettings .

  \section mvc-backbone The MVC backbone
  The project was developed with aims to utilize the principles of the Model-View-Controller design pattern.
  Even though there is no direct database interaction (for now), the MVC pattern can still be applied.

  \li <b>View:</b> The view is comprised of all the classes capable of being rendered in the user interface.
  All of these classes are placed in the src/ui folder.
  The \ref MainWindow class serves as the link between the UI and the controller.
  All other classes are tied to the \ref MainWindow class, which propagates data from/to the controller.
  Some classes, namely \ref ResultHistory , communicate directly with the model without implicating the controller.

  \li <b>Controller:</b> The controller portion of the project is based around the \ref Benchmark_Controller class.
  In addition to the benchmark classes found in src/benchmarks folder, it represents the program's core benchmarking functionality.
  Its job is to control the execution of benchmarks and communicate results from/to the view.
  The following is a brief overview of how the controller works:<br>
  -The user orders the benchmark to start. <br>
  -All relevant \ref BenchmarkSettings are gathered from the view and passed onto the controller. <br>
  -The controller examines the received input and prepares for the execution process. <br>
  -Selected benchmarks are queued up and executed one by one, each in its own freshly created thread.
  The benchmark execution flow can be described as a chain reaction.
  Completion of one benchmark starts the execution of the next until there are no more queued benchmarks. <br>
  -Benchmark results are propagated to the view as soon as they are available. <br>
  -After each completed \ref BenchmarkInstance the controller is responsible for ordering the model to save/upload the results. <br>

  \li <b>Model:</b> The model functionality is provided by \ref Benchmark_Model class.
  It serves the purpose of storing and uploading results and settings of a benchmark instance.
  Results stored locally are in JSON format.
  They are loaded on startup and shown with the \ref ResultHistory class.
  Uploading to remote servers is currently not implemented.

  \section helper-classes Helper classes
  Besides the classes that represent the MVC pattern, there are other classes that are used as helpers.
  \li \ref BenchmarkInstance
  \li \ref BenchmarkSettings
  \li \ref ArchiveExtractor
 */

/*!
 * \brief Program entry point.
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  SplashScreen *splashScreen = new SplashScreen();
  splashScreen->setPixmap(QPixmap(":/resources/images/splash-screen.jpg"));
  splashScreen->show();
  splashScreen->showMessage("Loading GUI");
  qApp->processEvents();
  MainWindow w;
  QTimer::singleShot(2000, &w, SLOT(show()) );
  QTimer::singleShot(2000, splashScreen, SLOT(deleteLater()) );
  return app.exec();
}
