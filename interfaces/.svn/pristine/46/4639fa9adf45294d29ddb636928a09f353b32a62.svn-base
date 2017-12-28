#ifndef CPROPERTIESPROVIDER
#define CPROPERTIESPROVIDER
#include "cElementData.h"

/// Интерфейс управления свойствами
class cPropertiesProvider : public QObject
{
    Q_OBJECT
public:
    virtual ~cPropertiesProvider() {}
    virtual QList<cAbstractProperty*> properties(QString pouType=QString()) = 0;///< Возвращает список свойств
public slots:
    virtual void addProperty(QString pouType,cAbstractProperty *property) = 0;///< Добавление свойства
    virtual void removeProperty(QString pouType,cAbstractProperty *property) = 0;///< Удаление свойства
    virtual void updateProperty(QString pouType,cAbstractProperty *property) = 0;///< Обновление свойства
signals:
    void propertyAddedSignal(QString pouType,cAbstractProperty *property);///< Сигнал о добавлении свойства
    void propertyRemovedSignal(QString pouType,cAbstractProperty *property);///< Сигнал об удалении свойства
    void propertyUpdatedSignal(QString pouType,cAbstractProperty *property);///< Сигнал об обновлении свойства
};

#endif // CPROPERTIESPROVIDER

