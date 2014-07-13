#include "matrixmarket_webview.h"
#include <QDebug>

MatrixMarket_WebView::MatrixMarket_WebView(QWidget *parent) :
  QWebView(parent)
{
  this->page()->setForwardUnsupportedContent(true);
  connect(this->page(), SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(downloadSlot(QNetworkRequest)) );
  connect(this->page(), SIGNAL(unsupportedContent(QNetworkReply*)), this, SLOT(unsupportedContentSlot(QNetworkReply*)) );
}

void MatrixMarket_WebView::downloadSlot(QNetworkRequest request)
{
  qDebug()<<"Download requested: "<<request.url();

}

void MatrixMarket_WebView::unsupportedContentSlot(QNetworkReply *reply)
{
  QString contentUrl = reply->url().toString();
  qDebug()<<"Unsupported content requested: "<<contentUrl;
  if(contentUrl.contains("/MM/", Qt::CaseSensitive) && contentUrl.endsWith(".tar.gz")){
    qDebug()<<"Content type: .tar.gz";
  }
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
