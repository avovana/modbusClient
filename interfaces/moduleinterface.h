#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QString>
#include <QWidget>
#include <QtXml>

/// Интерфейс модуля
class ModuleInterface
{
public:
  virtual ~ModuleInterface ( ) { }
      /**
      *@brief Инициализация основного интерфейса модуля
      */
  virtual void init () = 0;

    /**
      *@brief Возвращает имя модуля
      */
  virtual QString name ( ) = 0;
    /**
      *@brief Возвращает описание модуля
      */
  virtual QString description ( ) = 0;
    /**
      *@brief Возвращает иконку модуля
      */
  virtual QIcon icon ( ) = 0;
    /**
      *@brief Возвращает виджет, обрабатывающий файл с указанными свойствами
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на отображение файла
      */
  virtual QWidget * widget (QDomNode root,QVariant props) = 0;
    /**
      *@brief Возвращает действия, помещаемые в меню
      */
  virtual QList<QAction *> action ( ) = 0;
    /**
        *@brief Возвращает действия, помещаемые в контекстное меню дерева проектов
      */
  virtual QList<QAction *> contextMenuAction ( ) = 0;
    /**
      *@brief Возвращает список типов ветвей, обрабатываемых данным модулем
      */
  virtual QStringList fileType() = 0;
};


Q_DECLARE_INTERFACE (ModuleInterface, "ModuleInterface")

#endif // MODULEINTERFACE_H
