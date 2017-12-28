#ifndef DELEGATEMASK_H
#define DELEGATEMASK_H
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QString>
#include "sharedinfo/type.h"
#include "sharedinfo/col.h"

class DelegateMask : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DelegateMask        (QObject *parent);
    QWidget             *createEditor           (QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &             index) const;
    void                setEditorData           (QWidget *editor, const QModelIndex &) const;
    void                setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index           ) const;
    void                updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &       ) const;

private:

    QString             makeMask(const QModelIndex &index) const;
};

#endif // DELEGATEMASK_H
