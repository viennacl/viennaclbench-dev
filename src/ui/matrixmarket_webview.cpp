#include "matrixmarket_webview.h"
#include <QDebug>

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
MatrixMarket_WebView::MatrixMarket_WebView(QWidget *parent) :
  QWebView(parent)
{
  this->page()->setForwardUnsupportedContent(true);//enable non-html content processing
  this->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);//enable custom processing of external links
  connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClickedSlot(QUrl)) );//process external links
  connect(this->page(), SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(downloadSlot(QNetworkRequest)) );//process non-html download requests
  connect(this->page(), SIGNAL(unsupportedContent(QNetworkReply*)), this, SLOT(unsupportedContentSlot(QNetworkReply*)) );//detect non-html files (i.e. .tar.gz matrix files)
}

/*!
 * \brief Filters URL click events. Processes .tar.gz links, redirects all other links to system default browser.
 * Custom processing of external links must be enabled in order for this to work.
 * \param url Clicked URL
 */
void MatrixMarket_WebView::linkClickedSlot(QUrl url){
  qDebug()<<"link clicked";
  if(url.toString().endsWith(".tar.gz")){
    //accept .tar.gz links and open them with MatrixMarket browser
    this->load(url);
  }
  else{
    //redirect all other external links to system default browser
    QDesktopServices::openUrl(url);
  }
}

/*!
 * \brief Saves the downloaded matrix file into userHomeFolder/ViennaCL-Benchmark/MatrixMarket. Emits \ref fileReadyForBenchmark if successful.
 * \param reply Network reply object containing the downloaded matrix file
 */
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
  filename = filename.right(filename.length() - filename.lastIndexOf("/") - 1);// - 1 is to avoid getting double // characters (i.e. C:/folder//filename.txt)
  qDebug()<<"filename: "<<filename;

  QString downloadFolder = ArchiveExtractor::getMatrixMarketUserFolder();
  if(downloadFolder.isNull()){
    qDebug()<<"ERROR: Attempted to extract into non-existing user matrix market folder";
    return;
  }
  qDebug()<<"downloadFolder: "<<downloadFolder;

  QString fullPath = downloadFolder + filename;
  qDebug()<<"Fullpath: "<<fullPath;
  QFile mmFileToSave( fullPath );
  if(mmFileToSave.open(QIODevice::WriteOnly)){
    mmFileToSave.write(mmFile);
    mmFileToSave.close();
    qDebug()<<"File saved. Path: "<<fullPath;
    QString extractedFilePath = ArchiveExtractor::extractFileToUserHomeFolder(fullPath);
    qDebug()<<"extractedFilePath "<<extractedFilePath;
    if(extractedFilePath.isEmpty()){
      //extraction failed
    }
    else if(extractedFilePath.endsWith( QString(".mtx")) ){
      //extracted file is of .mtx format
      //proceed to do a benchmark with it
      emit fileReadyForBenchmark(extractedFilePath);
    }
  }
  else{
    qDebug()<<"Cannot save file";
    qDebug()<<mmFileToSave.errorString();
    mmFileToSave.close();
  }
  reply->deleteLater();
}

/*!
 * \brief Handles file download requests. Assigns the network request to the downloaded manager that handles downloading.
 * \param request Network request that requested the file
 */
void MatrixMarket_WebView::downloadSlot(QNetworkRequest request)
{
  qDebug()<<"Download requested: "<<request.url();
  downloadManager = new QNetworkAccessManager();
  currentDownload = downloadManager->get(request);

  connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(currentDownloadProgressSlot(qint64,qint64)) );
  connect(currentDownload, SIGNAL(finished()), currentDownload, SLOT(deleteLater()) );
  connect(downloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processDownloadedFile(QNetworkReply*)) );
  connect(downloadManager, SIGNAL(finished(QNetworkReply*)), downloadManager, SLOT(deleteLater()) );
}

/*!
 * \brief Used to emit download progress status without accessing the download manager directly.
 * \param bytesReceived Bytes downloaded so far
 * \param bytesTotal Total bytes to download
 */
void MatrixMarket_WebView::currentDownloadProgressSlot(qint64 bytesReceived, qint64 bytesTotal){
  emit currentDownloadProgress(bytesReceived, bytesTotal);
}

/*!
 * \brief Filters URL requests that contain unsupported content (.tar.gz files and such).
 * Forwards .tar.gz requests to \ref downloadSlot , ignores all other requests.
 * \param reply Network reply containing the unsupported file
 */
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

/*!
 * \brief Overridden event filter. Binds back/forward mouse buttons to back/forward web navigation buttons.
 * \param e Mouse event
 */
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
