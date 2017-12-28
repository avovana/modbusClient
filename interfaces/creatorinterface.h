#ifndef CREATORINTERFACE_H
#define CREATORINTERFACE_H

#include "moduleinterface.h"
#include "contextmenumanagerinterface.h"
#include "cobject.h"

/// Интерфейс модуля-мастера
class CreatorInterface : public ModuleInterface, public ContextMenuManagerInterface, public QObject
{
public:
  virtual ~CreatorInterface ( ) { }
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
     *@brief Возвращает название модуля
     *@param[in] root Нода, для которой модуль должен вернуть название
  */
  virtual QString name (cObject* root) = 0;
signals:
    void nodeIconChangedSignal(cObject* root,QIcon newIcon);///< Иконка ноды изменена

};


Q_DECLARE_INTERFACE (CreatorInterface, "CreatorInterface")

#endif // CREATORINTERFACE_H
