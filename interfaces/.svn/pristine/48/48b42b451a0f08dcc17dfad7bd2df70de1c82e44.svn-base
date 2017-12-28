#ifndef ITAGBASE
#define ITAGBASE
#include <QObject>
#include <QDateTime>
#include <QContiguousCache>
#include <QVariant>

class  ITagValue
{
public:
    virtual ~ITagValue(){}

    virtual QDateTime timestamp() const = 0;
    virtual operator double() = 0;
    virtual operator float() = 0;
    virtual operator QString() = 0;
    virtual operator uint64_t() = 0;
    virtual operator int64_t() = 0;
    virtual operator uint32_t() = 0;
    virtual operator int32_t() = 0;
    virtual operator uint16_t() = 0;
    virtual operator int16_t() = 0;
    virtual operator uint8_t() = 0;
    virtual operator int8_t() = 0;
    virtual operator QVariant() = 0;
    virtual ITagValue *operator[](const long &tick) const = 0;


};

class  IEditableTagValue : public QObject, public ITagValue
{
    Q_OBJECT
public:
    IEditableTagValue(QObject* parent = NULL) : QObject(parent){}
    virtual ~IEditableTagValue(){}
    virtual IEditableTagValue* operator = (QVariant newValue) = 0;

signals:
    void readyWrite(bool result);
};

class ITagBase : public QObject
{
    Q_OBJECT

public:
    ITagBase(QObject* parent = NULL) : QObject(parent){}
    virtual ~ITagBase(){}

    virtual QString tagName (const QString &id) const = 0;//! Возвращает имя тега по его идентификатору
    virtual QString tagDescription (const QString &id) const= 0;//! Возвращает описание тега по его идентификатору
    virtual QString tagMeasureUnit (const QString &id) const= 0;//! Возвращает единицы измерения тега по его идентификатору
    virtual QString tagValue (const QString &id, int base = 10)const= 0;//! Возвращает значение тега по его идентификатору
    virtual QString tagAddr (const QString &id) const= 0;//! Возвращает адрес тега по его идентификатору
    virtual QString tagDataType (const QString &id) const= 0;//! Возвращает тип тега по его идентификатору

    virtual QStringList idList ( ) const= 0;//! Возвращает список тегов
    virtual bool contains (const QString &id) const= 0;//! Проверка нахождения тега в базе тегов @return возвращает true, если тег с данным идентификатором присутствует в базе
    virtual uint32_t baseSize ( ) const= 0;//! Возвращает размер базы в байтах
    virtual QString tagBaseFileName ( ) const= 0;//! Возвращает имя файла базы
    virtual IEditableTagValue& operator[] (QString)= 0;//! Осуществляет доступ к значению тега базы по его идентификатору

signals:
    void baseUpdated ( );//! Содержимое базы обновилось
};


#endif // ITAGBASE

