/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include "matrixmarket_widget.h"
#include "ui_matrixmarket_widget.h"

/*!
 * \brief Default constructor. Inits UI and connects navigation buttons & progresbar.
 * \param parent Optional parent object.
 */
MatrixMarket_Widget::MatrixMarket_Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MatrixMarket_Widget)
{
  ui->setupUi(this);
  webView = ui->matrixMarket_WebView;
  //connect navigation buttons
  connect(ui->backButton, SIGNAL(clicked()), webView, SLOT(back()) );
  connect(ui->stopButton, SIGNAL(clicked()), webView, SLOT(stop()) );
  connect(ui->forwardButton, SIGNAL(clicked()), webView, SLOT(forward()) );
  //connect page load progress signals
  connect(webView, SIGNAL(loadProgress(int)), ui->progressBar, SLOT(setValue(int)) );
  connect(webView, SIGNAL(currentDownloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgressBar(qint64,qint64)) );
  //connect home button
  connect(ui->homeMMButton, SIGNAL(clicked()), this, SLOT(loadHomePage()) );
}

/*!
 * \brief Loads the home page.
 */
void MatrixMarket_Widget::loadHomePage(){
  webView->load(QUrl("qrc:///mmFiles/matrixmarket/index.html"));
}

/*!
 * \brief Updates the progressbar.
 * \param bytesReceived Progressbar current download size value
 * \param bytesTotal Progressbar max download size value
 */
void MatrixMarket_Widget::updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal){
  ui->progressBar->setValue((int)bytesReceived);
  ui->progressBar->setMaximum((int)bytesTotal);
}

/*!
 * \brief Destructor.
 */
MatrixMarket_Widget::~MatrixMarket_Widget()
{
  delete ui;
}
