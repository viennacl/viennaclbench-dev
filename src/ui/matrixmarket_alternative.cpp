#include "matrixmarket_alternative.h"
#include "ui_matrixmarket_alternative.h"

#include <iostream>
#include <QTimer>

MatrixMarket_Alternative::MatrixMarket_Alternative(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MatrixMarket_Alternative)
{
  ui->setupUi(this);

  loadJsonMatrixData();
  matrixModel = new QStandardItemModel(0, 13, this);

  matrixModel->setHeaderData(0, Qt::Horizontal, QString("Thumbnail"));
  matrixModel->setHeaderData(1, Qt::Horizontal, QString("Graph"));
  matrixModel->setHeaderData(2, Qt::Horizontal, QString("Id"));
  matrixModel->setHeaderData(3, Qt::Horizontal, QString("Name"));
  matrixModel->setHeaderData(4, Qt::Horizontal, QString("Group"));
  matrixModel->setHeaderData(5, Qt::Horizontal, QString("Rows"));
  matrixModel->setHeaderData(6, Qt::Horizontal, QString("Columns"));
  matrixModel->setHeaderData(7, Qt::Horizontal, QString("Nonzeroes"));
  matrixModel->setHeaderData(8, Qt::Horizontal, QString("Type"));
  matrixModel->setHeaderData(9, Qt::Horizontal, QString("Sym"));
  matrixModel->setHeaderData(10, Qt::Horizontal, QString("Spd?"));
  matrixModel->setHeaderData(11, Qt::Horizontal, QString("Download & Run"));
  matrixModel->setHeaderData(12, Qt::Horizontal, QString("Results"));

  ui->tableView->setModel(matrixModel);

  matrixFileNetManager = new QNetworkAccessManager(this);

  //using QTableView
  createMatrixTableView();

  //using QTableWidget
//    createMatrixTable();
  //hide it cuz we dont need it
  ui->matrixTable->hide();

}

