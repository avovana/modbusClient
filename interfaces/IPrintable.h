#ifndef IPrintable_H
#define IPrintable_H
#include <QPrinter>
#include "cobject.h"
///@brief Интерфейс поддержки функций печати
class IPrintable
{
public:
  virtual ~IPrintable ( ) { }
  virtual void print (QPrinter*) {} ///< Печать
  virtual void print (cObject*, QPrinter*) {} ///< Печать

};

#endif // IPrintable_H

