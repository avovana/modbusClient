#include "sharedinfo/col.h"

#warning Важен порядок инициализации статических членов
QList<QPair<QString, QString> > Col::_headersPairs      = initHeadersList();
QHash<QString, int>             Col::_columnsHeaders    = initColumnsHeaders();
QList<QString>                  Col::_headers           = initHeaders();
QHash<int, QString>             Col::_columnsAttrs      = initColumnsAttrs();
QHash<QString, int>             Col::_attributesXml     = initAtrributesXml();

QList<QPair<QString, QString> > Col::initHeadersList()
{
    QList<QPair<QString, QString> > headersPairs;

    #warning При изменение названий столбцов менять поиском по всему файлу!
    headersPairs.append(QPair<QString, QString>("Регистры", "number"));
    headersPairs.append(QPair<QString, QString>("Идентификатор привязанной переменной", "id"));
    headersPairs.append(QPair<QString, QString>("Наименование", "name"));
    headersPairs.append(QPair<QString, QString>("Тип", "type"));
    headersPairs.append(QPair<QString, QString>("Маска", "bin_mask"));
    headersPairs.append(QPair<QString, QString>("Описание", "description"));
    headersPairs.append(QPair<QString, QString>("Привязка по значению", "external"));
    headersPairs.append(QPair<QString, QString>("Приоритет", "priority"));
    headersPairs.append(QPair<QString, QString>("Статус записи", "status_write"));
    headersPairs.append(QPair<QString, QString>("Статус чтения", "status_read"));
    headersPairs.append(QPair<QString, QString>("Экстренное чтение", "extra_read"));
    headersPairs.append(QPair<QString, QString>("Экстренная запись", "extra_write"));
    headersPairs.append(QPair<QString, QString>("№ группы", "grp_number"));

    return headersPairs;
}

QHash<QString, int> Col::initColumnsHeaders()
{
    QHash<QString, int> columnsHeaders;

    for(int col = 0; col < _headersPairs.size(); ++col)
        columnsHeaders[_headersPairs.at(col).first] = col;

    return columnsHeaders;
}

QList<QString> Col::initHeaders()
{
    QList<QString> headers;

    for(int col = 0; col < _headersPairs.size(); ++col)
        headers << _headersPairs.at(col).first;

    return headers;
}

QHash<int, QString> Col::initColumnsAttrs()
{
    QHash<int, QString> columns;

    for(int col = 0; col < _headersPairs.size(); ++col)
        columns[col] = _headersPairs.at(col).second;

    return columns;
}

QHash<QString, int> Col::initAtrributesXml()
{
    QHash<QString, int> attributes;

    for(int col = 0; col < _headersPairs.size(); ++col)
        attributes[_headersPairs.at(col).second] = col;

    return attributes;
}

/* @brief Функция возвращает номер столбца по его имени наличие столбца
 * @param headerName - имя столбца
  */
int Col::num(QString headerName)
{
    return _columnsHeaders.value(headerName);
}

/* @brief Функция возвращает список названий столбцов
*/
QList<QString> Col::headers()
{
    return _headers;
}

/* @brief Функция возвращает название атрибута тега XML
 * @param colNum - номер столбца
 */
QString Col::attr(int colNum)
{
    return _columnsAttrs.value(colNum);
}

/* @brief Функция возвращает связки
 * "название атрибута тега XML" - "номер столбца"
 */
QHash<QString, int> Col::attributesXml()
{
    return _attributesXml; // м.б. вернуть по const & ?
}
