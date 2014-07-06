#include "benchmarklistwidget.h"

BenchmarkListWidget::BenchmarkListWidget(QWidget *parent) :
  QListWidget(parent)
{
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
  }
}

void BenchmarkListWidget::deselectAllItems()
{
  for ( int i = 0; i < this->count(); i++ ) {
    this->item(i)->setSelected(false);
  }
}
