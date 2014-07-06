#include <QPushButton>
#include <QTreeWidget>

class QtCategoryButton : public QPushButton
{
    Q_OBJECT
public:
    QtCategoryButton(const QString& a_Text, QTreeWidget* a_pParent,
        QTreeWidgetItem* a_pItem);
 
private slots:
    void ButtonPressed();
 
private:
    QTreeWidgetItem* m_pItem;
};
