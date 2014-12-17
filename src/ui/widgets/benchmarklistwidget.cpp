/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#include "benchmarklistwidget.h"
#include <QDebug>

/*!
 * \brief Default constructor.
 * \param parent Optional parent object.
 */
BenchmarkListWidget::BenchmarkListWidget(QWidget *parent) :
  QListWidget(parent)
{
}

/*!
 * \brief Overridden event handler. Prevents deselection of selected item(s) while attempting a drag-selection.
 * \param e Mouse event
 */
void BenchmarkListWidget::mouseMoveEvent(QMouseEvent *e){
  if(e->buttons() & Qt::LeftButton){
    e->ignore();
  }
  else{
    QAbstractItemView::mouseMoveEvent(e);
  }
}

/*!
 * \brief Selects all items.
 */
void BenchmarkListWidget::selectAllItems()
{
  for ( int i = 0; i < this->count(); i++ ) {
    this->item(i)->setSelected(true);
    this->item(i)->setIcon(QIcon(":/icons/icons/checkTrue.png"));
  }
}

/*!
 * \brief Deselects all items.
 */
void BenchmarkListWidget::deselectAllItems()
{
  for ( int i = 0; i < this->count(); i++ ) {
    this->item(i)->setSelected(false);
    this->item(i)->setIcon(QIcon(":/icons/icons/checkFalse.png"));
  }
}

/*!
 * \brief Selects the specified item (updates the icon aswell)
 * \param itemRow Item row
 */
void BenchmarkListWidget::setSelected(int itemRow){
  this->item(itemRow)->setSelected(true);
  this->item(itemRow)->setIcon(QIcon(":/icons/icons/checkTrue.png"));
}

/*!
 * \brief Deselects the specified item (updates the icon aswell)
 * \param itemRow Item row
 */
void BenchmarkListWidget::setDeselected(int itemRow){
  this->item(itemRow)->setSelected(false);
  this->item(itemRow)->setIcon(QIcon(":/icons/icons/checkFalse.png"));
}

/*!
 * \brief Checks if all items are selected and updates the first item ("select all" item) to be selected aswell.
 */
void BenchmarkListWidget::checkSelectedItems()
{
  bool allItemsSelected = true;
  for ( int i = 1; i < this->count(); i++ ) {
    if(!this->item(i)->isSelected()){
      allItemsSelected = false;
    }
  }
  if(allItemsSelected){
    this->item(0)->setSelected(true);
    this->item(0)->setIcon(QIcon(":/icons/icons/checkTrue.png"));
  }
}
