#ifndef TYPE_H
#define TYPE_H

#include <QHash>
#include <QMap>

/*
@brief Класс предоставляет информацию об используемых типах тегах
*/

class Type
{
public:
    static int         bytes(QString typeName);
    static QString     shortName(QString typeName);
    static QStringList names();
    static QString     fullName(QString shortName);

private:
    Type();
    static QHash<QString, QString>              _shortTypes;    // "Тип для XML" <---> "Отображаемый тип"
    static QHash<QString, QPair<int, QString> > _typesInfo;     // "Отображаемый тип" <---> "кол-во байт" <---> "Тип для XML"

    static QHash<QString, QPair<int, QString> > initTypes();
    static QHash<QString, QString>              initShortTypes();
};

#endif // TYPE_H
