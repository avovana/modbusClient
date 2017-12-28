/* *
@file delegatecombobox.h
@brief Делегат для создания имени тэга
@author Невзоров Владимир
@version 0.5
@copyright (C) 2016 НПФ Вымпел.
@date 05.08.2016
*/

#ifndef DELEGATESYMBOL_H
#define DELEGATESYMBOL_H
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QString>

class DelegateSymbols : public QStyledItemDelegate
{
    Q_OBJECT
signals:
    void                valueChanged();

public:
    DelegateSymbols     (QObject *parent = 0, QRegExp newRegExpr = QRegExp("0")                                                 );
    QWidget             *createEditor           (QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index        ) const;
    void                setEditorData           (QWidget *editor, const QModelIndex &index                                      ) const;
    void                setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index           ) const;
    void                updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &       ) const;

private:

    QRegExp             m_rx;
};

#endif // DELEGATESYMBOL_H
