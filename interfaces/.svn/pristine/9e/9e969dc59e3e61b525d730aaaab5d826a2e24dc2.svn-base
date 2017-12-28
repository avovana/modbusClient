#ifndef IElementsControllable_H
#define IElementsControllable_H

///@brief Интерфейс поддержки функций управления элементами
class IElementsControllable
{
public:
  virtual ~IElementsControllable ( ) { } 
  virtual void rename (QString newName) {Q_UNUSED(newName);} ///< Переименовать
  virtual void delete_element () {} ///< Удалить
  virtual void clone () {} ///< Клонировать
  virtual void selectAll () {} ///< Выделить всё
  /// @return Возвращает true в случае успешного выделения элемента
  virtual bool select (QString name) {Q_UNUSED(name);return false;} ///< Выделить элемент с заданным именем.
  
  /// @return Возвращает true в случае успешного выделения элементов  
  virtual bool select (QRegExp name_template) {Q_UNUSED(name_template);return false;} ///< Выделить элементы, имена которых совпадают с заданным шаблоном.


};

#endif // IElementsControllable_H

