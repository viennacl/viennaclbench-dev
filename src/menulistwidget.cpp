#include "menulistwidget.h"
#include <QMouseEvent>

/*
 * Sublass of QListWidget
 * Needed to display item icons centered: re-implemented sizeHint() and added itemSizeHint()
 * Needed to remove selection box functionality because it was causing items to be de-selected upon click+drag event
 * */

MenuListWidget::MenuListWidget(QWidget *parent) :
  QListWidget(parent)
{
}

/*
 * Calculates a new size for the MenuListWidget for which all items could properly fit
 * The issue was that the widget's width wasn't the same as the width of its items
 * */
QSize MenuListWidget::sizeHint() const
{
  QSize maxSize = QSize(0,0);//keep track of largest item size
  //go through each item and determine the largest size in which all items can fit
  for ( int i = 0; i < this->count(); i++ ) {
    const QModelIndex index = model()->index( i, 0 );
    const QSize tmpSize = this->sizeHintForIndex(index);

    if (tmpSize.width() > maxSize.width())
      maxSize.setWidth(tmpSize.width());
    if (tmpSize.height() > maxSize.height())
      maxSize.setHeight(tmpSize.height());
  }

  //update the widget's width so that all items can fit
  return QSize( maxSize.width() + rect().width() - contentsRect().width(), this->height());
}

/*
 * Normalizes each items width so that all items have the same width
 * Item's icon and text now appear centralized relative to the parent MenuListWidget
 * */
QSize MenuListWidget::itemSizeHint() const
{
  QSize maxSize = QSize(0,0);//keep track of largest item size
  //go through each item and determine the largest size in which all items can fit
  for ( int i = 0; i < this->count(); i++ ) {
    const QModelIndex index = model()->index( i, 0 );
    const QSize tmpSize = this->sizeHintForIndex(index);

    if (tmpSize.width() > maxSize.width())
      maxSize.setWidth(tmpSize.width());
    if (tmpSize.height() > maxSize.height())
      maxSize.setHeight(tmpSize.height());
  }

  //return the normalized item size
  return QSize( maxSize.width(), maxSize.height());
}

/*
 * overriding QListWidget's mouseMoveEvent slot
 * moving the mouse while holding left mouse button was causing
 * currently selected items to deselect because it attempted to
 * drag-select multiple items(even though that functionality was disabled)
*/
void MenuListWidget::mouseMoveEvent(QMouseEvent *e){
  if(e->buttons() & Qt::LeftButton){
    e->ignore();
  }
  else{
    QAbstractItemView::mouseMoveEvent(e);
  }
}
