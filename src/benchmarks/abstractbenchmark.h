/*! \file */
#ifndef ABSTRACTBENCHMARK_H
#define ABSTRACTBENCHMARK_H

#include <QObject>
#include <QString>

#include "../benchmarksettings.h"

/*!
  \enum Benchmark ID number.
*/
enum BenchmarkId { BLAS3,     ///< BLAS3 0
                   COPY,      ///< COPY 1
                   SPARSE,    ///< SPARSE 2
                   VECTOR,    ///< VECTOR 3
                   QR,        ///< QR 4
                   SOLVER,    ///< SOLVER 5
                   SCHEDULER, ///< SCHEDULER 6
                   ALL        ///< ALL 7 - marks all available benchmarks
                 };

/*!
 * \enum Benchmark precision.
 */
enum BenchmarkPrecision { SINGLE_PRECISION, ///< Single precision 0
                          DOUBLE_PRECISION  ///< Double precision 1
                        };

/*!
 * \enum Graph type to be used when plotting test results.
 */
enum GraphType { BAR_GRAPH, ///< Bar graph 0
                 LINE_GRAPH ///< Line graph 1
               };


/*!
 * \enum Benchmark mode.
 */
enum BenchmarkMode { BENCHMARK_MODE_BASIC, ///< Basic (standard) benchmark mode 0
                     BENCHMARK_MODE_EXPERT ///< Expert (advanced) benchmark mode 1
                   };

/*! \class AbstractBenchmark
 * \brief Abstract benchmark class defines common signal/slot functions inherited by all benchmark classes.
 * */
class AbstractBenchmark : public QObject
{
  Q_OBJECT
public:
  /*!
   * \brief Default constructor.
   * \param parent Parent object.
   */
  explicit AbstractBenchmark(QObject *parent = 0) :
    QObject(parent)
  {
  }
private:
  bool precision;
signals:
  /*!
   * \brief Emits an error message.
   * \param message The error message to be sent.
   */
  void errorMessage(QString message);

  /*!
   * \brief Emits the ID number of a benchmark that started its execution
   * \param benchmarkIdNumber The \ref BenchmarkId number
   */
  void benchmarkStarted(int benchmarkIdNumber);

  /*!
   * \brief Used to emit the final benchmark result (median value of all test results)
   * \param benchmarkName The benchmark name (e.g. "Copy")
   * \param finalValue The final result value
   */
  void finalResultSignal(QString benchmarkName, double finalValue);

  /*!
   * \brief Used to emit the result of benchmark tests.
   * \param testName The name of the completed test (e.g. ""Matrix-Matrix product")
   * \param key The key on x/y axis for which the result is to be displayed
   * \param value The test result value
   * \param graphType Which graph type is to be used when showing the result see \see GraphType ref \ref GraphType
   * \param testId Used to identify the test result so that it can receive "special" graphing treatment (different graph color)
   */
  void resultSignal(QString testName, double key, double value, int graphType, int testId);

  /*!
   * \brief Indicates a benchmark has completed all of its tests.
   */
  void benchmarkComplete();

  /*!
   * \brief Emits the measure a benchmark will use and axis on which the measure is to be shown.
   * \param unitMeasureName The name of the measure (e.g. "GB/s")
   * \param axis The axis on which to show the measure (Qt::XAxis/Qt::YAxis)
   */
  void unitMeasureSignal(QString unitMeasureName, int axis);

  /*!
   * \brief Used to emit a signal when a benchmark test was completed.
   */
  void testProgress();
public slots:
  /*!
   * \brief Sets the precision to be used by the benchmark.
   * \param p Precision to be set (false - single, true - double);
   */
  void setPrecision(bool p)
  {
    this->precision = p;
  }

  /*!
   * \brief Benchmark's current precision setting.
   * \return Returns the benchmark's precision setting.
   */
  bool getPrecision()
  {
    return this->precision;
  }

  /*!
   * \brief Begins the benchmark execution.
   * This function is the common way all benchmarks are started. Each benchmark implements its own version of this function.
   * It should present itself as the primary way of starting a benchmark.
   * In general, this function will perform neccessary preparations and eventually call run_benchmark() function.
   */
  virtual void execute() = 0;
};

#endif // ABSTRACTBENCHMARK_H
