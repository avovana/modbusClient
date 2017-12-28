#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QString>
#include <QWidget>
#include <QtXml>

/// Интерфейс модуля-редактора
class ModuleInterface
{
public:
  virtual ~ModuleInterface ( ) { }
  /* Plugin initialization */
  virtual void init () = 0;
    /**
      *@brief Инициализация основного интерфейса модуля
      */

  /* Plugin name, description */
  virtual QString name ( ) = 0;
    /**
      *@brief Возвращает имя модуля
      */
  virtual QString description ( ) = 0;
    /**
      *@brief Возвращает описание модуля
      */
  /* Plugin icon and widget */
  virtual QIcon icon ( ) = 0;
    /**
      *@brief Возвращает иконку модуля
      */
  virtual QWidget * widget (QDomNode root,QVariant props) = 0;
    /**
      *@brief Возвращает виджет, обрабатывающий файл с указанными свойствами
      *@param[in] root Ветвь дерева проекта, содержащая описание, интерпретируемое данным редактором
      *@param[in] props Свойства, влияющие на отобрадение файла
      */
  virtual QList<QAction *> action ( ) = 0;
    /**
      *@brief Возвращает действия, помещаемые в меню
      */
  virtual QList<QAction *> contextMenuAction ( ) = 0;
    /**
        *@brief Возвращает действия, помещаемые в контекстное меню дерева проектов
      */
  virtual QStringList fileType() = 0;
    /**
      *@brief Возвращает регулярное выражение, по которому определяется, может ли файл обрабатывать данный тип файла
      */
};


Q_DECLARE_INTERFACE (ModuleInterface, "ModuleInterface")

#endif // MODULEINTERFACE_H