void MatrixMarket_Alternative::createMatrixTableView(){
  int arraySize = jsonArray.size();

  QTableView *table = ui->tableView;

  /*
  Qt < 5
  verticalHeader->setResizeMode(QHeaderView::Fixed);
  Qt >= 5
  verticalHeader->sectionResizeMode(QHeaderView::Fixed);
  */
  table->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
  table->verticalHeader()->setDefaultSectionSize(100);

  table->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

  thumbNetManager2 = new QNetworkAccessManager(this);
  connect(thumbNetManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(thumbnailDownloadFinished2(QNetworkReply*)) );

  graphNetManager2 = new QNetworkAccessManager(this);
  connect(graphNetManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(graphDownloadFinished2(QNetworkReply*)) );

  QNetworkRequest req;
  QJsonObject matrixInfoObject;
  QUrl thumbUrl;
  QUrl graphUrl;
  QStandardItem *thumbItem;
  QStandardItem *graphItem;
  QString  numberString;
  QPushButton *downloadButton;

  for(int rowIndex = 0; rowIndex < arraySize; rowIndex++){
    matrixInfoObject = jsonArray.at(rowIndex).toObject();
    /*
    columnsNumber: "1,138"
    downloadSrc: "http://www.cise.ufl.edu/research/sparse/MM/HB/1138_bus.tar.gz"
    graphSrc: "http://yifanhu.net/GALLERY/GRAPHS/GIF_THUMBNAIL/HB@1138_bus.gif"
    group: "HB"
    id: "1"
    name: "1138_bus"
    nonzeros: "4,054"
    rowsNumber: "1,138"
    spd: "yes"
    sym: "yes"
    thumbSrc: "http://www.cise.ufl.edu/research/sparse/matrices/HB/1138_bus_thumb.png"
    type: "real symmetric"
    */

    matrixModel->insertRow(0);

    //---column 0: thumbnail icon ("thumbSrc")---
    thumbUrl = matrixInfoObject["thumbSrc"].toString();
    req.setUrl(thumbUrl);

    QNetworkReply *thumbReply = thumbNetManager2->get(req);//get(req) returns the same QNetworkReply* as the one from finished(QNetworkReply*) signal of netManager

    thumbItem = new QStandardItem("test");
    matrixModel->setItem(0, 0, thumbItem);
    thumbMap2.insert(thumbReply, thumbItem);
    //-------

    //---column 1: graph image ("graphSrc")---
    graphUrl = matrixInfoObject["graphSrc"].toString();
    req.setUrl(graphUrl);

    QNetworkReply *graphReply = graphNetManager2->get(req);

    graphItem = new QStandardItem("test");
    matrixModel->setItem(0, 1, graphItem);
    graphMap2.insert(graphReply, graphItem);
    //-------

    //---column 2: id---
    matrixModel->setData(matrixModel->index(0, 2), matrixInfoObject["id"].toString(), Qt::DisplayRole );
    //-------

    //---column 3: name---
    matrixModel->setData(matrixModel->index(0, 3), matrixInfoObject["name"].toString(), Qt::DisplayRole);
    //-------

    //---column 4: group---
    matrixModel->setData(matrixModel->index(0, 4), matrixInfoObject["group"].toString(), Qt::DisplayRole);
    //-------

    //---column 5: rowsNumber---
    numberString = matrixInfoObject["rowsNumber"].toString().replace(',',"");
    matrixModel->setData(matrixModel->index(0, 5), numberString.toInt(), Qt::DisplayRole);
    //-------

    //---column 6: columnsNumber---
    matrixModel->setData(matrixModel->index(0, 6), matrixInfoObject["columnsNumber"].toString(), Qt::DisplayRole);
    //-------

    //---column 7: nonzeros---
    matrixModel->setData(matrixModel->index(0, 7), matrixInfoObject["nonzeros"].toString(), Qt::DisplayRole);
    //-------

    //---column 8: type---
    matrixModel->setData(matrixModel->index(0, 8), matrixInfoObject["type"].toString(), Qt::DisplayRole);
    //-------

    //---column 9: sym---
    matrixModel->setData(matrixModel->index(0, 9), matrixInfoObject["sym"].toString(), Qt::DisplayRole);
    //-------

    //---column 10: spd---
    matrixModel->setData(matrixModel->index(0, 10), matrixInfoObject["spd"].toString(), Qt::DisplayRole);
    //-------

    //---column 11: download("downloadSrc") & run---
//    matrixModel->setData(matrixModel->index(0, 11), matrixInfoObject["downloadSrc"].toString(), Qt::DisplayRole);

    downloadButton = new QPushButton("Download and Run");
    downloadUrlMap.insert(downloadButton, matrixInfoObject["downloadSrc"].toString());
    table->setIndexWidget(matrixModel->index(0, 11), downloadButton);
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(getMatrixUrl()) );
    //-------

    //---column 12: results---
    matrixModel->setData(matrixModel->index(0, 12), QString("Results could be displayed in this widget"), Qt::DisplayRole);
    //-------

  }

}

void MatrixMarket_Alternative::getMatrixUrl(){
  QPushButton* button = (QPushButton*)sender();
  if(button){
    QString matrixUrl = downloadUrlMap.value(button);
    qDebug()<<matrixUrl;

    QUrl url(matrixUrl);
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply *downloadReply = matrixFileNetManager->get(request);

    //progress update
    connect(downloadReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(currentDownloadProgressSlot(qint64,qint64)) );
    //delete download reply object
    connect(downloadReply, SIGNAL(finished()), downloadReply, SLOT(deleteLater()) );

    //process downloaded matrix file
    connect(matrixFileNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processDownloadedFile(QNetworkReply*)) );
//    connect(downloadManager, SIGNAL(finished(QNetworkReply*)), downloadManager, SLOT(deleteLater()) );

  }
}

