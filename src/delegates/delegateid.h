#ifndef DELEGATEID_H
#define DELEGATEID_H
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QString>
#include <QHash>

class DelegateId : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DelegateId(QObject *, QRegExp newRegExpr);
    QWidget             *createEditor           (QWidget *parent, const QStyleOptionViewItem &, const QModelIndex&) const;
    void                setEditorData           (QWidget *editor, const QModelIndex &index                                      ) const;
    void                setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index           ) const;
    void                updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &       ) const;
signals:
    void error(QString message) const;
private:
    QRegExp           m_rx;
};

#endif // DELEGATEID_H
