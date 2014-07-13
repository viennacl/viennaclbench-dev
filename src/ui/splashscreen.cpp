#include "splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
  QSplashScreen(parent)
{
  this->installEventFilter(this);
}

bool SplashScreen::eventFilter(QObject *target, QEvent *event){
  if(event->type() == QEvent::MouseButtonRelease){
    event->accept();
    return true;
  }
  else if(event->type() == QEvent::MouseButtonPress){
    event->accept();
    return true;
  }
  else if(event->type() == QEvent::MouseButtonDblClick){
    event->accept();
    return true;
  }
  else{
    return QSplashScreen::eventFilter(target,event);
  }
}
