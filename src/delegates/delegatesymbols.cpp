#include "delegatesymbols.h"

DelegateSymbols::DelegateSymbols(QObject *parent, QRegExp newRegExpr) : QStyledItemDelegate(parent)
{
    m_rx = newRegExpr;
}

QWidget *DelegateSymbols::createEditor(QWidget* parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    if (index.data(Qt::UserRole).toInt() == 1)                  /* для блокировки поля */
        return NULL;

    QLineEdit* editor = new QLineEdit(parent);

    QRegExpValidator *val = new QRegExpValidator(m_rx, editor);
    editor->setValidator(val);
    return editor;
}

void DelegateSymbols::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::EditRole).toString();
    QLineEdit* line = static_cast<QLineEdit*>(editor);

    line->setText(value);
}

void DelegateSymbols::setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex &index) const
{
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    model->setData(index,value);
}

void DelegateSymbols::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
