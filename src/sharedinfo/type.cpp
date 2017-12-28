#include "sharedinfo/type.h"
#include <QDebug>

QHash<QString, QPair<int, QString> > Type::_typesInfo  = initTypes();
QHash<QString, QString>              Type::_shortTypes = initShortTypes();

QHash<QString, QPair<int, QString> > Type::initTypes()
{
    QHash<QString, QPair<int, QString> > types;

    types["WORD"]   = QPair<int, QString>(1, "ui16");
    types["DWORD"]  = QPair<int, QString>(2, "ui32");
    types["LWORD"]  = QPair<int, QString>(4, "ui64");
    types["INT"]    = QPair<int, QString>(1, "i16");
    types["DINT"]   = QPair<int, QString>(2, "ui32");
    types["LINT"]   = QPair<int, QString>(4, "i64");
    types["UINT"]   = QPair<int, QString>(1, "ui16");
    types["UDINT"]  = QPair<int, QString>(2, "ui32");
    types["ULINT"]  = QPair<int, QString>(4, "ui64");
    types["REAL"]   = QPair<int, QString>(2, "f");
    types["LREAL"]  = QPair<int, QString>(4, "d");
    types["BOOL"]  = QPair<int, QString>(1, "bool");

    return types;
}

QHash<QString, QString> Type::initShortTypes()
{
    static QHash<QString, QString> shortTypes;

    shortTypes["ui16"]  = "WORD";
    shortTypes["ui32"]  = "DWORD";
    shortTypes["ui64"]  = "LWORD";
    shortTypes["i16"]   = "INT";
    shortTypes["ui32"]  = "DINT";
    shortTypes["i64"]   = "LINT";
    shortTypes["ui16"]  = "UINT";
    shortTypes["ui32"]  = "UDINT";
    shortTypes["ui64"]  = "ULINT";
    shortTypes["f"]     = "REAL";
    shortTypes["d"]     = "LREAL";
    shortTypes["bool"]  = "BOOL";

    return shortTypes;
}

int Type::bytes(QString typeName)
{
    return _typesInfo.value(typeName).first;
}

/* @brief Функция возвращает полное название типа
 * @param shortName - краткое название
 */
QString Type::fullName(QString shortName)
{
    if(_shortTypes.contains(shortName))
        return _shortTypes.value(shortName);
    else
        return "error";
}

/* @brief Функция возвращает краткое название типа
 * @param typeName - полное название
 */
QString Type::shortName(QString typeName)
{
    return _typesInfo.value(typeName).second;
}

/* @brief Функция возвращает список полных названий типов
  */
QStringList Type::names()
{
    return _typesInfo.keys();
}
