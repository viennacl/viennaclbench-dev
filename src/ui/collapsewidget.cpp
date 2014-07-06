#include "collapsewidget.h"
#include "ui_collapsewidget.h"
#include <QDebug>

CollapseWidget::CollapseWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CollapseWidget)
{
  ui->setupUi(this);
  connect(ui->collapseButton, SIGNAL(clicked()), this, SLOT(updateChildVisibility()) );

}

CollapseWidget::~CollapseWidget()
{
  delete ui;
}

void CollapseWidget::setText(QString textToShow)
{
  ui->collapseButton->setText(textToShow);
}

void CollapseWidget::setChildWidget(QWidget *child)
{
  ui->verticalLayout->removeWidget(ui->childWidget);
  delete ui->childWidget;
  ui->childWidget = child;
  ui->verticalLayout->addWidget(ui->childWidget);
}

void CollapseWidget::updateChildVisibility()
{
  qDebug()<<"updating child visibility";
  if(ui->childWidget->isVisible()){
    ui->childWidget->hide();
  }
  else{
    ui->childWidget->show();
  }

}
