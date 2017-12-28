#include <QtWidgets>

#include "src/widgets/tree/model/maintreeitem.h"
#include "src/widgets/tree/model/maintreemodel.h"

// warning важен порядок инициализации, т.е. порядок расположение членов в h. файле
MainTreeModel::MainTreeModel(QDomNode root,
                             cVariablesProvider *variablesProvider,
                             QObject *parent)
    : QAbstractItemModel(parent),
      _root(root),
      _variablesProvider(variablesProvider),
      _parser(root),
      _globalVars(_parser.getGlobalVars())

{
    /// Создание названий столбцов
    QVector<QVariant> rootData;
    foreach (QString header, Col::headers())
        rootData << header;

    _rootItem = new MainTreeItem(rootData);

    /// Добавление групп строк
    QVector<QVariant> rowData;
    rowData << "ir" << "hr" << "di" << "co" << "internal";
    for (int i = 0; i < rowData.count(); ++i)
    {
        //rowData << columnStrings[i];
        _rootItem->insertChildren(_rootItem->childCount(), 1, _rootItem->columnCount());
        _rootItem->child(_rootItem->childCount() - 1)->setData(0, rowData[i]);
    }

    QDomElement baseElement = _root.firstChildElement();
    readXml(baseElement);

    //setupModelData(regInfo.split(QString("\n")), rootItem);
}

MainTreeModel::~MainTreeModel()
{
    delete _rootItem;
}

int MainTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return _rootItem->columnCount();
}

QVariant MainTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    /// Сокрытие тегов функционального блока
    QString internal = getItem(index)->parent()->data(Col::num("Регистры")).toString();
    if(internal == "internal")
        if(getItem(index)->isVisible() == PROP_UNVISIBLE)
            return "";

    if (role != Qt::DisplayRole     &&
        role != Qt::EditRole        &&
        role != Qt::BackgroundRole  &&
        role != Qt::ToolTipRole)
        return QVariant();

    if (role == Qt::BackgroundRole)
        return getItem(index)->getColor(index.column());

    if (role == Qt::ToolTipRole)
    {
        if(index.column() == Col::num("Регистры"))
            return getItem(index)->getToolTip();
        else if(index.column() == Col::num("Статус записи"))
            return "Задание привязанных переменных статуса записи "
                   "позволяет отслеживать выполнение операций записи "
                   "службой клиента Modbus";
        else if(index.column() == Col::num("Статус чтения"))
            return "Задание привязанных переменных статуса чтения "
                   "позволяет отслеживать выполнение операций чтения "
                   "значения тега службой клиента Modbus";
        else
            return QVariant();
    }

    /// Переименование группы "Статистика"
    // Если строка - групповая
    if(getItem(index)->parent() == _rootItem)
    {
        QString groupName = getItem(index)->data(Col::num("Регистры")).toString();

        if(groupName == "internal")
            if(index.column() == Col::num("Регистры"))
                return "Статистика";
    }



    /// Обнуление значений столбца для группы "Статистика"
    QString groupName = getItem(index)->parent()->data(Col::num("Регистры")).toString();
    // Если строка из группы "internal"
    if(groupName == "internal")
        if(index.column() == 0)
            return "";

//    /// Сокрытие тегов функционального блока
//    QString groupName2 = getItem(index)->parent()->data(Col::num("Регистры")).toString();
//    // Если строка из группы "internal"
//    if(groupName2 == "internal")
//        if(index.column() == 0)
//            return "";

    MainTreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags MainTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    // Если строка - групповая
    if(getItem(index)->parent() == _rootItem)
        return QAbstractItemModel::flags(index);

    QString groupName = getItem(index)->parent()->data(Col::num("Регистры")).toString();

    // Если строка из группы "internal"
    if(groupName == "internal")
        return QAbstractItemModel::flags(index);

    // Если столбец "Приоритет"
    if(index.column() == Col::num("Приоритет"))
    {
        if(groupName == "hr" || groupName == "co")
            Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        else
            return QAbstractItemModel::flags(index);
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

MainTreeItem *MainTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return _rootItem;
}

QList<QString> MainTreeModel::getListID() const
{
    QList<QString> listID;

    for(int childNum = 0; childNum < _rootItem->childCount(); ++childNum)
    {
        MainTreeItem *itemGroup = _rootItem->child(childNum);

        for(int i = 0; i < itemGroup->childCount(); ++i)
        {
            QString id = itemGroup->child(i)->data(Col::num("Идентификатор привязанной переменной")).toString();
            listID.push_back(id);
        }
    }

    return listID;
}

QList<QString> MainTreeModel::getBindedVars()
{
    QList<QString> bindVariables;

    for(int childNum = 0; childNum < _rootItem->childCount(); ++childNum)
    {
        MainTreeItem *itemGroup = _rootItem->child(childNum);

        for(int i = 0; i < itemGroup->childCount(); ++i)
        {
            QString id   = itemGroup->child(i)->data(Col::num("Идентификатор привязанной переменной")).toString();
            QString bind = itemGroup->child(i)->data(Col::num("Привязка по значению")).toString();

            if(bind.isEmpty())
                bindVariables.push_back(id);
            else
                bindVariables.push_back(bind);
        }
    }

    return bindVariables;
}

