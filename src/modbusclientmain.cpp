#include "modbusclientmain.h"

#include <QAction>
#include <QIcon>
#include <QDebug>
#include <QFile>

ModBusClientMain::ModBusClientMain (QObject *parent) :
  QObject (parent)
{

}

ModBusClientMain::~ModBusClientMain ( )
{

}

void ModBusClientMain::init()
{
    return;
}

void
ModBusClientMain::init (cPropertiesProvider *props)
{
    Q_UNUSED(props);
}

QString
ModBusClientMain::name ( )
{
    return  ("Модуль ModBus-клиента");
}

QString
ModBusClientMain::description ( )
{
    return  ("Модуль позволяет редактировать теги для ModBus");
}

QIcon
ModBusClientMain::icon ( )
{
    return QIcon (":/icons/icon.png");
}

QWidget *ModBusClientMain::widget(QDomNode /*root*/, QVariant /*props*/)
{
    return NULL;
}

QWidget *
ModBusClientMain::widget (QDomNode root,
                          QVariant props,
                          cVariablesProvider *variablesProvider,
                          cElementProvider *elementsProvider)
{
    //qDebug() << Q_FUNC_INFO;
    Q_UNUSED(props);
    //Q_UNUSED(variablesProvider);
    Q_UNUSED(elementsProvider);
    //qDebug()<<variablesProvider;
    //qDebug()<<variablesProvider->variables();
    //qDebug()<<variablesProvider->variable("var");

  return new ModBusClient(root, variablesProvider);
}

QList<QAction *>
ModBusClientMain::action ( )
{
  return QList<QAction *>();
}

QList<QAction *>
ModBusClientMain::contextMenuAction()
{

    return QList<QAction *>();
}

QStringList ModBusClientMain::fileType()
{
    return QStringList("ModBusClient");
}

bool ModBusClientMain::build(QDomNode root, QVariant props, cVariablesProvider *variablesProvider, cElementProvider *elementsProvider)
{
    Q_UNUSED(root);
    Q_UNUSED(props);
    Q_UNUSED(variablesProvider);
    Q_UNUSED(elementsProvider);
    return false;
}

QList<QDomNode> ModBusClientMain::blocks(QDomNode root, QVariant props) const
{
    Q_UNUSED(props);
    QList<QDomNode> lst;

    QDomDocument document = Parser(root).getFileInfo();

    lst.push_back(document.firstChild());
    return lst;

}

QList<QPair<QString, type_enum> > ModBusClientMain::variables(QDomNode root, QVariant props) const
{
    Q_UNUSED(props);

    QList<QPair<QString, type_enum> > vars;

    QDomDocument document = Parser(root).getFileInfo();

    // Нахождение группы "externalVars"
    QDomNode groupVars = document.firstChild().firstChild().firstChild();

    while(!groupVars.isNull())
    {
        if(groupVars.nodeName() == "externalVars")
            break;

        groupVars.nextSibling();
    }

    QDomNode var = groupVars.firstChild();

    while(!var.isNull())
    {

        QString varName = var.toElement().attribute("name");

        // Нахождение параметра "type"
        QDomNode param = var.firstChild();
        while(!param.isNull())
        {
            if(param.nodeName() == "type")
                break;

            param.nextSibling();
        }
        QString type = param.firstChild().toElement().tagName();

        /*
        // Нахождение параметра "initialValue"
        param = var.firstChild();
        while(param != NULL)
        {
            if(param.nodeName() == "initialValue")
                break;

            param.nextSibling();
        }
        QString simpleValue = param.firstChild().toElement().tagName();*/

        vars.push_back(QPair<QString, type_enum> (varName,Parser(root).strToType(type)));

        var = var.nextSibling();
    }

    return vars;
}

//#ifndef HAVE_QT5
//Q_EXPORT_PLUGIN2 (Sample, Sample)
//#endif
