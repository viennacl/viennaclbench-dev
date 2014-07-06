#ifndef COLLAPSEWIDGET_H
#define COLLAPSEWIDGET_H

#include <QWidget>

namespace Ui {
  class CollapseWidget;
}

class CollapseWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CollapseWidget(QWidget *parent = 0);
  ~CollapseWidget();

  void setText(QString textToShow);
  void setChildWidget(QWidget *child);
private:
  Ui::CollapseWidget *ui;

signals:
  void toggleChildVisibility();
public slots:
  void updateChildVisibility();
};

#endif // COLLAPSEWIDGET_H
