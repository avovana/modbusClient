#ifndef IAUTOGLOBALVARIABLEGENERATOR
#define IAUTOGLOBALVARIABLEGENERATOR
#include <QList>
#include "cElementData.h"

///@brief Интерфейс поддержки добавления автосгенерированных программных блоков
class IAutoGlobalVariablesGenerator
{
public:

  virtual ~IAutoGlobalVariablesGenerator ( ) { }

    /**
      *@brief Возвращает список переменных (пара имя-тип), которые должны быть созданы при добавлении блока
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на отображение файла
      */

  virtual QList<QPair<QString, type_enum> > variables(QDomNode root,
                                 QVariant props) const = 0;
};
#endif // IAUTOGLOBALVARIABLEGENERATOR

