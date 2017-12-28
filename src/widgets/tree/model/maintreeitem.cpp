/*
    maintreeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include "src/widgets/tree/model/maintreeitem.h"
#include <QStringList>

MainTreeItem::MainTreeItem(const QVector<QVariant> &data,
                           MainTreeItem *parent)
    : _property(TAG_NONE, 0)
{
    parentItem = parent;
    itemData = data;
    _itemColor.resize(data.size());
}

MainTreeItem::~MainTreeItem()
{
    qDeleteAll(childItems);
}

MainTreeItem *MainTreeItem::child(int number)
{
    return childItems.value(number);
}

/*
 * @brief Функция возвращает первого встретившегося ребенка, удовлетворяющего
 * веденным параметрам
 */
MainTreeItem *MainTreeItem::child(QString data, int column)
{
    if (column < 0 || column > itemData.size())
        return NULL;

    for(int i = 0; i < childItems.size(); ++i)
    {
        if(data == childItems.value(i)->data(column))
            return childItems.value(i);
    }
    return NULL;
}

int MainTreeItem::childCount() const
{
    return childItems.count();
}

int MainTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<MainTreeItem*>(this));

    return 0;
}

int MainTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant MainTreeItem::data(int column) const
{
    return itemData.value(column);
}

bool MainTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        MainTreeItem *item = new MainTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool MainTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (MainTreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

MainTreeItem *MainTreeItem::parent()
{
    return parentItem;
}

bool MainTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool MainTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (MainTreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool MainTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

QPair<TypeOfShift, int> MainTreeItem::getProperty() const
{
    return _property;
}

void MainTreeItem::setProperty(const QPair<TypeOfShift, int> &value)
{
    _property = value;
}

void MainTreeItem::setProperty(TypeOfShift type, int value)
{
    _property.first = type;
    _property.second = value;
}

QVariant MainTreeItem::getColor(int column) const
{
    return _itemColor.value(column);
}

bool MainTreeItem::setColor(int column, const QBrush &color)
{
    if (column < 0 || column >= _itemColor.size())
        return false;

    _itemColor[column] = color;
    return true;
}


QString MainTreeItem::getToolTip() const
{
    switch(_property.first)
    {
    case TAG_OFFSET:
        return "Абсолютное смещение";
        break;

    case TAG_SHIFT:
        return "Относительное смещение";
        break;

    default:
        break;
    }

    return "";
}

IsVisible MainTreeItem::isVisible() const
{
    return _isVisible;
}

void MainTreeItem::setIsVisible(const IsVisible &isVisible)
{
    _isVisible = isVisible;
}
