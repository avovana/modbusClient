#include "delegatecombobox.h"
#include <QDebug>

DelegateComboBox::DelegateComboBox(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *DelegateComboBox::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &index) const
{
    if (index.data(Qt::UserRole).toInt() == 1)                  /* для блокировки поля */
        return NULL;

    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->setEditable(false);

    editor->addItems(QStringList(Type::names()));

    return editor;
}

void DelegateComboBox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(value);
}

void DelegateComboBox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void DelegateComboBox::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

