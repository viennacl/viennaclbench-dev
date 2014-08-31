#ifndef MENULISTWIDGET_H
#define MENULISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>

/*! \class MenuListWidget
 * \brief Main menu list widget. Customized to display menu items with icons in a proper centered way, among other things.
 */
class MenuListWidget : public QListWidget
{
  Q_OBJECT

public:
  explicit MenuListWidget(QWidget *parent = 0);
  virtual QSize sizeHint() const;
  QSize itemSizeHint() const;

signals:

public slots:
  void mouseMoveEvent(QMouseEvent *e);
};

#endif // MENULISTWIDGET_H
