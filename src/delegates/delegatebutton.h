#ifndef DELEGATEBUTTON_H
#define DELEGATEBUTTON_H
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QString>
#include <QHash>
#include "src/widgets/lineedit/lineedit.h"

class DelegateButton : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DelegateButton(QObject *, QRegExp newRegExpr, cVariablesProvider *variablesProvider, QString type);
    QWidget             *createEditor           (QWidget *parent, const QStyleOptionViewItem &, const QModelIndex&) const;
    void                setEditorData           (QWidget *editor, const QModelIndex &index                                      ) const;
    void                setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index           ) const;
    void                updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &       ) const;
signals:
    void error(QString message) const;
private:
    QRegExp             _m_rx;
    cVariablesProvider *_variablesProvider;
    QString             _typeToChoose;
};

#endif // DELEGATEBUTTON_H
