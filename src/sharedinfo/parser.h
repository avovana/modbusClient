#ifndef PARSER_H
#define PARSER_H
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "utilsinterface.h"

class Parser
{
public:
    Parser(QDomNode root);
    QDomDocument getFileInfo(QString fileName = QString(":/templates/manager.xml"));

    QString getClientName();
    void showFile(QDomDocument document);
    type_enum strToType(QString value) const;
    QList<QString> getGlobalVars();
private:
    QDomNode _root;
};

#endif // PARSER_H
