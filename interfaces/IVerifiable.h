#ifndef IVerifiable_H
#define IVerifiable_H
#include "cobject.h"

///@brief Интерфейс поддержки проверки корректности программного блока
class IVerifiable
{
public:
  virtual ~IVerifiable ( ) { } 
  /// @return Возвращает true в случае успешного прохождения проверки
  virtual bool validate (cObject *body) {Q_UNUSED(body);return true;} ///< Проверить программный блок на корректность.
};

#endif // IVerifiable_H

