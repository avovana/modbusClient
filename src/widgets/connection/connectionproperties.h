#ifndef CONNECTIONPROPERTIES_H
#define CONNECTIONPROPERTIES_H

#include <QWidget>
#include <QDomDocument>
#include <QDateTime>

namespace Ui {
class ConnectionProperties;
}

class ConnectionProperties : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionProperties(QDomNode root, QWidget *parent = 0);
    ~ConnectionProperties();
    void                        save();

signals:
    void                        modified();

private slots:
    void                        module_cmbox_currentTextChanged(const QString &module);
    void                        on_choosePage1_rb_toggled(bool checked);
    void                        on_choosePage2_rb_toggled(bool checked);

private:
    void                        changeCmboxItems(const QString &module);
    void                        getProps();
    void                        setProps(QHash<QString, QString> connectionProps);

    QHash<QString, QString>     readXml(QDomElement baseElement);
    void                        writeXml(QHash<QString, QString> connectionProps, QDomElement baseNode); //можно и в private;

private:
    Ui::ConnectionProperties    *ui;
    QDomNode                    _root;
    qlonglong                   _base_version;
    QHash<QString, QString>     _connectionProps;   // Содержит названия атрибутов и их значения
    bool                        _isModified;        // Флаг наличия не сохраненных изменений
};

#endif // CONNECTIONPROPERTIES_H
