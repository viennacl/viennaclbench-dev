#include "qtcategorybutton.h"

QtCategoryButton::QtCategoryButton( const QString& a_Text,
        QTreeWidget* a_pParent, QTreeWidgetItem* a_pItem )
    : QPushButton(a_Text, a_pParent)
    , m_pItem(a_pItem)
{
    connect(this, SIGNAL(pressed()), this, SLOT(ButtonPressed()));
}
 
void QtCategoryButton::ButtonPressed()
{
    m_pItem->setExpanded( !m_pItem->isExpanded() );
}
