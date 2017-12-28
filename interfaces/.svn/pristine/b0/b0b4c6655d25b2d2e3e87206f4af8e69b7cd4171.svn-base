#ifndef COBJECT
#define COBJECT
#include <QString>
#include <QDomNode>
#include <QVariantMap>
#include <QAction>

/// Объект дерева проектов
class cObject
{
public:
    virtual ~cObject(){}
    virtual QString text() = 0;///< Название объекта
    virtual QDomNode node() = 0;///<  Нода, связанная с объектом
    virtual QString type() = 0;///<  Тип объекта
    virtual QVariantMap props() = 0;///< Свойства объекта
    virtual QList<cObject *> children() = 0;///< Список дочерних объектов
    virtual cObject* addChild(QString type, QString text, QVariantMap props, QDomNode node) = 0; ///< Добавление дочернего объекта
};

///@brief Интерфейс навигатора по программным блокам проекта
///@attention Интерфейс будет расширен
class cNavigator
{
public:
    virtual ~cNavigator(){}
    virtual cObject* findCurrentNode(QAction *action) = 0;///< Получить объект, связанный с текущим действием
    virtual cObject* findCurrentNode(QDomNode node) = 0;///< Получить объект, связанный с нодой

};

#endif // COBJECT

