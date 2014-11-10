#include "collapsewidget.h"
#include "ui_collapsewidget.h"
#include <QDebug>

/*!
 * \brief Default constructor.
 * \param parent Optional parent.
 */
CollapseWidget::CollapseWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CollapseWidget)
{
  ui->setupUi(this);
  connect(ui->collapseButton, SIGNAL(clicked()), this, SLOT(updateChildVisibility()) );
}

/*!
 * \brief Destructor.
 */
CollapseWidget::~CollapseWidget()
{
  delete ui;
}

/*!
 * \brief Sets a title or text to be shown on top of this widget (next to the collapse button).
 * \param textToShow Text to show
 */
void CollapseWidget::setText(QString textToShow)
{
  ui->collapseButton->setText(textToShow);
}

/*!
 * \brief Sets the collapsible child widget.
 * \param child The widget to be made collapsible
 */
void CollapseWidget::setChildWidget(QWidget *child)
{
  ui->verticalLayout->removeWidget(ui->childWidget);
  delete ui->childWidget;
  ui->childWidget = child;
  ui->verticalLayout->addWidget(ui->childWidget);
}

/*!
 * \brief Toggles collapsed/restored status of the child widget
 */
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
