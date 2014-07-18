#include "matrixmarket_webview.h"
#include <QDebug>

MatrixMarket_WebView::MatrixMarket_WebView(QWidget *parent) :
  QWebView(parent)
{
  this->page()->setForwardUnsupportedContent(true);
  connect(this->page(), SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(downloadSlot(QNetworkRequest)) );
  connect(this->page(), SIGNAL(unsupportedContent(QNetworkReply*)), this, SLOT(unsupportedContentSlot(QNetworkReply*)) );
}

void MatrixMarket_WebView::processDownloadedFile(QNetworkReply* reply){
  QByteArray mmFile;
  mmFile = reply->readAll();
  if(mmFile.isEmpty()){
    qDebug()<<"Downloaded file is empty";
  }
  else{
    qDebug()<<"Download successful, bytes downloaded:"<<mmFile.size();
  }
  QString filename =  reply->url().toString();
  filename = filename.right(filename.length() - filename.lastIndexOf("/"));
  QString downloadFolder = QDir::currentPath() + "/MatrixMarket";

  //  qDebug()<<"Folder: "<<downloadFolder;
  //  qDebug()<<"Downloaded filename: "<<filename;
  QString fullPath = downloadFolder + filename;
  qDebug()<<"Fullpath: "<<fullPath;
  QFile mmFileToSave( fullPath );
  if(mmFileToSave.open(QIODevice::WriteOnly)){
    mmFileToSave.write(mmFile);
    qDebug()<<"File saved. Path: "<<fullPath;
    ArchiveExtractor::extractFile(fullPath);
  }
  else{
    qDebug()<<"Cannot save file";
    qDebug()<<mmFileToSave.errorString();
  }
  reply->deleteLater();
}

void MatrixMarket_WebView::downloadSlot(QNetworkRequest request)
{
  qDebug()<<"Download requested: "<<request.url();
  downloadManager = new QNetworkAccessManager();
  currentDownload = downloadManager->get(request);

  connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(currentDownloadProgressSlot(qint64,qint64)) );
  connect(currentDownload, SIGNAL(finished()), currentDownload, SLOT(deleteLater()) );
  connect(downloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processDownloadedFile(QNetworkReply*)) );
  connect(downloadManager, SIGNAL(finished()), downloadManager, SLOT(deleteLater()) );
}

void MatrixMarket_WebView::currentDownloadProgressSlot(qint64 bytesReceived, qint64 bytesTotal){
  emit currentDownloadProgress(bytesReceived, bytesTotal);
}

void MatrixMarket_WebView::unsupportedContentSlot(QNetworkReply *reply)
{
  QString contentUrl = reply->url().toString();
  qDebug()<<"Unsupported content requested: "<<contentUrl;
  if(contentUrl.contains("/MM/", Qt::CaseSensitive) && contentUrl.endsWith(".tar.gz")){
    qDebug()<<"Content type: .tar.gz";
    downloadSlot(reply->request());
  }
  reply->deleteLater();
}

void MatrixMarket_WebView::mouseReleaseEvent(QMouseEvent *e)
{
  e->accept();

  // Qt4 uses XButton1/2 instead of back/forward button enums
  // will use XButtons for backwards compatibility with Qt4
  // Qt::XButton1 == Qt::BackButton
  // Qt::XButton2 == Qt::ForwardButton

  if(e->button() == Qt::XButton1){
    this->back();
  }
  else if(e->button() == Qt::XButton2){
    this->forward();
  }
  else{
    this->page()->event(e);
  }

}
