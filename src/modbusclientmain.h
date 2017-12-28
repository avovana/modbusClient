#ifndef MODBUSCLIENTMAIN_H
#define MODBUSCLIENTMAIN_H

#include "utilsinterface.h"
#include "version.h"
#include "modbusclient.h"
#include "sharedinfo/parser.h"
#include <QVariantMap>
#include <QtXml>
#include "IAutoBlockGenerator.h"
#include "IAutoGlobalVariablesGenerator.h"

class ModBusClientMain : public QObject,public Version, public UtilsInterface, public IAutoBlockGenerator, public IAutoGlobalVariablesGenerator
{
  Q_OBJECT
  Q_INTERFACES (UtilsInterface)
  Q_PLUGIN_METADATA(IID "UtilsInterface" FILE "ModuleInterface.json")

  Q_PROPERTY(QString version READ version)
    Q_PROPERTY(QString build READ build_stamp)

public:
  ModBusClientMain (QObject *parent = 0);
  ~ModBusClientMain ( );
  virtual void init ();
  virtual void init (cPropertiesProvider *props);
  virtual QString name ( );
  virtual QString description ( );
  virtual QIcon icon ( );
  virtual QWidget * widget (QDomNode root,
                            QVariant props);
  virtual QWidget * widget (QDomNode root,
                            QVariant props,
                            cVariablesProvider* variablesProvider,
                            cElementProvider* elementsProvider);
  virtual QList<QAction *> action ( );
  virtual QList<QAction *> contextMenuAction ( );
   virtual QStringList fileType();
  virtual bool build (QDomNode root,
                      QVariant props,
                      cVariablesProvider* variablesProvider,
                      cElementProvider* elementsProvider);

  virtual QList<QDomNode> blocks(QDomNode root,
                                 QVariant props) const;
  virtual QList<QPair<QString, type_enum> > variables(QDomNode root,
                                 QVariant props) const;
};

#endif // MODBUSCLIENTMAIN_H
