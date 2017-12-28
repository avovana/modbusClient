#ifndef IDebuggable_H
#define IDebuggable_H

#include "cobject.h"
#include "cvariablesprovider.h"
#include "cInstance.h"
#include "DeviceStateInterfaces/IRuntimeState.h"

///@brief Интерфейс поддержки отладки
class IDebuggable
{
public:

  virtual ~IDebuggable ( ) { } 

    /**
      *@brief Возвращает виджет с информацией о процессах, происходящих в инстансе
      *@param[in] instanceProvider содержит сведения об инстансе, глобальные переменные ресурса, интервал задачи 
      *@param[in] base База состояния контроллера
      *@param[in] root Корневая нода с исполняемой и отлаживаемой программой
      *@param[in] props Свойства, влияющие на отображение файла
      */

  virtual QWidget* debugWidget(cInstance *instanceProvider,
                               IRuntimeState *base,
                               QDomNode root,
                               QVariant props) = 0;
};

#endif // IDebuggable_H

