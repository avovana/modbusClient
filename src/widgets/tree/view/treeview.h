#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include "src/widgets/tree/model/maintreemodel.h"
#include "src/widgets/variablelist/variablelist.h"

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget *parent);

    virtual void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void setVariablesProvider(cVariablesProvider *variablesProvider);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void deleteRow();
    void createVarList();
    void copy();
    void updateRow(QString varName, ReceivedData);

private:
    cVariablesProvider *_variablesProvider; // Указатель для доступа к переменным
};

#endif // TREEVIEW_H
