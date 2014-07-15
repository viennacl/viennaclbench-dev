#include "matrixmarket_widget.h"
#include "ui_matrixmarket_widget.h"

MatrixMarket_Widget::MatrixMarket_Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MatrixMarket_Widget)
{
  ui->setupUi(this);
  webView = ui->matrixMarket_WebView;
//  ui->progressBar->hide();
  connect(ui->backButton, SIGNAL(clicked()), webView, SLOT(back()) );
  connect(ui->forwardButton, SIGNAL(clicked()), webView, SLOT(forward()) );
//  connect(webView, SIGNAL(loadStarted()), ui->progressBar, SLOT(show()) );
//  connect(webView, SIGNAL(loadFinished(bool)), ui->progressBar, SLOT(hide()) );
  connect(webView, SIGNAL(loadProgress(int)), ui->progressBar, SLOT(setValue(int)) );
  connect(webView, SIGNAL(currentDownloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgressBar(qint64,qint64)) );
//  connect(webView->currentDownload, SIGNAL())
  connect(ui->homeMMButton, SIGNAL(clicked()), this, SLOT(loadHomePage()) );
}

void MatrixMarket_Widget::loadHomePage(){
  webView->load(QUrl("http://www.cise.ufl.edu/research/sparse/matrices/"));
}

void MatrixMarket_Widget::updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal){
  ui->progressBar->setValue((int)bytesReceived);
  ui->progressBar->setMaximum((int)bytesTotal);
}

MatrixMarket_Widget::~MatrixMarket_Widget()
{
  delete ui;
}
