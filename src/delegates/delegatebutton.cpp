#include "src/delegates/delegatebutton.h"

DelegateButton::DelegateButton(QObject* parent,
                               QRegExp newRegExpr,
                               cVariablesProvider *variablesProvider,
                               QString type)
    : QStyledItemDelegate(parent),
      _m_rx(newRegExpr),
      _variablesProvider(variablesProvider),
      _typeToChoose(type)
{

}

QWidget *DelegateButton::createEditor(QWidget* parent, const QStyleOptionViewItem &, const QModelIndex& /*index*/) const
{
    LineEdit* editor = new LineEdit(parent, _variablesProvider, _typeToChoose);
    
    QRegExpValidator *val = new QRegExpValidator(_m_rx, editor);
    editor->setValidator(val);
    
    return editor;
}

void DelegateButton::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::EditRole).toString();

    LineEdit* line = static_cast<LineEdit*>(editor);
    line->setText(value);
}

void DelegateButton::setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex &index) const
{
    LineEdit* line = static_cast<LineEdit*>(editor);
    QString value = line->text();
    QString oldValue = index.data().toString();

    if(value == oldValue)
        return;

    model->setData(index,value);
}

void DelegateButton::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