void MatrixMarket_Alternative::currentDownloadProgressSlot(qint64 bytesReceived, qint64 bytesTotal){
  qDebug()<<"download progress: " << bytesReceived << " of " << bytesTotal;
}

void MatrixMarket_Alternative::processDownloadedFile(QNetworkReply* reply){
  QByteArray matrixFileData;
  matrixFileData = reply->readAll();
//  if(matrixFileData.isEmpty()){
//    qDebug()<<"Downloaded file is empty";
//    reply->deleteLater();
//    return;
//  }
//  else{
//    qDebug()<<"Download successful, bytes downloaded:"<<matrixFileData.size();
//  }
  QString filename =  reply->url().toString();
  filename = filename.right(filename.length() - filename.lastIndexOf("/") - 1);// - 1 is to avoid getting double // characters (i.e. C:/folder//filename.txt)
  qDebug()<<"filename: "<<filename;

  QString downloadFolder = ArchiveExtractor::getMatrixMarketUserFolder();
  if(downloadFolder.isNull()){
    qDebug()<<"ERROR: Attempted to extract into non-existing user matrix market folder";
    reply->deleteLater();
    return;
  }
  qDebug()<<"downloadFolder: "<<downloadFolder;

  QString fullPath = downloadFolder + filename;
  qDebug()<<"Fullpath: "<<fullPath;
  QFile matrixFileToSave( fullPath );
  if(matrixFileToSave.open(QIODevice::WriteOnly)){
    matrixFileToSave.write(matrixFileData);
    matrixFileToSave.close();
    qDebug()<<"File saved. Path: "<<fullPath;
    QString extractedFilePath = ArchiveExtractor::extractFileToUserHomeFolder(fullPath);
    qDebug()<<"extractedFilePath "<<extractedFilePath;
    if(extractedFilePath.isEmpty()){
      //extraction failed
    }
    else if(extractedFilePath.endsWith( QString(".mtx")) ){
      //make sure the file is of .mtx format
      //proceed to do a benchmark with it
      emit fileReadyForBenchmark(extractedFilePath);
    }
  }
  else{
    qDebug()<<"Cannot save file";
    qDebug()<<matrixFileToSave.errorString();
    matrixFileToSave.close();
  }
  reply->deleteLater();
}

MatrixMarket_Alternative::~MatrixMarket_Alternative()
{
  delete ui;
}

void MatrixMarket_Alternative::loadJsonMatrixData(){
  //  :/icons/resources/mDataList.json.js
  //  QFile file("C:/Qt workspace/MatrixMarket_Alternative/mDataList.json.js");
  QFile file("C:/Qt workspace/MatrixMarket_Alternative/test.txt");
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
    qDebug()<<"Failed to open file: ";
  }
  QByteArray resultData = file.readAll();
  qDebug()<<"Read raw data:";
  //  qDebug()<<resultData;

  QJsonParseError jsonError;
  QJsonDocument loadedJsonDoc( QJsonDocument::fromJson(resultData, &jsonError));
  if(loadedJsonDoc.isEmpty()){
    qDebug()<<"json is empty";
  }
  if(loadedJsonDoc.isArray()){
    qDebug()<<"json is array";
    jsonArray = loadedJsonDoc.array();
    //    readResultFile(loadedJsonDoc.array());
  }
  if(loadedJsonDoc.isObject()){
    qDebug()<<"json is object";
  }
  qDebug()<<jsonError.errorString();
  file.close();
}

void MatrixMarket_Alternative::graphDownloadFinished(QNetworkReply *reply)
{
  //  qDebug()<< "reply id in slot: " << reply <<  endl;
  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
    return;
  }

  QByteArray jpegData = reply->readAll();
  QPixmap *pixmap = new QPixmap;
  pixmap->loadFromData(jpegData);

  //    QPixmap resPix = pix.scaled(75,75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  int rowIndex = graphMap.value(reply);
  QLabel *label = dynamic_cast<QLabel*>(ui->matrixTable->cellWidget(rowIndex, 1));//get the graph label pointer
  label->setPixmap(*pixmap);
  reply->close();
  reply->deleteLater();
}