QVariant MainTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex MainTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    MainTreeItem *parentItem = getItem(parent);

    MainTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

//bool MainTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
//{
//    bool success;
//    beginInsertColumns(parent, position, position + columns - 1);
//    success = rootItem->insertColumns(position, columns);
//    endInsertColumns();

//    return success;
//}

bool MainTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    MainTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, _rootItem->columnCount());
    endInsertRows();

    return success;
}

/*
 * @brief Функция предоставляет иформацию со строки
  */
QHash<QString, QString> MainTreeModel::getTagInfo(QModelIndex index)
{
    MainTreeItem *tag = getItem(index);

    // Создать хэш аттрибутов
    QHash<QString, QString> attributes;
    attributes["name"]        = tag->data(Col::num("Наименование")).toString();
    attributes["type"]        = tag->data(Col::num("Тип")).toString();
    attributes["bin_mask"]    = tag->data(Col::num("Маска")).toString();
    attributes["id"]          = tag->data(Col::num("Идентификатор привязанной переменной")).toString();
    attributes["description"] = tag->data(Col::num("Описание")).toString();
    attributes["shiftType"]   = QString::number(tag->getProperty().first);
    attributes["number"]      = QString::number(tag->getProperty().second);

    return attributes;
}

/*
 * @brief Функция добавляет новый элемент дерева
  */
QModelIndex MainTreeModel::insertTag(PositionToInsert insertType,
                                    const QModelIndex &index,
                                    QHash<QString, QString> attributes)
{
    /// Выполнение основных проверок
    // Проверить индекс
    if(index == QModelIndex())
        return QModelIndex();

    // Установить родителя тэга
    MainTreeItem *tagGroup = getItem(index.parent());

    if(tagGroup == _rootItem)
    {
        tagGroup = getItem(index);
    }

    // Если группа "internal"
    QString groupName = tagGroup->data(Col::num("Регистры")).toString();
    if(groupName == "internal")
        return QModelIndex();

    // Проверить ID
    QString id = attributes.value("id");
    if(attributes.value("id") == "")
    {
        emit error("id empty");
        return QModelIndex();
    }

    // Получить все используемые Id
    QList<QString> listID = getListID();

    if(listID.contains(id))
    {
        emit error("id exists");
        return QModelIndex();
    }

    QString external = attributes.value("external");
    // Создать перечень с значениями, участвующими в привязки
    QList<QString> bindVariables = getBindedVars();

    // Если введеное значение уже используется
    if(bindVariables.contains(external))
    {
        emit error("var binded");
        return QModelIndex();
    }

    /// Определение позици для вставки
    // Определить позицию
    int insertPos = 0;
    switch (insertType)
    {
    case INSERT_BEFORE:
        insertPos = index.row();
        break;

    case INSERT_AFTER:
        insertPos = index.row() + 1;
        break;

    case INSERT_START:
        insertPos = 0;
        break;

    case INSERT_END:
        insertPos = tagGroup->childCount();
        break;

    default:
        break;
    }

    /// Добавление нового тэга
    // Добавить тег
    MainTreeItem *tag       = createTag(tagGroup, insertPos, attributes);
    MainTreeItem *parent    = tag->parent();
    MainTreeItem *tagPrev   = parent->child(tag->childNumber() - 1);

    // Пересчитать № Регистров в зависимости от наличия смещения
    TypeOfShift shiftType   = (TypeOfShift)attributes.value("shiftType").toInt();
    int               pos   = tag->childNumber();
    switch (shiftType)
    {
    case TAG_OFFSET:
        calculateAdresses(tag);
        break;

    case TAG_SHIFT:
        if(pos >= 1)
            calculateAdresses(tagPrev);
        else if (pos == 0)
            calculateAdresses(tag);
        break;

    case TAG_NONE:
        if(pos >= 1)
            calculateAdresses(tagPrev); //или calculateRegNumbers(tag->parent()->child(tag->childCount() - 1));
        else if (pos == 0)
            calculateAdresses(tag);
        //у 0-ого считать не будем
        break;

    case TAG_STAT:
        break;

    default:
        // Пересчитать все № Регистров начиная с начала чтобы дать № Регистра и новой строке
        if(parent->childCount() > 0)
            calculateAdresses(parent->child(0));
        break;
    }

    return createIndex(tag->childNumber(), 0, tag);
}

