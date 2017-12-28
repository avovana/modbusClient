#include "parser.h"


QHash <QString, type_enum> AvaliableTypes =
{
        std::pair<QString, type_enum> ("BOOL",st_BOOL),
        std::pair<QString, type_enum> ("BYTE",st_BYTE),
        std::pair<QString, type_enum> ("WORD",st_WORD),
        std::pair<QString, type_enum> ("DWORD",st_DWORD),
        std::pair<QString, type_enum> ("LWORD",st_LWORD),
        std::pair<QString, type_enum> ("SINT",st_SINT),
        std::pair<QString, type_enum> ("INT",st_INT),
        std::pair<QString, type_enum> ("DINT",st_DINT),
        std::pair<QString, type_enum> ("LINT",st_LINT),
        std::pair<QString, type_enum> ("USINT",st_USINT),
        std::pair<QString, type_enum> ("UINT",st_UINT),
        std::pair<QString, type_enum> ("UDINT",st_UDINT),
        std::pair<QString, type_enum> ("ULINT",st_ULINT),
        std::pair<QString, type_enum> ("REAL",st_REAL),
        std::pair<QString, type_enum> ("LREAL",st_LREAL),
        std::pair<QString, type_enum> ("TIME",st_TIME),
        std::pair<QString, type_enum> ("DATE",st_DATE),
        std::pair<QString, type_enum> ("DT",st_DT),
        std::pair<QString, type_enum> ("TOD",st_TOD),
        std::pair<QString, type_enum> ("STRING",st_STRING),
        std::pair<QString, type_enum> ("WSTRING",st_WSTRING),
        std::pair<QString, type_enum> ("ARRAY",st_ARRAY),
        std::pair<QString, type_enum> ("DERIVED",st_DERIVED),
        std::pair<QString, type_enum> ("ENUM",st_ENUM),
        std::pair<QString, type_enum> ("SUBRANGESIGNED",st_SUBRANGESIGNED),
        std::pair<QString, type_enum> ("SUBRANGEUNSIGNED",st_SUBRANGEUNSIGNED),
        std::pair<QString, type_enum> ("STRUCT",st_STRUCT),
        std::pair<QString, type_enum> ("ANY",st_ANY),
        std::pair<QString, type_enum> ("ANY_DERIVED",st_ANY_DERIVED),
        std::pair<QString, type_enum> ("ANY_ELEMENTARY",st_ANY_ELEMENTARY),
        std::pair<QString, type_enum> ("ANY_MAGNITUDE",st_ANY_MAGNITUDE),
        std::pair<QString, type_enum> ("ANY_NUM",st_ANY_NUM),
        std::pair<QString, type_enum> ("ANY_REAL",st_ANY_REAL),
        std::pair<QString, type_enum> ("ANY_INT",st_ANY_INT),
        std::pair<QString, type_enum> ("ANY_BIT",st_ANY_BIT),
        std::pair<QString, type_enum> ("ANY_STRING",st_ANY_STRING),
        std::pair<QString, type_enum> ("ANY_DATE",st_ANY_DATE),
        std::pair<QString, type_enum> ("POINTER",st_POINTER),
        std::pair<QString, type_enum> ("TEXT",st_TEXT),
        std::pair<QString, type_enum> ("HTML",st_HTML)
};

Parser::Parser(QDomNode root)
    :_root(root)
{

}

QDomDocument Parser::getFileInfo(QString fileName)
{
    // Получение информации из файла
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "Ошибка! Файл с описанием переменных функционального блока клиента ModBus не открыт!";

    QString rez = file.readAll();

    rez = rez.arg(getClientName());
    QDomDocument document;
    document.setContent(rez);
    return document;
}

QString Parser::getClientName()
{
    QString clientName = _root.toElement().attribute("name");
    if (clientName[0].isDigit())
        clientName.prepend("_");
    clientName.replace(QRegExp("\\W"),"_");

    return clientName;
}

// Отображение сгенерированного файла
void Parser::showFile(QDomDocument document)
{
    QDomDocument document1("New doc1");
    document1.appendChild(document.firstChild());

    QFile file2("testForWriteXml2.xml");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Ошибка! Файл с описанием переменных функционального блока клиента ModBus не открыт!";
    }

    QTextStream out2(&file2);
    document1.save(out2, 4);
}

// warning Массивы пока не работают
type_enum Parser::strToType(QString value) const
{
    //if (value.startsWith("_ARRAY_of"))
    //    return strToType(nameToArraySize(value));
    return AvaliableTypes.value(value,st_ANY);
}

QList<QString> Parser::getGlobalVars()
{
    QList<QString> vars;

    QDomDocument document = getFileInfo();
    QDomElement groupTags = document.firstChildElement();

    while(groupTags.firstChildElement("externalVars").isNull())
    {
        if(!groupTags.childNodes().isEmpty())
            groupTags = groupTags.firstChildElement();
        else
            break;
    }
    groupTags = groupTags.firstChildElement();

    QDomElement var = groupTags.firstChildElement();

    while(!var.isNull())
    {
        QString varName = var.attribute("name");
        vars.push_back("config.resource1.P." + varName);

        var = var.nextSiblingElement();
    }

    return vars;
}
