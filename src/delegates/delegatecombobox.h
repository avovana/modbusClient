/* *
@file delegatecombobox.h
@brief Делегат для выбора типа переменной
@author Невзоров Владимир
@version 0.5
@copyright (C) 2016 НПФ Вымпел.
@date 05.08.2016
*/

#ifndef DELEGATECOMBOBOX_H
#define DELEGATECOMBOBOX_H
#include <QStyledItemDelegate>
#include <QComboBox>
#include "sharedinfo/type.h"

class DelegateComboBox : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DelegateComboBox    (QObject *parent);
    QWidget             *createEditor           (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index  ) const Q_DECL_OVERRIDE;
    void                setEditorData           (QWidget *editor, const QModelIndex &index                                      ) const Q_DECL_OVERRIDE;
    void                setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index           ) const Q_DECL_OVERRIDE;
    void                updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index  ) const Q_DECL_OVERRIDE;

private:
    //void                remakeRegistersNumbers(QAbstractItemModel *model, const QModelIndex &typeIndex) const;

    QHash<QString, int>             column;
};

#endif // DELEGATECOMBOBOX_H
