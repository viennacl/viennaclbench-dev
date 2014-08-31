#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace Ui {
  class HomeScreen;
}

/*! \class HomeScreen
 * \brief Homescreen widget is created with this class.
 */
class HomeScreen : public QWidget
{
  Q_OBJECT

public:
  explicit HomeScreen(QWidget *parent = 0);
  ~HomeScreen();
  QPushButton* quickStartButton; ///< Exposes the quickstart button for usage in other widgets

private:
  Ui::HomeScreen *ui;
};

#endif // HOMESCREEN_H
