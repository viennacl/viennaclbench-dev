/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include "expertbenchmarkscreen.h"
#include "ui_expertbenchmarkscreen.h"

#include <QToolTip>

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
ExpertBenchmarkScreen::ExpertBenchmarkScreen(QWidget *parent) :
  BenchmarkScreen(parent),
  ui(new Ui::ExpertBenchmark)
{
  //--- IMPORTANT ---///
  //The UI form of this class needs to be initialized within a widget of the parent class (BenchmarkScreen).
  //This whole UI class is esentially placed within a widget (expertConfigPlaceholder) in the parent UI.
  ui->setupUi(expertConfigPlaceholder);

  initExpertSpecific();
}

/*!
 * \brief Destructor
 */
ExpertBenchmarkScreen::~ExpertBenchmarkScreen()
{
  delete ui;
}

/*!
 * \brief Initializes all benchmark plots.
 */
void ExpertBenchmarkScreen::initExpertSpecific(){
  loadDefaultSettings();
  //expert specific connections
  connect(ui->expert_CustomMatrixBrowsebutton, SIGNAL(clicked()), this, SLOT(setCustomSparseMatrixPath()) );
  connect(ui->expert_CustomMatrixDefaultButton, SIGNAL(clicked()), ui->expert_SparseCustomMatrix, SLOT(clear()) );

  connect(ui->expert_ResetSettingsButton, SIGNAL(clicked()), this, SLOT(loadDefaultSettings()) );
}

/*!
 * \brief Ensures all specified expert benchmark settings are valid.
 * \return Returns true if settings are valid, otherise false.
 */
bool ExpertBenchmarkScreen::validateSettings(){
  //expert specific
  if(ui->expert_Blas3AMatSize->value() <= 0) return false;
  if(ui->expert_Blas3BMatSize->value() <= 0) return false;
  if(ui->expert_Blas3CMatSize->value() <= 0) return false;

  if(ui->expert_CopyVecMin->value() <= 0) return false;
  if(ui->expert_CopyVecMax->value() <= 0) return false;
  if(ui->expert_CopyVecMin->value() >= ui->expert_CopyVecMax->value()) return false;

  if(ui->expert_CopyIncFactor->value() <= 1) return false;
  if(ui->expert_VectorVecMin->value() <= 0) return false;
  if(ui->expert_VectorVecMax->value() <= 0) return false;
  if(ui->expert_VectorVecMin->value() >= ui->expert_VectorVecMax->value()) return false;
  if(ui->expert_VectorIncFactor->value() <= 1) return false;

  if(ui->expert_SparseMatSize->value() <= 0) return false;

  return true;
}

/*!
 * \brief Collects expert benchmark settings from the UI.
 * \return Returns a \ref BenchmarkSettings object containing all expert settings.
 */
BenchmarkSettings ExpertBenchmarkScreen::getExpertSettings()
{
  BenchmarkSettings settings;

  //expert specific
  settings.blas3MinSize = ui->expert_Blas3AMatSize->value();
  settings.blas3MaxSize = ui->expert_Blas3BMatSize->value();
  settings.blas3IncFactor = ui->expert_Blas3CMatSize->value();

  settings.copyIncFactor = ui->expert_CopyIncFactor->value();
  settings.copyMaxVectorSize = ui->expert_CopyVecMax->value() * 1000;
  settings.copyMinVectorSize = ui->expert_CopyVecMin->value() * 1000;

  settings.vectorMinVectorSize = ui->expert_VectorVecMin->value() * 1000;
  settings.vectorMaxVectorSize = ui->expert_VectorVecMax->value() * 1000;
  settings.vectorIncFactor = ui->expert_VectorIncFactor->value();

  settings.sparseMatSize = ui->expert_SparseMatSize->value();
  settings.sparseCustomMatrix = ui->expert_SparseCustomMatrix->text();

  return settings;
}

void ExpertBenchmarkScreen::loadDefaultSettings(){
  BenchmarkSettings defaultSettings;

  //expert specific
  ui->expert_Blas3AMatSize->setValue(defaultSettings.blas3MinSize);
  ui->expert_Blas3BMatSize->setValue(defaultSettings.blas3MaxSize);
  ui->expert_Blas3CMatSize->setValue(defaultSettings.blas3IncFactor);

  ui->expert_CopyIncFactor->setValue(defaultSettings.copyIncFactor);
  ui->expert_CopyVecMax->setValue(defaultSettings.copyMaxVectorSize / 1000);
  ui->expert_CopyVecMin->setValue(defaultSettings.copyMinVectorSize / 1000);

  ui->expert_VectorVecMin->setValue(defaultSettings.vectorMinVectorSize / 1000);
  ui->expert_VectorVecMax->setValue(defaultSettings.vectorMaxVectorSize / 1000);
  ui->expert_VectorIncFactor->setValue(defaultSettings.vectorIncFactor);

  ui->expert_SparseMatSize->setValue(defaultSettings.sparseMatSize);
  ui->expert_SparseCustomMatrix->setText(defaultSettings.sparseCustomMatrix);
}

/*!
 * \brief Prepares the benchmark for execution with a custom sparse matrix
 * \param matrixFilePath Absolute path to the selected matrix file
 */
void ExpertBenchmarkScreen::setupCustomSparseMatrix(QString matrixFilePath)
{
  loadDefaultSettings();//use the default settings for custom matrix benchmarking
  //expert specific
  ui->expert_SparseCustomMatrix->setText(matrixFilePath);//use the custom matrix file
  selectSparseBenchmark();
}

/*!
 * \brief Opens a file dialog to choose a custom matrix for the sparse benchmark.
 */
void ExpertBenchmarkScreen::setCustomSparseMatrixPath(){
  ui->expert_SparseCustomMatrix->setText( QFileDialog::getOpenFileName(this,
                                                                       QString("Select a Custom Sparse Matrix"),
                                                                       ArchiveExtractor::getMatrixMarketUserFolder(),
                                                                       "Matrix Market Files (*.mtx)" )
                                          );
}
