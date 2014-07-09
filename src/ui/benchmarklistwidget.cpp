#include "benchmarklistwidget.h"
#include <QDebug>

BenchmarkListWidget::BenchmarkListWidget(QWidget *parent) :
  QListWidget(parent)
{
//  connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(activatedSignal(QModelIndex)) );
//  connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedSignal(QModelIndex)) );
//  connect(this, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(currentItemChangedSignal(QListWidgetItem*,QListWidgetItem*)) );
//  connect(this, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChangedSignal(int)) );
//  connect(this, SIGNAL(currentTextChanged(QString)), this, SLOT(currentTextChangedSignal(QString)) );
//  connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedSignal(QModelIndex)) );
//  connect(this, SIGNAL(entered(QModelIndex)), this, SLOT(enteredSignal(QModelIndex)) );
//  connect(this, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemActivatedSignal(QListWidgetItem*)) );
//  connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChangedSignal(QListWidgetItem*)) );
//  connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClickedSignal(QListWidgetItem*)) );
//  connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClickedSignal(QModelIndex)) );
//  connect(this, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(itemEnteredSignal(QListWidgetItem*)) );
//  connect(this, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(itemPressedSignal(QListWidgetItem*)) );
//  connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChangedSignal()) );
//  connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(pressedSignal(QModelIndex)) );
//  connect(this, SIGNAL(viewportEntered()), this, SLOT(viewportEnteredSignal()) );
//  connect(this, SIGNAL(), this, SLOT() );
//  connect(this, SIGNAL(), this, SLOT() );
  //  connect(this, SIGNAL(), this, SLOT() );
}

void BenchmarkListWidget::mouseMoveEvent(QMouseEvent *e){
  if(e->buttons() & Qt::LeftButton){
    e->ignore();
  }
  else{
    QAbstractItemView::mouseMoveEvent(e);
  }
}

void BenchmarkListWidget::selectAllItems()
{
  for ( int i = 0; i < this->count(); i++ ) {
    this->item(i)->setSelected(true);
    this->item(i)->setIcon(QIcon(":/icons/icons/checkTrue.png"));
  }
}

void BenchmarkListWidget::deselectAllItems()
{
  for ( int i = 0; i < this->count(); i++ ) {
    this->item(i)->setSelected(false);
    this->item(i)->setIcon(QIcon(":/icons/icons/checkFalse.png"));
  }
}

/*
 * Checks if all items are selected and updates the first item (ALL) to be selected aswell
 * */
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

//void BenchmarkListWidget::currentItemChangedSignal (QListWidgetItem *current, QListWidgetItem *previous)
//{
//  qDebug()<<"currentItemChanged";
//}

//void BenchmarkListWidget::currentRowChangedSignal (int currentRow)
//{
//  qDebug()<<"currentRowChanged";

//}

//void BenchmarkListWidget::currentTextChangedSignal (const QString &currentText)
//{
//  qDebug()<<"currentTextChanged";

//}

//void BenchmarkListWidget::itemActivatedSignal (QListWidgetItem *item)
//{
//  qDebug()<<"itemActivated";

//}

//void BenchmarkListWidget::itemChangedSignal (QListWidgetItem *item)
//{
//  qDebug()<<"itemChanged ";

//}

//void BenchmarkListWidget::itemClickedSignal (QListWidgetItem *item)
//{
//  qDebug()<<"itemClicked ";

//}

//void BenchmarkListWidget::itemDoubleClickedSignal (QListWidgetItem *item)
//{
//  qDebug()<<" itemDoubleClicked";

//}

//void BenchmarkListWidget::itemEnteredSignal (QListWidgetItem *item)
//{
//  qDebug()<<" itemEntered";

//}

//void BenchmarkListWidget::itemPressedSignal (QListWidgetItem *item)
//{
//  qDebug()<<" itemPressed";

//}

//void BenchmarkListWidget::itemSelectionChangedSignal ()
//{
//  qDebug()<<" itemSelectionChanged";

//}

//void BenchmarkListWidget::activatedSignal (const QModelIndex &index)
//{
//  qDebug()<<" activatedSignal";

//}

//void BenchmarkListWidget::clickedSignal (const QModelIndex &index)
//{
//  qDebug()<<" clickedSignal";

//}

//void BenchmarkListWidget::doubleClickedSignal(const QModelIndex &index)
//{
//  qDebug()<<" doubleClickedSignal";

//}

//void BenchmarkListWidget::enteredSignal(const QModelIndex &index)
//{
//  qDebug()<<" enteredSignal";

//}

//void BenchmarkListWidget::pressedSignal(const QModelIndex &index)
//{
//  qDebug()<<" pressedSignal";

//}

//void BenchmarkListWidget::viewportEnteredSignal()
//{
//  qDebug()<<" viewportEnteredSignal";

//}
