#include "ui/mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QSplashScreen *splashScreen = new QSplashScreen();
  splashScreen->setPixmap(QPixmap(":/resources/images/splash-screen.jpg"));
  splashScreen->show();
  splashScreen->showMessage("Loading GUI");
  qApp->processEvents();
  MainWindow w;
  w.show();
  splashScreen->finish(&w);

  return a.exec();
}
