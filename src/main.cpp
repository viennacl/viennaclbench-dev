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
