#ifndef MAINTREEITEM_H
#define MAINTREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QBrush>
#include "src/sharedinfo/enums.h"

//// Наличие смещения у тега
//enum TypeOfShift              /* меняя здесь, меняй везде! */
//{
//    TAG_OFFSET,
//    TAG_SHIFT,
//    TAG_NONE,
//    TAG_STAT                // Особый тег - "Статистики"
//};

class MainTreeItem
{
public:
    explicit MainTreeItem(const QVector<QVariant> &data, MainTreeItem *parent = 0);
    ~MainTreeItem();

    MainTreeItem *child(int number);
    MainTreeItem *child(QString data, int column);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    MainTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    QPair<TypeOfShift, int> getProperty() const;
    void                    setProperty(const QPair<TypeOfShift, int> &value);
    void                    setProperty(TypeOfShift type, int value);

    QVariant                getColor(int column) const;
    bool                    setColor(int column, const QBrush &color);

    QString                 getToolTip() const;

    IsVisible               isVisible() const;

    void setIsVisible(const IsVisible &isVisible);

private:
    QList<MainTreeItem*>        childItems;
    QVector<QVariant>           itemData;
    MainTreeItem                *parentItem;
    QPair<TypeOfShift, int>     _property;
    QVector<QBrush>             _itemColor;
    IsVisible                   _isVisible;

};

#endif // MAINTREEITEM_H
