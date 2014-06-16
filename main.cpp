#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);


//  qDebug()<<"toVariant";
//  qDebug()<<jsonDoc.toVariant().toString();

  // set the value of the environment variable QML_IMPORT_TRACE as 1
//  QByteArray data = "1";
//  qputenv("QML_IMPORT_TRACE", data);


  MainWindow w;
  //  w.writeJson(&jsonObject);
//  w.readJson(jsonObject);
  w.show();

  return a.exec();
}
