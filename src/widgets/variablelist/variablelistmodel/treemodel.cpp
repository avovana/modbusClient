#include "treeitem.h"
#include "treemodel.h"
#include <QStringList>

TreeModel::TreeModel(cVariablesProvider *variablesProvider,
                     QString data,
                     ReceivedData dataType,
                     QObject *parent)
    : QAbstractItemModel(parent),
     _data(data),
     _dataType(dataType)
{
    QList<QVariant> rootData;
    rootData << "Переменные";

    rootItem = new TreeItem(rootData);

    auto varMap = variablesProvider->variablesMap();

    TreeItem *item = rootItem;

    // Пройтись по всем переменным в карте переменных ПЛК-Дизайнера
    foreach (auto fullPath, varMap.keys())
    {
        cAbstractVariable   *varPLC = varMap.value(fullPath);
        //QString             varType = varPLC->types().first()->name();

        //item = addElement(fullPath, varType, item);
        item = addElement(fullPath, varPLC, item);
    }

    /* Правильное отображение одним списком
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";

    rootItem = new TreeItem(rootData);
    auto varMap = variablesProvider->variablesMap();

    TreeItem *item = rootItem;

    // Пройтись по всем переменным в карте переменных ПЛК-Дизайнера
    foreach (auto fullPath, varMap.keys())
    {
        cAbstractVariable *varPLC = varMap.value(fullPath);

        QList<QVariant> columnData;
        columnData << fullPath;
        columnData << fullPath;

        TreeItem *newItem = new TreeItem(columnData, rootItem);
        rootItem->appendChild(newItem);
    }
        */
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((role != Qt::DisplayRole) &&
        (role != DataRoles::DATA_FULL_NAME) &&
        (role != DataRoles::DATA_TYPE))
    {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    if(role == DataRoles::DATA_FULL_NAME)
    {
        return item->data(FULL_NAME);
    }

    if(role == DataRoles::DATA_TYPE)
    {
        return item->data(DataTypes::TYPE);
    }

    return item->data(SHORT_NAME);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    switch(_dataType)
    {
    case RECEIVED_TYPE:
    {
        QString type = index.data(DataRoles::DATA_TYPE).toString();

        if(type == _data)
        {
            return QAbstractItemModel::flags(index);
        }
        else
        {
            return Qt::NoItemFlags;
        }
        break;

    }
    case RECEIVED_NAME:
    {
        QString name = index.data(DataRoles::DATA_SHORT_NAME).toString();

        if(name == _data)
        {
            return QAbstractItemModel::flags(index);
        }
        else
        {
            return Qt::NoItemFlags;
        }
        break;
    }

    case RECEIVED_ANY:
        return QAbstractItemModel::flags(index);
        break;

    default:
        break;
    }

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        QList<QVariant> headerData;
        headerData << "Переменные";

        if (section < headerData.count())
            return headerData.at(section);
        else
            return QVariant();
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

/*
 * @brief Функция для определения вхождения одной строки в другую
 * @param test     - строка для проверки
 * @param main     - основная строка
 * @warning        - подразумеваются строки с "."(точками)
 */
bool TreeModel::isContain(QString test, QString main)
{
    // Вычленить подстроки
    QStringList first1 = test.split(".");
    QStringList second2 = main.split(".");

    // Пройтись по каждой подстроке 1ой строки
    for(int i = 0; i < first1.count(); ++i)
    {
        // Флаг успеха поиска подстроки 1ой строки в подстроках 2ой строки
        bool find = false;

        // Пройтись по каждой подстроке 2ой строки
        for(int j = 0; j < second2.count(); ++j)
        {
            // Если подстроки одинаковые
            if(first1.at(i) == second2.at(j))
            {
                // Убрать из следующего рассмотрения найденную подстроку
                second2.removeAt(j);

                // Установить флаг успеха поиска подстроки
                find = true;
                break;
            }
        }

        if(find == false)
            return false;
    }
    return true;
}

/*
 * @brief Функция для добавления нового элемента в дерево
 * @param name      - название
 * @param type      - тип
 * @param parent    - родитель
 */
TreeItem* TreeModel::addElement(QString name, cAbstractVariable *varPLC, TreeItem *parent)
{
    QString type = varPLC->types().first()->name();

    /// 1. Добавление элемента в определенный узел
    // Делать пока есть родитель
    while(parent->parentItem() != 0)
    {
        // Получить название родителя
        QString parentName = parent->data(FULL_NAME).toString();


        // Определить вхождение названия родителя в текущее название
        if (isContain(parentName, name))
        {
            // Добавить новый элемент
            QList<QVariant> data;
            data << name;                       // [0]FULL_NAME
            data << name.split(".").takeLast(); // [1]SHORT_NAME
            data << type;                       // [2]TYPE
            //data << varPLC;                     // [3]varPLC
            data << qVariantFromValue((void *) varPLC);

            //yourPointer = (YourClass *) v.value<void *>();


            TreeItem *item = new TreeItem(data, parent);
            parent->appendChild(item);
            return item;
        }
        else
        {
            parent = parent->parentItem();
        }
    }

    /// 2. Добавление элемента в самый верхний уровень
    // Добавить новый элемент
    QList<QVariant> data;
    data << name;                       // [0]FULL_NAME
    data << name.split(".").takeLast(); // [1]SHORT_NAME
    data << type;                       // [2]TYPE
    data << qVariantFromValue((void *) varPLC);
    //data << varPLC;                     // [3]varPLC
    TreeItem *item = new TreeItem(data, parent);
    parent->appendChild(item);
    return item;
}