void MatrixMarket_Alternative::graphDownloadFinished2(QNetworkReply *reply)
{
  //  qDebug()<< "reply id in slot: " << reply <<  endl;
  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
    return;
  }

  QByteArray jpegData = reply->readAll();

//  QPixmap *pixmap = new QPixmap;
//  pixmap->loadFromData(jpegData);
  QImage image;
  image.loadFromData(jpegData);

  //    QPixmap resPix = pix.scaled(75,75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  QStandardItem *graphItem = graphMap2.value(reply);
  graphItem->setData(QVariant(QPixmap::fromImage(image)), Qt::DecorationRole);
  reply->close();
  reply->deleteLater();
}

void MatrixMarket_Alternative::thumbnailDownloadFinished(QNetworkReply *reply){
  //  qDebug()<< "reply id in slot: " << reply <<  endl;
  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
    return;
  }

  QByteArray jpegData = reply->readAll();
  QPixmap *pixmap = new QPixmap;
  pixmap->loadFromData(jpegData);

  int rowIndex = thumbMap.value(reply);
  QLabel *label = dynamic_cast<QLabel*>(ui->matrixTable->cellWidget(rowIndex, 0));//get the thumbnail label pointer
  label->setPixmap(*pixmap);
  reply->close();
  reply->deleteLater();
}

void MatrixMarket_Alternative::thumbnailDownloadFinished2(QNetworkReply *reply){
  //  qDebug()<< "reply id in slot: " << reply <<  endl;
  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
    return;
  }

  QByteArray jpegData = reply->readAll();
//  QPixmap *pixmap = new QPixmap;
//  pixmap->loadFromData(jpegData);
  QImage image;
  image.loadFromData(jpegData);

  //    QPixmap resPix = pix.scaled(75,75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  QStandardItem *thumbItem = thumbMap2.value(reply);
  thumbItem->setData(QVariant(QPixmap::fromImage(image)), Qt::DecorationRole);
  reply->close();
  reply->deleteLater();
}

/* //TODO abort all image downloads (iterate through thumbMap & graphMap and call abort on each QNetworkReply* */