QModelIndex MainTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MainTreeItem *childItem = getItem(index);
    MainTreeItem *parentItem = childItem->parent();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool MainTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = _rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (_rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool MainTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    /* Если индекс дейвствителен и не представляющий тег с названием группы.
       Т.к. сюда может придти parent группы(к примеру parent группы "ir").
       В этой модели, такой parent будет invalid
    */
    if(!parent.isValid())
        return false;

    MainTreeItem *parentItem = getItem(parent);
    bool success             = true;

    // Если строка из группы "internal"
    //QString groupName = parentItem->data(_columnsInfo->getColumn("Регистры")).toString();
    //if(groupName == "internal")
        //return false;

    // Удалить элемент
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    // Пересчитать номера регистров
    if(position >= 1)
    {
        calculateAdresses(parentItem->child(position - 1));
    } else if (position == 0)
    {
        if (parentItem->childCount() != 0)
            calculateAdresses(parentItem->child(position));
    }

    return success;
}

int MainTreeModel::rowCount(const QModelIndex &parent) const
{
    MainTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool MainTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    MainTreeItem *item = getItem(index);
    int     currColumn = index.column();

    // 2. Поле "Регистры" изменено?
    if(currColumn == Col::num("Регистры"))
    {
        // Очистили поле?
        if(value.toString() == "")
        {
            TypeOfShift shiftType = TypeOfShift(item->getProperty().first);

            if(shiftType == TAG_OFFSET)
            {
                item->setProperty(TAG_NONE, 0);
                item->setColor(currColumn, QBrush());
            }

            int position = item->childNumber();

            if(position >= 1)
            {
                // Пересчитать поля у данной строки и всех последующих строк в группе
                calculateAdresses(item->parent()->child(position - 1));
            } else if (position == 0)
            {
                // Пересчитать поля у строк, начиная со следующей
                calculateAdresses(item->parent()->child(position));
            }
        }
        else
        {
            item->setProperty(TAG_OFFSET, value.toInt());
            item->setData(currColumn, value.toString());
            item->setColor(currColumn, QBrush(QColor("lightskyblue")));

            calculateAdresses(item);
        }
        dataChanged(index,index); // <--- так более правильно. Или можно так ---> createIndex(item->childNumber(), 0, item)

        return true;
    }

    // 3. Поле "Тип" изменено?
    if(currColumn == Col::num("Тип"))
    {
        if(item->parent()->childCount() != 0)
        {
            item->setData(currColumn, value.toString());
            calculateAdresses(item);
        }

        return true;
    }

    // 4. Поле "Идентификатор привязанной переменной" изменено?
    if(currColumn == Col::num("Идентификатор привязанной переменной"))
    {
        QString newId       = value.toString();
        QString bindValue   = item->data(Col::num("Привязка по значению")).toString();

        // Получить все используемые Id
        QList<QString> listID = getListID();

        // Проверить id
        if((listID.contains(newId)))
        {
            emit error("id used");
            return false;
        }

        // Если поле в столбце "Привязка" пустое
        if(bindValue.isEmpty())
        {
            // Создать перечень с значениями, участвующими в привязки
            QList<QString> bindVariables = getBindedVars();

            // Если введеное значение уже используется
            if(bindVariables.contains(newId))
            {
                emit error("var binded");
                return false;
            }

            // Проверить привязку
            if((_variablesProvider->variablesMap().value(newId)     == NULL) &&
               (_variablesProvider->variablesMap().value(bindValue) == NULL) &&
               !_globalVars.contains(newId) &&
               !_globalVars.contains(bindValue))          // if(variablesProvider->variable(item->text(column)) == NULL) пока не работает
                item->setColor(currColumn, QBrush(QColor("lightsalmon")));
            else
                item->setColor(currColumn, QBrush());

            item->setData(currColumn, newId);

            dataChanged(index,index);
            return true;
        }
        else
        {
            // Проверить привязку
            if((_variablesProvider->variablesMap().value(newId)     == NULL) &&
               (_variablesProvider->variablesMap().value(bindValue) == NULL) &&
                !_globalVars.contains(newId) &&
                !_globalVars.contains(bindValue))
                item->setColor(currColumn, QBrush(QColor("lightsalmon")));
            else
                item->setColor(currColumn, QBrush());

            item->setData(currColumn, newId);
            dataChanged(index,index);
            return true;
        }
    }

    // 5. Поле "Привязка по значению" изменено?
    if(currColumn == Col::num("Привязка по значению"))
    {
        QString bindNew   = value.toString();
        int columnBind    = currColumn;
        int columnID      = Col::num("Идентификатор привязанной переменной");

        // Если обнулили поле
        if(bindNew.isEmpty())
        {
            // Получить текущие значения
            QString bindOld = item->data(columnBind).toString();
            QString idOld   = item->data(columnID).toString();

            // Установить новое значение
            item->setData(columnBind, bindNew); // item->setData(columnBind, "");

            /*Теперь значение идентификатора становиться и значением для привязки*/
            // Проверить - не используется ли такое значение для привязки

            // Исключить на время текущее значение идентификатора
            item->setData(columnID, "");
            QModelIndex indexId = createIndex(item->childNumber(), columnID, item);

            if(setData(indexId, idOld) == false)
            {
                item->setData(columnID, idOld);
                item->setData(columnBind, bindOld);
            }

            dataChanged(index,index);
            return true;
        }
        // Если внесли какое-то значение
        else
        {
            // Исключить из последующего списка текущее значение // Исключить на время текущее значение идентификатора
            QString idOld = item->data(columnID).toString();
            item->setData(columnID, "");

            // Создать перечень с значениями, участвующими в привязки
            QList<QString> bindVariables = getBindedVars();

            // Если введеное значение уже используется
            if(bindVariables.contains(bindNew))
            {
                item->setData(columnID, idOld);
                emit error("var binded");
                return true;
            }

            // Восстановить значение
            item->setData(columnID, idOld);

            // Проверить привязку
            if((_variablesProvider->variablesMap().value(bindNew) == NULL) &&
               (_variablesProvider->variablesMap().value(idOld)   == NULL) &&
                !_globalVars.contains(bindNew) &&
                !_globalVars.contains(idOld))
                item->setColor(columnID, QBrush(QColor("lightsalmon")));
            else
                item->setColor(columnID, QBrush());

            item->setData(columnBind, bindNew);

            dataChanged(index,index);
            return true;
        }
    }

    // 5. Поле "Статус записи" изменено?
    if(currColumn == Col::num("Статус записи"))
    {
        QString newStatus = value.toString();

        // Проверить привязку
        if(_variablesProvider->variablesMap().value(newStatus) == NULL)
            item->setColor(currColumn, QBrush(QColor("lightsalmon")));
        else
            item->setColor(currColumn, QBrush());

        item->setData(currColumn, newStatus);

        dataChanged(index,index);
        return true;
    }

    // 6. Поле "Привязка по значению" изменено?
    if(currColumn == Col::num("Статус чтения"))
    {
        QString newStatus = value.toString();

        // Проверить привязку
        if(_variablesProvider->variablesMap().value(newStatus) == NULL)
            item->setColor(currColumn, QBrush(QColor("lightsalmon")));
        else
            item->setColor(currColumn, QBrush());

        item->setData(currColumn, newStatus);

        dataChanged(index,index);
        return true;
    }

    bool result = item->setData(currColumn, value.toString());
    dataChanged(index,index);

    if (result)
        return true;

    //5. Оповестить верхний класс
    //emit treeUpdated();

    return false;
}

