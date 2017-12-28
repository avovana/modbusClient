#ifndef CELEMENTPROVIDER_H
#define CELEMENTPROVIDER_H

#include <QObject>
#include "cElementData.h"

/// Интерфейс управления компонентами
class cElementProvider : public QObject
{
    Q_OBJECT
public:
    virtual ~cElementProvider() {}
    /**
    	@param name - имя компонента
    	@return Описание компонента из библиотеки. Если не найден - возвращается NULL
    */
    virtual cElementData *find(QString name) = 0;///< Поиск компонента по его имени

    virtual QStringList groups() = 0;///< Список групп компонентов
    /**
    	@param group - имя группы. 
    	@return Список компонента из библиотеки, принадлежащей данной группе. Если указана пустое имя группы, то будет возвращен список всех компонентов в библиотеке
    */

    virtual QList<cElementData *> elements(QString group = QString()) = 0;///< Список элементов в группе
signals:
    void elementAddedSignal(QString group, cElementData *element); ///< Сигнал излучается при добавлении нового комонента
public slots:
    /**
    	@param group - имя группы, в которую необходимо добавить компонент. Если группа с указанным именем отсутствует, то она будет создана.
    	@param element - компонент, который будет доавлен.
    */
    virtual void addElement(QString group, cElementData *element) = 0;///< Добавление компонента в группу
};

#endif // CELEMENTPROVIDER_H
