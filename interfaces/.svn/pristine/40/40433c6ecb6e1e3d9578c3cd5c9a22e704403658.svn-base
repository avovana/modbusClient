#ifndef UtilsInterface_
#define UtilsInterface_

#include "moduleinterface.h"
#include "cobject.h"
#include "cvariablesprovider.h"
#include "cElementProvider.h"
#include "cpropertiesprovider.h"

///@brief Интерфейс модуля утилит
///Модуль утилит
class UtilsInterface : public ModuleInterface
{
public:
  virtual ~UtilsInterface ( ) { }
      /**
        *@brief Инициализация основного интерфейса модуля
        *@param[in] props Объект класса cPropertiesProvider, позволяющий управлять глобальными настройками модуля
        */

  virtual void init (cPropertiesProvider *props) = 0;    

    /**
      *@brief Возвращает виджет, обрабатывающий файл с указанными свойствами
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на интерпретацию файла
      *@param[in] variablesProvider Указатель на объект класса cVariablesProvider, позволяющий управлять переменными проекта
      *@param[in] elementsProvider Указатель на объект класса cElementProvider, позволяющий производить поиск и получать информацию о доступных компонентах
     */
  virtual QWidget * widget (QDomNode root,QVariant props, cVariablesProvider* variablesProvider, cElementProvider* elementsProvider) {Q_UNUSED(root);Q_UNUSED(props);Q_UNUSED(variablesProvider);Q_UNUSED(elementsProvider); return NULL;}

  /**
      *@brief Вызывается при сборке проекта
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на интерпретацию файла
      *@param[in] variablesProvider Указатель на объект класса cVariablesProvider, позволяющий управлять переменными проекта
      *@param[in] elementsProvider Указатель на объект класса cElementProvider, позволяющий производить поиск и получать информацию о доступных компонентах
      *@return Возвращает true в случае успешной сборки
      */
  virtual bool build (QDomNode root,QVariant props, cVariablesProvider* variablesProvider, cElementProvider* elementsProvider) = 0;
};
Q_DECLARE_INTERFACE (UtilsInterface, "UtilsInterface")

#endif // UtilsInterface

