#ifndef cInstance_H
#define cInstance_H

#include <QObject>
#include "cElementData.h"

/// Интерфейс описания работающего инстанса
class cInstance : public QObject
{
    Q_OBJECT
public:
    virtual ~cInstance() {}
    virtual QString taskName() = 0; //< Имя текущей задачи
    virtual QString instanceName() = 0; //< Имя текущего инстанса
    virtual QString pouName() = 0; //< Имя программы, выполняемой в текущем инстансе
    virtual QHash<QString, cAbstractVariable*> variables() = 0; //< Карта переменных, актуальная для текущего инстанса
    virtual int interval() = 0;//Интервал обновления текущей задачи
};

#endif // cInstance_H
 
