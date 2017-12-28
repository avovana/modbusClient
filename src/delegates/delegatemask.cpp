#include "delegatemask.h"
#include <QDebug>

DelegateMask::DelegateMask(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *DelegateMask::createEditor(QWidget* parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    if (index.data(Qt::UserRole).toInt() == 1)                  /* для блокировки поля */
        return NULL;

    QLineEdit* editor = new QLineEdit(parent);

    QString mask = makeMask(index);
    editor->setInputMask(mask);
    editor->setText(index.data().toString());

    return editor;
}

void DelegateMask::setEditorData(QWidget */*editor*/, const QModelIndex &/*index*/) const
{
}

void DelegateMask::setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex &index) const
{
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    QString value = line->text();

    model->setData(index,value);
}

void DelegateMask::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

QString DelegateMask::makeMask(const QModelIndex &index) const
{
    // Получили текст из combobox'a с типов тега
    QModelIndex typeIndex = index.model()->index(index.row(),Col::num("Тип"),index.parent());
    QString type = typeIndex.data().toString();
    // Сделали маску исходя из типа тега
    int quantityOfBools = 16 * Type::bytes(type);
    QString mask;
    mask.fill(QChar('b'),quantityOfBools);
    mask.append(tr(";_"));

    return mask;
}
