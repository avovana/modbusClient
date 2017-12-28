#ifndef OBJECTINTERFACE_H
#define OBJECTINTERFACE_H

#include "moduleinterface.h"
#include "cobject.h"

/// Интерфейс модуля-мастера
class ObjectInterface : public ModuleInterface
{
public:
  virtual ~ObjectInterface ( ) { }
  /* Plugin initialization */
    /**
      *@brief Инициализация основного интерфейса модуля
      *@param[in] navigator объект, позволяющий управлять деревом проектов
      *@param[in] avaliableModules список доступных модулей
      */
  virtual void init (cNavigator* navigator,QList<ModuleInterface*> avaliableModules) = 0;

  /**
     *@brief Возвращает иконку модуля
     *@param[in] root Нода, для которой модуль должен вернуть иконку
  */
  virtual QIcon icon (cObject* root) = 0;

  /**
      *@brief Возвращает виджет, обрабатывающий файл с указанными свойствами
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на отобрадение файла
      */
  virtual QWidget * widget (cObject* root,QVariant props) = 0;  
signals:
  void nodeIconChangedSignal(cObject* root,QIcon newIcon);///< Иконка ноды изменена

};


Q_DECLARE_INTERFACE (ObjectInterface, "ObjectInterface")

#endif // OBJECTINTERFACE_H
