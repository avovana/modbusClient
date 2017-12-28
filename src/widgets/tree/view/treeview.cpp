#include "treeview.h"
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QClipboard>
#include <QApplication>

TreeView::TreeView(QWidget *parent)
    : QTreeView(parent)
{

}

void TreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    setCurrentIndex(index);

    QMenu menu(this);

    if(index.parent().isValid())
    {
        menu.addAction("Удалить", this, SLOT(deleteRow()));
        menu.addAction("&Привязать", this, SLOT(createVarList()));
        menu.addAction("&Копировать", this, SLOT(copy()));
    }
    else
    {
        menu.addAction("Удалить", this, SLOT(deleteRow()))->setDisabled(true);
        menu.addAction("&Привязать", this, SLOT(createVarList()))->setDisabled(true);
    }

    if (!menu.isEmpty())
        menu.exec(event->globalPos());
}

void TreeView::keyPressEvent(QKeyEvent * event)
{
    int keyArg = event->key();

    // Получить начальные данные
    // Считаем, что выделена 1 строка
    QModelIndex          index = selectionModel()->currentIndex();
    MainTreeModel *modelInside = static_cast<MainTreeModel*>(model());

    switch (keyArg)
    {
    case Qt::Key_Down:
    case Qt::Key_Up:
    {
        QModelIndex indexToInsert;

        if(keyArg == Qt::Key_Down)
            indexToInsert = index.sibling(index.row() + 1, 0);
        else if (keyArg == Qt::Key_Up)
            indexToInsert = index.sibling(index.row() - 1, 0);

        if(!index.isValid() || !indexToInsert.isValid())
            return;

        QHash<QString, QString> attributes  = modelInside->getTagInfo(index);

        // Удалить строку
        modelInside->removeRow(index.row(), index.parent());
        /* warning
        после удаления, индекс, следующий за index измениться и
        станет указывать на следующую строку
        */

        // Вставить новую строку
        QModelIndex newIndex  = modelInside->insertTag(INSERT_BEFORE, indexToInsert, attributes);

        // Установить выделение
        setCurrentIndex(newIndex);
    }
        break;

    case Qt::Key_Delete:
        modelInside->removeRow(index.row(), index.parent());
        break;

    default:
        break;
    }
}

void TreeView::deleteRow()
{
    QModelIndex          index = selectionModel()->currentIndex();
    MainTreeModel *modelInside = static_cast<MainTreeModel*>(model());
    modelInside->removeRow(index.row(), index.parent());
}

void TreeView::createVarList()
{
    if(_variablesProvider == NULL)
        return;

    // Взять тип
    QModelIndex     index = selectionModel()->currentIndex();
    int               row = index.row();
    QModelIndex indexType = index.parent().child(row, Col::num("Тип"));

    // Создать виджет с доступными переменными для перезаписи текущей строки
    VariableList *varList = new VariableList(_variablesProvider, indexType.data().toString(), RECEIVED_TYPE);
    //connect(varList, &VariableList::setEnabled, this, &ProcessTree::setEnabled);
    connect(varList, &VariableList::usingVariable, this, &TreeView::updateRow);

    // Запретить любые действия пользователя с интерфейсом
    //emit setEnabled(false);

    // Показать созданный виджет
    varList->show();
}

void TreeView::copy()
{
    QModelIndex     index = selectionModel()->currentIndex();
    QString value = index.data().toString();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(value);
}

void TreeView::updateRow(QString varName, ReceivedData /*dataType*/)
{
    // Взять нужные параметры текущей строки
    QModelIndex index                               = selectionModel()->currentIndex();
    MainTreeModel *modelInside                      = static_cast<MainTreeModel*>(model());//или dynamic_cast<derv_class *>(base_class_ptr_expr)?
    QHash<QString, QString> attributesCurrentRow    = modelInside->getTagInfo(index);

    if(!index.isValid())
        return;

    MainTreeItem *internalData = (MainTreeItem *)index.internalPointer();
    TypeOfShift      shiftType = internalData->getProperty().first;
    int                 number = internalData->getProperty().second;

    // Если такое имя уже используется
    if(modelInside->getBindedVars().contains(varName))
    {
        modelInside->error("var binded");
        return;
    }

    if(internalData->parent()->data(0) == "internal")  //_columnsInfo
        return;

    // Удалить строку
    deleteRow();

    /// Добавление переменной
    // Получить переменную
    cAbstractVariable *varPLC = _variablesProvider->variablesMap().value(varName);

    // Создать хэш аттрибутов
    QHash<QString, QString> attributes;
    attributes["name"]        = varPLC->name();
    attributes["type"]        = varPLC->types().first()->name();
    attributes["bin_mask"]    = ""; // расчитать маску
    attributes["id"]          = attributesCurrentRow.value("id");
    attributes["description"] = varPLC->documentation();
    attributes["shiftType"]   = QString::number(shiftType);
    attributes["number"]      = QString::number(number);
    attributes["external"]    = varName;

    // Вставить новую строку
    QModelIndex      newIndex = modelInside->insertTag(INSERT_AFTER, selectionModel()->currentIndex(), attributes);

    // Установить выделение
    setCurrentIndex(newIndex);
}

void TreeView::setVariablesProvider(cVariablesProvider *variablesProvider)
{
    _variablesProvider = variablesProvider;
}
