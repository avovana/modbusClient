#ifndef ISearchable_H
#define ISearchable_H
#include "cobject.h"

///@brief Интерфейс поддержки функций правки
class ISearchable
{
public:
  virtual ~ISearchable ( ) { } 
    /**
      *@brief Поиск элемента с заданным именем.
      *@return Возвращает true в случае успешного нахождения элемента
      *@param[in] pou_object объект, в пределах которого должен осуществляться поиск
      *@param[in] text текст, который должен быть найден
      *@param[in] caseSensitive опция "Учитывать регистр"
      *@param[in] whoolWordOnly опция "Слова полностью"
      */
  virtual bool find (cObject* pou_object, QString text, bool caseSensitive, bool whoolWordOnly) {Q_UNUSED(pou_object);Q_UNUSED(text);Q_UNUSED(caseSensitive);Q_UNUSED(whoolWordOnly);return false;}
    /**
      *@brief Поиск элемента, имя которого совпадает с заданным шаблоном.
      *@return Возвращает true в случае успешного нахождения элемента
      *@param[in] pou_object Объект, в пределах которого должен осуществляться поиск
      *@param[in] name_template шаблон для поиска
      *@param[in] caseSensitive опция "Учитывать регистр"
      *@param[in] whoolWordOnly опция "Слова полностью"
      */
  virtual bool find (cObject* pou_object, QRegExp name_template, bool caseSensitive, bool whoolWordOnly) {Q_UNUSED(pou_object);Q_UNUSED(name_template);Q_UNUSED(caseSensitive);Q_UNUSED(whoolWordOnly);return false;}
  /// @return Возвращает true в случае успешного нахождения элемента
  virtual bool findNext () {return false;} ///< Продолжить поиск
  /// @return Возвращает true в случае успешного переименования элемента  
  virtual bool replace (cObject* pou_object, QString old_name, QString new_name, bool caseSensitive, bool whoolWordOnly) {Q_UNUSED(pou_object);Q_UNUSED(old_name);Q_UNUSED(new_name);Q_UNUSED(caseSensitive);Q_UNUSED(whoolWordOnly);return false;} ///< Переименовать элемент с old_name на new_name.
  /// @return Возвращает true в случае успешного переименования элементов 
  virtual bool replace (cObject* pou_object, QRegExp old_name_template, QString new_name, bool caseSensitive, bool whoolWordOnly) {Q_UNUSED(pou_object);Q_UNUSED(old_name_template);Q_UNUSED(new_name);Q_UNUSED(caseSensitive);Q_UNUSED(whoolWordOnly);return false;} ///< Переименовать элементы, имена которых совпадают с заданным шаблоном old_name_template на new_name.
     
};

#endif // ISearchable_H

