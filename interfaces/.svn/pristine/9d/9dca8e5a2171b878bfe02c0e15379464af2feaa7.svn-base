#ifndef IAUTOBLOCKGENERATOR
#define IAUTOBLOCKGENERATOR
#include <QDomNode>
#include <QList>
#include <QVariant>

///@brief Интерфейс поддержки добавления автосгенерированных программных блоков
class IAutoBlockGenerator
{
public:

  virtual ~IAutoBlockGenerator ( ) { }

    /**
      *@brief Возвращает список нод, содержащих тело автосгенерированных блоков
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на отображение файла
      */

  virtual QList<QDomNode> blocks(QDomNode root,
                                 QVariant props) const = 0;
};

#endif // IAUTOBLOCKGENERATOR

