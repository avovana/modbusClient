#ifndef EDITORINTERFACE
#define EDITORINTERFACE

#include "moduleinterface.h"
#include "cobject.h"
#include "cvariablesprovider.h"
#include "cElementProvider.h"
#include "cpropertiesprovider.h"

///@brief Интерфейс модуля-редактора
///Модуль-редактор, в отличие от интерфейса модуля, получает доступ к переменным, компонентам и свойствам программных блоков 
class EditorInterface : public ModuleInterface
{
public:
  virtual ~EditorInterface ( ) { }
      /**
        *@brief Инициализация основного интерфейса модуля
        *@param[in] props Объект класса cPropertiesProvider, позволяющий управлять глобальными настройками модуля
        */

  virtual void init (cPropertiesProvider *props) = 0;


    /**
      *@brief Возвращает виджет, обрабатывающий файл с указанными свойствами
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на отобрадение файла
      *@param[in] variablesProvider Объект класса cVariablesProvider, позволяющий управлять переменными блока
      *@param[in] elementsProvider Объект класса cElementProvider, позволяющий производить поиск и получать информацию о доступных компонентах
      */
  virtual QWidget * widget (QDomNode root,
                            QVariant props,
                            cVariablesProvider* variablesProvider,
                            cElementProvider *elementsProvider) = 0;

};
Q_DECLARE_INTERFACE (EditorInterface, "EditorInterface")

#endif // EDITORINTERFACE

