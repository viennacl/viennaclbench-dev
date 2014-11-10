#include "menulistwidget.h"

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
MenuListWidget::MenuListWidget(QWidget *parent) :
  QListWidget(parent)
{
}

/*!
 * \brief Calculates a new size for the entire widget so that all items can properly fit.
 * Some items might be larger than the widget, so they wouldn't be displayed properly.
 * \return Returns a size that will make sure each item can properly fit inside the widget.
 */
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

  //update the width so that the largest item can fit
  //if the largest item can fit, then all items can fit
  return QSize( maxSize.width() + rect().width() - contentsRect().width(), this->height());
}

/*!
 * \brief Calculates a normalized size that makes sure all items are of the same size.
 * This normalized size is a combination of the largest width and height of all items.
 * The normalized size is needed in order to properly show centered icons.
 * \return Returns a normalized size which can be used to make all items of the same size.
 */
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

/*!
 * \brief Overridden event handler. Prevents deselection of selected item(s) while attempting a drag-selection.
 * \param e Mouse event
 */
void MenuListWidget::mouseMoveEvent(QMouseEvent *e){
  if(e->buttons() & Qt::LeftButton){
    e->ignore();
  }
  else{
    QAbstractItemView::mouseMoveEvent(e);
  }
}