bool MainTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = _rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

/*
 * @brief Функция по импорту Xml файла modbus-базы
 */
bool MainTreeModel::open(QIODevice *device)
{
    //Открыть Xml файл
    QDomDocument doc;

    device->open(QIODevice::ReadOnly | QIODevice::Text);
    QString data =  QString::fromUtf8(device->readAll());
    device->close();

    QString errorMsg;
    int errorLine=0;
    int errorColumn=0;
    if (!doc.setContent (data,&errorMsg,&errorLine,&errorColumn))
    {
        // Why "emit" and return and the same time?
        emit error("readFile");
        return false;
    }

    // Заполнить дерево информацией из файла
    readXml(doc.firstChildElement());
    return true;
}
/*
 * @brief Функция по экспорту файла modbus-базы
 */
void MainTreeModel::save(QIODevice *device)
{
    // Получить базовую ноду
    QDomElement     baseNode;
    QDomDocument    domDocument;
    baseNode      = domDocument.createElement("base");

    //TODO Попробовать присовить
    // Получить ноду с информацией модели
    QDomElement modelInfo = makeXml();
    if(!modelInfo.isNull())
    {
        while(modelInfo.childNodes().length() >= 1)
        {
            // Заполнить базовую ноду информацией с модели
            baseNode.appendChild(modelInfo.firstChildElement());
            modelInfo.removeChild(baseNode.firstChild());
        }
    }

    // Записать информацию в файл
    //QDomDocument domDocument;
    domDocument.appendChild(baseNode);

    QTextStream out(device);
    domDocument.save(out, 4);
}

/*
 * @bried Функция для заполнения элемента Xml
 */
void MainTreeModel::save()
{
    // Убедиться в наличии базовой ноды
    if(!_root.hasChildNodes())
    {
        QDomElement     baseNode;
        QDomDocument    domDocument;
        baseNode      = domDocument.createElement("base");
        _root.appendChild(baseNode);
    }

    /// Запись атрибутов
    // Получить базовую ноду
    QDomElement baseNode = _root.firstChildElement();

    // Записать версию
    qlonglong _base_version = baseNode.attribute("base_version").toInt();
    _base_version = _base_version + 1;
    baseNode.setAttribute("base_version", QString::number(_base_version));

    // Записать время сборки
    baseNode.setAttribute("date_time", QDateTime::currentDateTime().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString("hh:mm:ss"));

    /// Заполнение базовой ноды
    // Удалить всех детей ноды
    if (baseNode.hasChildNodes())
    {
        while(baseNode.childNodes().length() >= 1)
        {
            baseNode.removeChild(baseNode.firstChild());
        }
    }

    // Получить ноду с информацией модели
    QDomElement modelInfo = makeXml();
    if(!modelInfo.isNull())
    {
        while(modelInfo.childNodes().length() >= 1)
        {
            // Заполнить базовую ноду информацией с модели
            baseNode.appendChild(modelInfo.firstChildElement());
            modelInfo.removeChild(baseNode.firstChild());
        }
    }
}

