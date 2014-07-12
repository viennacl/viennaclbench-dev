#include "ui/mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QSplashScreen *splashScreen = new QSplashScreen();
  splashScreen->setPixmap(QPixmap(":/resources/images/splash-screen.jpg"));
  splashScreen->show();
  splashScreen->showMessage("Loading GUI");
  qApp->processEvents();
  MainWindow w;
  QTimer::singleShot(2000, &w, SLOT(show()) );
  QTimer::singleShot(2000, splashScreen, SLOT(deleteLater()) );
  return a.exec();
}
