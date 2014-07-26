#include "ui/mainwindow.h"
#include "ui/splashscreen.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include "archiveextractor.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
//  QStandardPaths::writableLocation(QStandardPaths::DataLocation); //Introduced in Qt5...
//  ArchiveExtractor::extractFileToWorkFolder(QString("MatrixMarket/gearbox.tar.gz"));
//  ArchiveExtractor::extractFileToUserHomeFolder(QString("MatrixMarket/gearbox.tar.gz"));
  ArchiveExtractor::extractFileToTargetFolder("C:/Users/Namik/ViennaCL Benchmark/MatrixMarket/685_bus.tar.gz", "C:/kurac/");
  qDebug()<<"1";
  ArchiveExtractor::extractFileToWorkFolder("C:/Users/Namik/ViennaCL Benchmark/MatrixMarket/685_bus.tar.gz");
    qDebug()<<"2";
  ArchiveExtractor::extractFileToUserHomeFolder("C:/Users/Namik/ViennaCL Benchmark/MatrixMarket/685_bus.tar.gz");
    qDebug()<<"3";
  SplashScreen *splashScreen = new SplashScreen();
  splashScreen->setPixmap(QPixmap(":/resources/images/splash-screen.jpg"));
  splashScreen->show();
  splashScreen->showMessage("Loading GUI");
  qApp->processEvents();
  MainWindow w;
  QTimer::singleShot(2000, &w, SLOT(show()) );
  QTimer::singleShot(2000, splashScreen, SLOT(deleteLater()) );
  return app.exec();
}