/*
 * @brief Функция считывает информацию с строки и создает тэг
 */
QDomElement MainTreeModel::makeXmlTag(MainTreeItem *parent, int row)
{
    if (row < 0 || row > parent->childCount() - 1)
        return QDomElement();

    QDomDocument domDocument;
    QDomElement tagXml = domDocument.createElement("tag");

    MainTreeItem *tag = parent->child(row);

    // Пройтись по всем значениям в строке
    for(int col = 0; col < tag->columnCount(); ++col)
    {
        // Получить очередной атрибут
        QString value       = tag->data(col).toString();
        QString atrrName    = Col::attr(col);

        // Если пустое значение
        if(value == "")
            continue;

        // Если атрибут "number"
        if(atrrName == "number")
            continue;

        // Добавить атрибут
        if(atrrName == "type")
            tagXml.setAttribute(atrrName, Type::shortName(value));
        else
            tagXml.setAttribute(atrrName, value);
    }

    // Добавить смещение
    TypeOfShift shiftType = (TypeOfShift)tag->getProperty().first;
    switch (shiftType)
    {
    case TAG_OFFSET:
        tagXml.setAttribute("offset", tag->getProperty().second);
        break;

    case TAG_SHIFT:
        tagXml.setAttribute("shift", tag->getProperty().second);
        break;

    case TAG_NONE:
        break;

    default:

        break;
    }

    return tagXml;
}

QDomElement MainTreeModel::makeXml()
{
    QDomElement baseNode;

    QDomDocument domDocument;
    baseNode = domDocument.createElement("base");

    // 1. Создаем теги типов регистров(XML)
    for(int i = 0; i < _rootItem->childCount(); ++i)
    {
        // Название группы
        MainTreeItem *regGroup = _rootItem->child(i);
        QString regGroupName   = _rootItem->child(i)->data(Col::num("Регистры")).toString();

        QDomElement regGroupXml;

        QDomDocument domDocument;
        regGroupXml = domDocument.createElement(regGroupName);
        //baseNode.appendChild(regGroupXml);

        // 2. Создаем теги XML
        for(int row = 0; row < regGroup->childCount(); ++row)
        {// здесь проходимся по всем строкам(row), находящимся в данном типе регистров(type)

            // 3. Заполняем тег свойствами
            QDomElement tag = makeXmlTag(regGroup, row);

            if(tag.isNull())
                return QDomElement();

            // 4. Добавляем тег в тег типа регистров(XML)
            regGroupXml.appendChild(tag);
        }

        // 5. Добавить созданную группу с тегами в основную ноду
        baseNode.appendChild(regGroupXml);
    }

    return baseNode;
}

/*
 * @brief Функция считывает информацию взятую из Xml
 * @warning Сложно читать функцию из-за особого обращения
 * с тегами группы "internal"
 */
