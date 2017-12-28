#include "delegateid.h"
#include <QDebug>

DelegateId::DelegateId(QObject* parent, QRegExp newRegExpr) : QStyledItemDelegate(parent)
{
    m_rx = newRegExpr;
}

QWidget *DelegateId::createEditor(QWidget* parent, const QStyleOptionViewItem &, const QModelIndex& /*index*/) const
{
    QLineEdit* editor = new QLineEdit(parent);
    
    QRegExpValidator *val = new QRegExpValidator(m_rx, editor);
    editor->setValidator(val);
    
    return editor;
}

void DelegateId::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::EditRole).toString();

    QLineEdit* line = static_cast<QLineEdit*>(editor);
    line->setText(value);
}

void DelegateId::setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex &index) const
{
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    QString oldValue = index.data().toString();

    if(value == oldValue)
        return;

    model->setData(index,value);
}

void DelegateId::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