void MatrixMarket_Alternative::createMatrixTable(){
  int arraySize = jsonArray.size();

  QTableWidget *table = ui->matrixTable;

  table->setRowCount(arraySize);

  /*
  Qt < 5
  verticalHeader->setResizeMode(QHeaderView::Fixed);
  Qt >= 5
  verticalHeader->sectionResizeMode(QHeaderView::Fixed);
  */
  table->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
  table->verticalHeader()->setDefaultSectionSize(75);

  thumbNetManager;

  thumbNetManager = new QNetworkAccessManager(this);
  connect(thumbNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(thumbnailDownloadFinished(QNetworkReply*)) );

  graphNetManager = new QNetworkAccessManager(this);
  connect(graphNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(graphDownloadFinished(QNetworkReply*)) );

  QNetworkRequest req;
  QJsonObject matrixInfoObject;
  QUrl thumbUrl;
  QUrl graphUrl;

  for(int rowIndex = 0; rowIndex < arraySize; rowIndex++){
    matrixInfoObject = jsonArray.at(rowIndex).toObject();
    /*
    columnsNumber: "1,138"
    downloadSrc: "http://www.cise.ufl.edu/research/sparse/MM/HB/1138_bus.tar.gz"
    graphSrc: "http://yifanhu.net/GALLERY/GRAPHS/GIF_THUMBNAIL/HB@1138_bus.gif"
    group: "HB"
    id: "1"
    name: "1138_bus"
    nonzeros: "4,054"
    rowsNumber: "1,138"
    spd: "yes"
    sym: "yes"
    thumbSrc: "http://www.cise.ufl.edu/research/sparse/matrices/HB/1138_bus_thumb.png"
    type: "real symmetric"
    */

    //---column 0: thumbnail icon ("thumbSrc")---
    thumbUrl = matrixInfoObject["thumbSrc"].toString();
    req.setUrl(thumbUrl);

    QNetworkReply *thumbReply = thumbNetManager->get(req);//get(req) returns the same QNetworkReply* as the one from finished(QNetworkReply*) signal of netManager
    thumbMap.insert(thumbReply, rowIndex);

    QLabel *thumbLabel = new QLabel;
    thumbLabel->setAlignment(Qt::AlignCenter);

    table->setCellWidget(rowIndex, 0, thumbLabel);
    //-------

    //---column 1: graph image ("graphSrc")---
    graphUrl = matrixInfoObject["graphSrc"].toString();
    req.setUrl(graphUrl);

    QNetworkReply *graphReply = graphNetManager->get(req);
    graphMap.insert(graphReply, rowIndex);

    QLabel *graphLabel = new QLabel;
    graphLabel->setAlignment(Qt::AlignCenter);
    table->setCellWidget(rowIndex, 1, graphLabel);
    //-------

    //---column 2: id---
    QTableWidgetItem *idItem = new QTableWidgetItem(matrixInfoObject["id"].toString() );

    table->setItem(rowIndex, 2, idItem);
    //-------

    //---column 3: name---
    QTableWidgetItem *nameItem = new QTableWidgetItem(matrixInfoObject["name"].toString());

    table->setItem(rowIndex, 3, nameItem);
    //-------

    //---column 4: group---
    QTableWidgetItem *groupItem = new QTableWidgetItem(matrixInfoObject["group"].toString());

    table->setItem(rowIndex, 4, groupItem);
    //-------

    //---column 5: rowsNumber---
    QTableWidgetItem *rowNumberItem = new QTableWidgetItem(matrixInfoObject["rowsNumber"].toString());

    table->setItem(rowIndex, 5, rowNumberItem);
    //-------

    //---column 6: columnsNumber---
    QTableWidgetItem *columnNumberItem = new QTableWidgetItem(matrixInfoObject["columnsNumber"].toString());

    table->setItem(rowIndex, 6, columnNumberItem);
    //-------

    //---column 7: nonzeros---
    QTableWidgetItem *nonzerosItem = new QTableWidgetItem(matrixInfoObject["nonzeros"].toString());

    table->setItem(rowIndex, 7, nonzerosItem);
    //-------

    //---column 8: type---
    QTableWidgetItem *typeItem = new QTableWidgetItem(matrixInfoObject["type"].toString());

    table->setItem(rowIndex, 8, typeItem);
    //-------

    //---column 9: sym---
    QTableWidgetItem *symItem = new QTableWidgetItem(matrixInfoObject["sym"].toString());

    table->setItem(rowIndex, 9, symItem);
    //-------

    //---column 10: spd---
    QTableWidgetItem *spdItem = new QTableWidgetItem(matrixInfoObject["spd"].toString());

    table->setItem(rowIndex, 10, spdItem);
    //-------

    //---column 11: download("downloadSrc") & run---
//    QTableWidgetItem *downloadRunItem = new QTableWidgetItem(matrixInfoObject["downloadSrc"].toString());
//    table->setItem(rowIndex, 11, downloadRunItem);

    QPushButton *downloadButton = new QPushButton(matrixInfoObject["downloadSrc"].toString());
    table->setCellWidget(rowIndex, 11, downloadButton);
    //-------

    //---column 12: results---
    QTableWidgetItem *resultsItem = new QTableWidgetItem("Results could be displayed in this widget");

    table->setItem(rowIndex, 12, resultsItem);
    //-------
  }


}