void MainTreeModel::readXml(QDomElement baseElement)
{
    /// Считывание информации
    // Пройтись по группам регистров
    for (QDomElement tagGroupXml = baseElement.firstChildElement(); !tagGroupXml.isNull(); tagGroupXml = tagGroupXml.nextSiblingElement())
    {
        MainTreeItem *tagGroup = _rootItem->child(tagGroupXml.tagName(), Col::num("Регистры"));

        if(tagGroup == NULL)
            continue;

        // Пройтись по тегам в данной группе регистров
        for (QDomElement tagXml = tagGroupXml.firstChildElement(); !tagXml.isNull(); tagXml = tagXml.nextSiblingElement())
        {
            /// Получение информации для новой строки
            // Взять значения аттрибутов
            QString           name = tagXml.attribute("name");
            QString           type = Type::fullName(tagXml.attribute("type"));
            QString           mask = "";
            QString       bin_mask = "";
            QString             id = tagXml.attribute("id");
            QString    description = tagXml.attribute("description");
            QString         number = "";
            QString       external = tagXml.attribute("external");
            QString       priority = tagXml.attribute("priority");
            QString   status_write = tagXml.attribute("status_write");
            QString    status_read = tagXml.attribute("status_read");
            QString    extra_read  = tagXml.attribute("extra_read");
            QString    extra_write = tagXml.attribute("extra_write");
            TypeOfShift  shiftType = TAG_NONE;
            QString    grp_number  = tagXml.attribute("grp_number");

            /// Изменение некоторых атрибутов
            // Считать маску
            // Eсть "bin_mask"?
            if(tagXml.hasAttribute("bin_mask"))
            {
                bin_mask = tagXml.attribute("bin_mask");
            }
            // Eсть "mask"?
            else if(tagXml.hasAttribute("mask"))
            {
                mask = tagXml.attribute("mask");

                bool ok = false;
                // Привезти значение mask к двоичному виду
                quint64 maskValue = mask.toULongLong(&ok, 0);
                bin_mask = QString::number(maskValue, 2);
            }

            // Считать смещение
            // Есть "offset"?
            if(tagXml.attribute("offset") != "")
            {
                shiftType = TAG_OFFSET;
                number    = tagXml.attribute("offset");
            }
            // Есть "shift"?
            else if(tagXml.attribute("shift") != "")
            {
                shiftType = TAG_SHIFT;
                number    = tagXml.attribute("shift");
            }

            // Если не теги статистики
            if(!(tagGroupXml.tagName() == "internal"))
                if(grp_number == "")
                    grp_number = "0";

            // Изменить повторяющейся id
            id = makeUniqueID(id);

            // Убрать все недопустимые символы
            // [^\\w\\.] - все не буквы, не точка
            id.replace(QRegExp("[^\\w\\.]"), "_");

            // Создать хэш аттрибутов
            QHash<QString, QString> attributesXml;
            attributesXml["name"] = name;
            attributesXml["type"] = type;
            attributesXml["mask"] = mask;
            attributesXml["bin_mask"] = bin_mask;
            attributesXml["id"] = id;
            attributesXml["description"] = description;
            attributesXml["number"] = number;
            attributesXml["external"] = external;
            attributesXml["priority"] = priority;
            attributesXml["status_write"] = status_write;
            attributesXml["status_read"] = status_read;
            attributesXml["extra_read"] = extra_read;
            attributesXml["extra_write"] = extra_write;
            attributesXml["shiftType"] = QString::number(shiftType);
            attributesXml["grp_number"] = grp_number;

            /// Вставка новой строки
            bool isVarFB = attributesXml["id"].contains(_parser.getClientName().toUpper());
            if(!isVarFB)
                createTag(tagGroup, tagGroup->childCount(), attributesXml);
        }
        // Добавить список созданных строк в дерево
        //treeWidget->topLevelItem(currentRegGroup)->addChildren(tags);

        // Раскрыть группу с добавленными строками
        //treeWidget->topLevelItem(currentRegGroup)->setExpanded(true);

        /// Пересчет всех № начиная с начала
        if(tagGroup->childCount() > 0)
            calculateAdresses(tagGroup->child(0));

        // Оповестить верхний класс о новой текущей строке
        //emit currentRowInfo(currentRegGroup, currentRow);
    }

    /// Добаление тегов статистики
    QDomDocument fileStats = _parser.getFileInfo(":/templates/stats.xml");
    QDomNode tagGroup = fileStats.firstChild();
    QDomElement tag = tagGroup.firstChildElement();

    while(!tag.isNull())
    {
        MainTreeItem *internalGroup = _rootItem->child("internal", Col::num("Регистры"));
        if(internalGroup != NULL)
        {
            MainTreeItem *item = internalGroup->child(tag.attribute("id"), Col::num("Идентификатор привязанной переменной"));

            if(item == NULL)
            {
                QHash<QString, QString> attributes;

                attributes["name"]          = tag.attribute("name");
                attributes["type"]          = Type::fullName(tag.attribute("type"));
                attributes["bin_mask"]      = "";
                attributes["id"]            = tag.attribute("id");
                attributes["description"]   = "";
                attributes["number"]        = "";
                attributes["shiftType"]     = QString::number(TAG_NONE);
                attributes["external"]      = "config.resource1.P." + tag.attribute("external");

                createTag(internalGroup, internalGroup->childCount(), attributes);
            }
        }
        tag = tag.nextSibling().toElement();
    }

        /// Добавление строк переменных функционального блока
        // Получение информации из файла
        QDomDocument document = _parser.getFileInfo();

        // Нахождение нужной группы для извлечения информации о переменных
        QDomNode outputVars = document.firstChild().firstChild().firstChild().nextSibling();

        QDomNode var = outputVars.firstChild();

        //TODO HARDCODE config.resource1.P.
        // Надо будет обеспечить получение данной информации
        // Формирование тегов переменных
        while(!var.isNull())
        {
            QHash<QString, QString> attributes;
            attributes["name"]          = var.toElement().attribute("documentation");
            attributes["type"]          = var.firstChild().firstChildElement().tagName();
            attributes["bin_mask"]      = "";
            attributes["id"]            = var.toElement().attribute("name");
            attributes["description"]   = "";
            attributes["number"]        = "";
            attributes["shiftType"]     = QString::number(TAG_NONE);
            //attributes["external"]      = "CONFIG.RESOURCE1.P" + _parser.getClientName().toUpper() + "_" + var.toElement().attribute("name");
            attributes["external"]      = "config.resource1.P." + _parser.getClientName() + "_" + var.toElement().attribute("name");

            MainTreeItem *internalGroup = _rootItem->child("internal", Col::num("Регистры"));
                if(internalGroup != NULL)
                {
                    MainTreeItem *tag = internalGroup->child(attributes["id"], Col::num("Идентификатор привязанной переменной"));
                    if(tag == NULL)
                    {
                        createTag(internalGroup, internalGroup->childCount(), attributes, PROP_UNVISIBLE);
                    }
                    else
                    {
                        tag->setIsVisible(PROP_UNVISIBLE);
                    }
                }
            var = var.nextSibling();
        }
}

