#include "resulthistory.h"
#include "ui_resulthistory.h"
#include <QDebug>

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
ResultHistory::ResultHistory(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ResultHistory)
{
  ui->setupUi(this);
  loadHistory();
}

/*!
 * \brief Loads all result files and displays them in the table widget.
 */
void ResultHistory::loadHistory(){
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
  QDir historyDir( QDir::home().absolutePath() + QString("/ViennaCL-Benchmark/benchmarkHistory/") );
  if(historyDir.exists()){
    foreach( QFileInfo info, historyDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files)){
      if(info.isFile() && info.fileName().endsWith(".json")){
        //process json file
        QFile file(info.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly)){
          qDebug()<<"Failed to open file: "<<info.absoluteFilePath();
          continue;
        }
        QByteArray resultData = file.readAll();

        QJsonDocument loadedResultData( QJsonDocument::fromJson(resultData));
        readResultFile(loadedResultData.object());
        file.close();
      }
    }
  }
#endif

}

#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
/*!
 * \brief Reads a single JSON result file and displays its content in the table widget.
 * \param jsonRoot JSON file to be read
 */
void ResultHistory::readResultFile(const QJsonObject &jsonRoot){
  int currentRow = ui->tableWidget->rowCount();
  ui->tableWidget->insertRow(currentRow);
  QTableWidgetItem *Mode = new QTableWidgetItem(jsonRoot["mode"].toString());
  ui->tableWidget->setItem(currentRow, 0, Mode);
  QTableWidgetItem *Precision = new QTableWidgetItem(jsonRoot["precision"].toString());
  ui->tableWidget->setItem(currentRow, 1, Precision);

  QJsonObject benchmarkObject(jsonRoot["blas3"].toObject());
  QJsonValue val(benchmarkObject["result"]);

  QTableWidgetItem *Blas3 = new QTableWidgetItem( QString::number(val.toVariant().toDouble() ) );
  ui->tableWidget->setItem(currentRow, 2, Blas3);

  benchmarkObject = QJsonObject(jsonRoot["copy"].toObject());
  val = QJsonValue(benchmarkObject["result"]);
  QTableWidgetItem *Copy = new QTableWidgetItem( QString::number(val.toVariant().toDouble() ) );
  ui->tableWidget->setItem(currentRow, 3, Copy);

  benchmarkObject = QJsonObject(jsonRoot["sparse"].toObject());
  val = QJsonValue(benchmarkObject["result"]);
  QTableWidgetItem *Sparse = new QTableWidgetItem( QString::number(val.toVariant().toDouble() ) );
  ui->tableWidget->setItem(currentRow, 4, Sparse);

  benchmarkObject = QJsonObject(jsonRoot["vector"].toObject());
  val = QJsonValue(benchmarkObject["result"]);
  QTableWidgetItem *Vector = new QTableWidgetItem( QString::number(val.toVariant().toDouble() ) );
  ui->tableWidget->setItem(currentRow, 5, Vector);
}
#endif

/*!
 * \brief Destructor.
 */
ResultHistory::~ResultHistory()
{
  delete ui;
}
