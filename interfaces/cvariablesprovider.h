#ifndef CVARIABLESPROVIDER_H
#define CVARIABLESPROVIDER_H
#include "cElementData.h"

/// Интерфейс управления переменными блока
class cVariablesProvider : public QObject
{
    Q_OBJECT
public:
    virtual ~cVariablesProvider()
    {

    }
    virtual QList<cAbstractVariable*> variables() = 0; ///< Возвращает список переменных
    virtual QMap<QString,cAbstractVariable*> variablesMap() = 0; ///< Возвращает карту переменных
    virtual cAbstractVariable* variable(QString) = 0; ///< Возвращает переменную по имени или NULL в случае отсутствия
public slots:
    virtual cAbstractVariable* addVariable(QString name, type_enum type) = 0;///< Добавление новой переменной
    virtual cAbstractVariable* addVariable(QString name, QString type) = 0;///< Добавление новой переменной
    virtual void addVariable(cAbstractVariable *variable) = 0;///< Добавление новой переменной
    virtual void removeVariable(cAbstractVariable *variable) = 0;///< Удаление переменной
    virtual void updateVariable(cAbstractVariable *variable) = 0;///< Изменение переменной
signals:
    void variableAddedSignal(cAbstractVariable *variable);///< Сигнал о добавлении новой переменной
    void variableRemovedSignal(cAbstractVariable *variable);///< Сигнал об удалении переменной
    void variableUpdatedSignal(cAbstractVariable *variable);///< Сигнал об изменении переменной

    void selectVariable(cAbstractVariable* name);///< Сигнал о выборе переменной
    void variablesUpdated(unsigned long tick);///< Сигнал об изменении значений переменных
};

#endif // CVARIABLESPROVIDER_H