/*
 * @brief Функция создает новый элемент
 */
MainTreeItem* MainTreeModel::createTag(MainTreeItem *tagGroup, int position, QHash<QString, QString> attributes, IsVisible isVisible)
{
    if(tagGroup == NULL)
        return NULL;

    int columnID    = Col::num("Идентификатор привязанной переменной");
    int columnBind  = Col::num("Привязка по значению");

    QModelIndex parent = createIndex(tagGroup->childNumber(), 0, tagGroup);
    beginInsertRows(parent, position, position);

    // Добавить новый тег
    tagGroup->insertChildren(position, 1, tagGroup->columnCount());
    MainTreeItem *tag = tagGroup->child(position);

    tag->setIsVisible(isVisible);

    // Заполнить тэг данными расположив данные в соответствующих столбцах
    QHashIterator<QString, QString> attr(attributes);
    while (attr.hasNext())
    {
        attr.next();

        // Если данный атрибут используется в таблице
        if(Col::attributesXml().contains(attr.key()))
        {
            int     column  = Col::attributesXml().value(attr.key());
            QString value   = attr.value();
            tag->setData(column, value);
        }
    }

    // Проверить привязку
    if((_variablesProvider->variablesMap().value(tag->data(columnID).toString()) == NULL) &&
        (_variablesProvider->variablesMap().value(tag->data(columnBind).toString()) == NULL) &&
        !_globalVars.contains(tag->data(columnBind).toString()) &&
        !_globalVars.contains(tag->data(columnID).toString()))
        tag->setColor(columnID, QBrush(QColor("lightsalmon")));
    else
        tag->setColor(columnID, QBrush());

    TypeOfShift shiftType = (TypeOfShift)attributes.value("shiftType").toInt();
    switch (shiftType)
    {
    case TAG_OFFSET:
        tag->setProperty(TAG_OFFSET, attributes.value("number").toInt());
        tag->setColor(Col::num("Регистры"), QBrush(QColor("lightskyblue")));
        //tag->setColor(QBrush(QColor("lightskyblue")));
        break;

    case TAG_SHIFT:
        tag->setProperty(TAG_SHIFT, attributes.value("number").toInt());
        tag->setColor(Col::num("Регистры"), QBrush(QColor("lightsteelblue")));
        //tag->setColor(QBrush(QColor("lightsteelblue")));
        break;

    case TAG_NONE:
        tag->setProperty(TAG_NONE, 0);
        break;

    default:
        break;
    }

    endInsertRows();

    return tag;
}

/*
 * @brief Рекурсивная функция для пересчета № Регистров у строк, после данной.
 * При обнаружение строки со свойством offset, вызывает себя.
 * Сама испускает сигнал об изменение данных
 */
void MainTreeModel::calculateAdresses(MainTreeItem *tag)
{
    // Подразумеваем что заходим сюда с тегом, который уже входит в группу тегов

    if(tag->childNumber() == 0)
    {
        int column = Col::num("Регистры");
        tag->setData(column, 0);
    }

    MainTreeItem    *regGroup = tag->parent();

    // Проинициализировать нужные переменные
    int                 index = tag->childNumber();
    int                lenght = regGroup->childCount() - 1;  /* - 1 чтобы не выйти за пределы, т.к. работаем и со следующей строкой */
    int             regColumn = Col::num("Регистры");

    // Пройтись по строкам, начиная с данной
    for(int currentItem = index; currentItem < lenght; ++currentItem)
    {
        int          nextItem = currentItem + 1;
        TypeOfShift shiftType = TypeOfShift(regGroup->child(nextItem)->getProperty().first);

        //Назначить следующей строке значение в колонке "№ Регистра"
        switch (shiftType)
        {
        case TAG_OFFSET:
            calculateAdresses(regGroup->child(nextItem));
            return;
            break;

        case TAG_SHIFT:
        {
            int regNumber     = regGroup->child(currentItem)->data(regColumn).toInt();
            int shift         = regGroup->child(nextItem)->getProperty().second;
            int regNumberNext = regNumber + shift;

            regGroup->child(nextItem)->setData(regColumn, QString::number(regNumberNext));
            break;
        }

        case TAG_NONE:
        {
            QString regTypeUI = regGroup->child(currentItem)->data(Col::num("Тип")).toString();
            int       regType = Type::bytes(regTypeUI);
            int     regNumber = regGroup->child(currentItem)->data(regColumn).toInt();
            int regNumberNext = regType + regNumber;

            regGroup->child(nextItem)->setData(regColumn, QString::number(regNumberNext));
            break;
        }

        default:
            break;
        }
    }

    dataChanged(createIndex(tag->childNumber()          , 0, regGroup),
                createIndex(regGroup->childCount() - 1  , 0, regGroup));
}

