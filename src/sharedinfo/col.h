#ifndef COL_H
#define COL_H
#include <QList>
#include <QPair>
#include <QHash>

/*
@brief Класс предоставляет информацию об используемых столбцах
*/

class Col
{
public:
    Col();
    ~Col();
    static int                  num(QString headerName);
    static QHash<QString, int>  attributesXml();
    static QString              attr(int colNum);
    static QList<QString>       headers();
private:
    static QList<QPair<QString, QString> >  _headersPairs;          // "Название столбца"   ---> "Атрибут XML"
    static QList<QString>                   _headers;               // Названия столбцов
    static QHash<QString, int>              _attributesXml;         // "Атрибут XML"        ---> "№ столбца"
    static QHash<QString, int>              _columnsHeaders;        // "Название столбца"   ---> "№ столбца"
    static QHash<int, QString>              _columnsAttrs;          // "№ столбца"          ---> "Атрибут XML"
    static QList<QPair<QString, QString> >  initHeadersList();
    static QHash<QString, int>              initAtrributesXml();
    static QHash<int, QString>              initColumnsAttrs();
    static QList<QString>                   initHeaders();
    static QHash<QString, int>              initColumnsHeaders();
};


#endif // COL_H
