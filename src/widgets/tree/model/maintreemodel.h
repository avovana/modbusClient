#ifndef MAINTREEMODEL_H
#define MAINTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QPair>

#include "cvariablesprovider.h"

#include "sharedinfo/type.h"
#include "sharedinfo/col.h"
#include "sharedinfo/enums.h"
#include "sharedinfo/parser.h"
#include "src/widgets/tree/model/maintreeitem.h"

class MainTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MainTreeModel(QDomNode root,
                  cVariablesProvider *variablesProvider,
                  QObject *parent = 0);

    ~MainTreeModel();

    QVariant        data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex     parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int             rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags   flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool            setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool            setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    bool            insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool            removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool            removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    //bool          insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    QModelIndex     insertTag(PositionToInsert insertType, const QModelIndex &index, QHash<QString, QString> attributes);

    void            modifyTags(QModelIndexList indexes, QString text, ValueAction action);

    QHash<QString, QString> getTagInfo(QModelIndex index);
    void            save();
    void            save(QIODevice *device);
    bool            open(QIODevice *device);
    QList<QString>  getListID() const;
    QList<QString>  getBindedVars();

signals:
    void            error(QString);

private:
    MainTreeItem    *getItem(const QModelIndex &index) const;

private:
    QDomNode                                _root;
    cVariablesProvider                      *_variablesProvider; // для доступа к переменным ПЛК Дизайнера
    Parser                                  _parser;
    QList<QString>                          _globalVars;
    MainTreeItem                            *_rootItem;
    void                                    readXml(QDomElement baseElement);
    QString                                 makeUniqueID(QString id);
    void                                    calculateAdresses(MainTreeItem *tag);
    MainTreeItem*                           createTag(MainTreeItem *tagGroup, int position, QHash<QString, QString> attributesXml, IsVisible prop = PROP_VISIBLE);
    QDomElement                             makeXmlTag(MainTreeItem *parent, int row);
    QDomElement                             makeXml();
};

#endif // MAINTREEMODEL_H
