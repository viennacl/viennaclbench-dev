#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include <QSplashScreen>
#include <QEvent>

/*! \class SplashScreen
 * \brief Customized splash screen widget. Does not disappear when clicked on.
 */
class SplashScreen : public QSplashScreen
{
  Q_OBJECT
public:
  explicit SplashScreen(QWidget *parent = 0);

  bool eventFilter(QObject *target, QEvent *event);
signals:

public slots:

};

#endif // SPLASHSCREEN_H