/*
 * @brief Функция для создания уникального ID
 */
QString MainTreeModel::makeUniqueID(QString id)
{
    // Есть еще uniqueListId

    // Инициализация значений для будущего суффикса
    static int numberSuffix = 0;
    static int   fieldWidth = 3;

    // Задать большую ширину суффикса при
    // его определенном значение
    if(numberSuffix >= 1000)
        fieldWidth = 4;

    // Получить все используемые Id
    QList<QString> listID = getListID();
    //QList<QString> listID;

    // Проверить - "ID пустой?"
    if(id == "")
    {
        // Прицепляем такой суффикс пока новый ID не станет уникальным
        ++numberSuffix;
        id = "id";
        QString temp = QString("%1").arg(numberSuffix, fieldWidth, 10, QChar('0'));

        while(listID.contains(id + temp))
        {
            ++numberSuffix;
            temp = QString("%1").arg(numberSuffix, fieldWidth, 10, QChar('0'));
        }
        id += temp;

        qDebug() << "~:red"
                 << "В списке тегов обнаружен тег с пустым идентификатором. "
                 << "Идентификатор данного тега заменен на " << id;

        // Проверить - "Уже есть такой ID"?
    } else if(listID.contains(id)) {

        QString oldID = id;
        ++numberSuffix;
        QString temp = QString("%1").arg(numberSuffix, fieldWidth, 10, QChar('0'));
        // Прицепляем такой суффикс пока новый ID не станет уникальным
        while(listID.contains(id + temp))
        {
            ++numberSuffix;
            temp = QString("%1").arg(numberSuffix, fieldWidth, 10, QChar('0'));
        }
        id += temp;

        qDebug() << "~:red"
                 << "В списке тегов обнаружен тег с повторяющимся идентификатором: "
                 << oldID
                 << ". Идентификатор данного тега заменен на "
                 << id;
    }

    return id;
}

/*
 * @brief Функция для изменения полей идентификаторов у выделенных строк
 */
void MainTreeModel::modifyTags(QModelIndexList indexes, QString text, ValueAction action)
{
    QList<QString> oldIDs;
    //QHash<MainTreeItem *, QString> selectedItemsOnlyTags;
    QHash<MainTreeItem *, QString> tags;

    // Получить выделенные пользователем тега
    for(int i = 0; i < indexes.count(); ++i)
    {
        if(indexes.at(i).parent() != QModelIndex())
        {
            MainTreeItem *tag   = getItem(indexes.at(i));
            QString id          = tag->data(Col::num("Идентификатор привязанной переменной")).toString();
            tags.insert(tag, id);

            oldIDs.append(id);
        }
    }

    /* Проверить не приведет ли изменение одного из идентификаторов
       к появлению недопустимого значения.
       Попутно будем вставлять в соответствие "строка - id"
       новый измененный id.*/
    QMutableHashIterator<MainTreeItem *, QString> i(tags);
    while (i.hasNext())
    {
        i.next();
        QString idToCheck = i.value(); //<< ": " << i.value() << endl;

        switch(action)
        {
        case VALUE_PREPEND:
            idToCheck.prepend(text);
            i.setValue(idToCheck);
            break;

        case VALUE_APPEND:
            idToCheck.append(text);
            i.setValue(idToCheck);
            break;

        case VALUE_DEL_START:
            if(idToCheck.startsWith(text))
            {
                idToCheck.remove(0, text.length());
                i.setValue(idToCheck);
            }
            break;

        case VALUE_DEL_END:
            if(idToCheck.endsWith(text))
            {
                //ToDo при превышение размера удаляемого куска
                idToCheck.remove(idToCheck.size() - text.length(), text.length());
                i.setValue(idToCheck);
            }
            break;

        default:
            break;
        }

        // Получить все используемые Id
        QList<QString> listID = getListID();

        if(listID.contains(idToCheck))
        {
            emit error("id used");
            return;
        }

        if(idToCheck == "")
        {
            emit error("id empty");
            return;
        }
    }

    // Установить каждой строке свой новый id
    QHashIterator<MainTreeItem *, QString> iterator(tags);
    while (iterator.hasNext())
    {
        iterator.next();
        QString modifiedID = iterator.value();
        iterator.key()->setData(Col::num("Идентификатор привязанной переменной"), modifiedID);
    }
}
