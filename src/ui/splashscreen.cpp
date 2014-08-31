#include "splashscreen.h"

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
SplashScreen::SplashScreen(QWidget *parent) :
  QSplashScreen(parent)
{
  this->installEventFilter(this);
}

/*!
 * \brief Overridden event filter. Prevents the splash screen from disappearing when clicked on.
 * \param target Target object
 * \param event The event
 * \return Event success
 */
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
