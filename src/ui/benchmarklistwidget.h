#ifndef BENCHMARKLISTWIDGET_H
#define BENCHMARKLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>

class BenchmarkListWidget : public QListWidget
{
  Q_OBJECT

public:
  explicit BenchmarkListWidget(QWidget *parent = 0);
signals:

public slots:
  void mouseMoveEvent(QMouseEvent *e);
  void selectAllItems();
  void deselectAllItems();
  void checkSelectedItems();

//  void	currentItemChangedSignal ( QListWidgetItem * current, QListWidgetItem * previous );
//  void	currentRowChangedSignal ( int currentRow );
//  void	currentTextChangedSignal ( const QString & currentText );
//  void	itemActivatedSignal ( QListWidgetItem * item );
//  void	itemChangedSignal ( QListWidgetItem * item );
//  void	itemClickedSignal ( QListWidgetItem * item );
//  void	itemDoubleClickedSignal ( QListWidgetItem * item );
//  void	itemEnteredSignal ( QListWidgetItem * item );
//  void	itemPressedSignal ( QListWidgetItem * item );
//  void	itemSelectionChangedSignal ();

////  AbstractView
//  void	activatedSignal ( const QModelIndex & index );
//  void	clickedSignal ( const QModelIndex & index );
//  void	doubleClickedSignal ( const QModelIndex & index );
//  void	enteredSignal ( const QModelIndex & index );
//  void	pressedSignal ( const QModelIndex & index );
//  void	viewportEnteredSignal ();
};

#endif // BENCHMARKLISTWIDGET_H
