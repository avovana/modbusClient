#include "connectionproperties.h"
#include "ui_connectionproperties.h"
#include <QDebug>

ConnectionProperties::ConnectionProperties(QDomNode root, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionProperties),
    _root(root),
    _isModified(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    // Считать XML
    _connectionProps = readXml(root.firstChildElement());

    // Обновить интерфейс
    setProps(_connectionProps);

    // Установить отслеживание изменений
    connect(ui->module_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::module_cmbox_currentTextChanged);
    connect(ui->mode_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::getProps);
    connect(ui->ports_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::getProps);
    connect(ui->baud_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::getProps);
    connect(ui->parity_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::getProps);
    connect(ui->stopBits_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::getProps);
    connect(ui->dataBits_cmbox, &QComboBox::currentTextChanged, this, &ConnectionProperties::getProps);
    connect(ui->dtr_chckbox, &QCheckBox::stateChanged, this, &ConnectionProperties::getProps);
    connect(ui->rts_chckbox, &QCheckBox::stateChanged, this, &ConnectionProperties::getProps);
    connect(ui->adress_spbox, static_cast<void (QSpinBox::*)(const QString &)>(&QSpinBox::valueChanged), this, &ConnectionProperties::getProps);
    connect(ui->byteTimeout_spbox, static_cast<void (QSpinBox::*)(const QString &)>(&QSpinBox::valueChanged), this, &ConnectionProperties::getProps);
    connect(ui->packageTimeout_spbox, static_cast<void (QSpinBox::*)(const QString &)>(&QSpinBox::valueChanged), this, &ConnectionProperties::getProps);
    connect(ui->retry_spbox, static_cast<void (QSpinBox::*)(const QString &)>(&QSpinBox::valueChanged), this, &ConnectionProperties::getProps);
}

ConnectionProperties::~ConnectionProperties()
{
    delete ui;
}

/*
 * @brief Функция считывает информацию из XML
 * @desription XML ---> ДАННЫЕ
 */
QHash<QString, QString> ConnectionProperties::readXml(QDomElement baseElement)
{
    QHash<QString, QString> connectionProps;

    /// Получить тип модуля и номер порта
    QString portName = baseElement.attribute("portName");
    if(portName.contains("ttyVS"))
    {
        QString portNumber = portName.remove(QRegExp("\\D"));

        connectionProps.insert("portNumber", portNumber);
        connectionProps.insert("module", "CPU");

    }
    else if(portName.contains("ttyRS"))
    {
        QString portNumber = portName.remove(QRegExp("\\D"));
        int number = portNumber.toInt();
        int wholePart = number / 8;
        int smallPart = number - (8 * wholePart) + 1;

        connectionProps.insert("portNumber", QString::number(smallPart));
        connectionProps.insert("module", QString("RSSW в слоте %1").arg(wholePart));
    }
    else
    {
        connectionProps.insert("portNumber", "1");
        connectionProps.insert("module", "CPU");
    }

    /// Получить номер версии
    if(baseElement.hasAttribute("base_version"))
    {
        _base_version = baseElement.attribute("base_version").toInt();
    }
    else
    {
        _base_version = 0;
    }

    /// Получить основные параметры
    connectionProps.insert("adress", baseElement.attribute("adress", "1"));
    connectionProps.insert("packageTimeout", baseElement.attribute("packageTimeout", "1000"));
    connectionProps.insert("baudrate", baseElement.attribute("baudrate", "19200"));
    connectionProps.insert("mode", baseElement.attribute("mode", "RS-232"));
    connectionProps.insert("byteTimeout", baseElement.attribute("byteTimeout", "100"));
    connectionProps.insert("dataBits", baseElement.attribute("dataBits", "8"));
    connectionProps.insert("stopBits", baseElement.attribute("stopBits", "1"));
    connectionProps.insert("parity", baseElement.attribute("parity", "NONE"));
    connectionProps.insert("dtr", baseElement.attribute("dtr", "0"));
    connectionProps.insert("rts", baseElement.attribute("rts", "0"));

    return connectionProps;
}

/*
 * @brief Функция считывает значения с интерфейса
 * @desription ИНТЕРФЕЙС ---> ДАННЫЕ
 */
void ConnectionProperties::getProps()
{
    _isModified = true;
    emit modified();

    /// Обновить основные параметры
    _connectionProps.insert("module", ui->module_cmbox->currentText());
    _connectionProps.insert("mode", ui->mode_cmbox->currentText());
    _connectionProps.insert("portNumber", ui->ports_cmbox->currentText());
    _connectionProps.insert("baudrate", ui->baud_cmbox->currentText());
    _connectionProps.insert("parity", ui->parity_cmbox->currentText());
    _connectionProps.insert("stopBits", ui->stopBits_cmbox->currentText());
    _connectionProps.insert("dataBits", ui->dataBits_cmbox->currentText());
    _connectionProps.insert("adress", QString::number(ui->adress_spbox->value()));
    _connectionProps.insert("byteTimeout", ui->byteTimeout_spbox->text());
    _connectionProps.insert("packageTimeout", ui->packageTimeout_spbox->text());
    _connectionProps.insert("retry", ui->retry_spbox->text());
    _connectionProps.insert("dtr", QString::number(ui->dtr_chckbox->isChecked()));
    _connectionProps.insert("rts", QString::number(ui->rts_chckbox->isChecked()));

    /// Создать название порта
    // Получить нужные поля
    QString portName = _connectionProps.value("module");
    QString portNumber = _connectionProps.value("portNumber");

    // Сформировать имя порта
    if(portName.contains("RSSW"))
    {
        // Расчитать номер
        int number = portName.remove(QRegExp("\\D")).toInt();
        number = number * 8 + (portNumber.toInt() - 1);

        // Софрмировать имя
        portName.clear();
        portName.append("ttyRS");
        portName.append(QString::number(number));

    } else if(portName.contains("CPU"))
    {
        // Софрмировать имя
        portName.clear();
        portName.append("ttyVS");
        portName.append(portNumber);
    }

    _connectionProps.insert("portName", portName);
}

/*
 * @brief Функция обновляет XML
 * @desription ДАННЫЕ ---> XML
 */
void ConnectionProperties::writeXml(QHash<QString, QString> connectionProps, QDomElement baseNode)
{
    // Удалить ненужную информацию
    connectionProps.remove("module");
    connectionProps.remove("portNumber");

    /// Запись атрибутов
    if(_root.firstChildElement().hasAttribute("base_version"))
        _base_version = _root.firstChildElement().attribute("base_version").toInt();
    else
        _base_version = 0;

    ++_base_version;
    baseNode.setAttribute("base_version", QString::number(_base_version));
    baseNode.setAttribute("version", "1");

    // Записать время сборки
    baseNode.setAttribute("date_time", QDateTime::currentDateTime().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString("hh:mm:ss"));

    /// Запись основных параметров
    QHashIterator<QString, QString> i(connectionProps);
    while (i.hasNext())
    {
        i.next();

        // Записать основные параметры
        if(!(i.value().isEmpty()))
            baseNode.setAttribute(i.key(), i.value());
    }
}

/*
 * @brief Функция обновляет интерфейс
 * полученными данными
 * @desription ДАННЫЕ ---> ИНТЕРФЕЙС
 */
void ConnectionProperties::setProps(QHash<QString, QString> connectionProps)
{
    ui->module_cmbox->setCurrentText(connectionProps.value("module"));
    changeCmboxItems(connectionProps.value("module"));
    ui->mode_cmbox->setCurrentText(connectionProps.value("mode"));
    ui->ports_cmbox->setCurrentText(connectionProps.value("portNumber"));
    ui->baud_cmbox->setCurrentText(connectionProps.value("baudrate"));

    ui->parity_cmbox->setCurrentText(connectionProps.value("parity"));
    ui->stopBits_cmbox->setCurrentText(connectionProps.value("stopBits"));
    ui->dataBits_cmbox->setCurrentText(connectionProps.value("dataBits"));
    ui->adress_spbox->setValue(connectionProps.value("adress").toInt());

    ui->byteTimeout_spbox->setValue(connectionProps.value("byteTimeout").toInt());
    ui->packageTimeout_spbox->setValue(connectionProps.value("packageTimeout").toInt());
    ui->retry_spbox->setValue(connectionProps.value("retry").toInt());

    ui->dtr_chckbox->setChecked(connectionProps.value("dtr").toInt());
    ui->rts_chckbox->setChecked(connectionProps.value("rts").toInt());
}

/*
 * @brief Обновить состояние виджета
 */
void ConnectionProperties::save()
{
    // Убедиться в наличии базовой ноды
    if(!_root.hasChildNodes())
    {
        QDomElement     baseNode;
        QDomDocument    domDocument;
        baseNode      = domDocument.createElement("base");
        _root.appendChild(baseNode);
    }

    // Обновить XML
    writeXml(_connectionProps, _root.firstChildElement());

    // Сбросить флаг
    _isModified = false;
}

/*
 * @brief Функция изменяет модифицирует содержимое Combobox'a
 * и некоторых частей интерфейса
 */
void ConnectionProperties::changeCmboxItems(const QString &module)
{
    ui->ports_cmbox->clear();

    int quantityOfAvaliblePorts = 1;
    QStringList ports;

    if(module == "CPU")
    {
        quantityOfAvaliblePorts = 4;
        ui->rts_chckbox->setVisible(true);
        ui->dtr_chckbox->setVisible(true);
        ui->label_35->setVisible(true);
    }

    if(module.contains("RSSW"))
    {
        quantityOfAvaliblePorts = 8;
        ui->rts_chckbox->setVisible(false);
        ui->dtr_chckbox->setVisible(false);
        ui->label_35->setVisible(false);
    }

    for(int i = 1; i <= quantityOfAvaliblePorts; ++i)
    {
        QString port = QString("%1").arg(QString::number(i));
        ports.append(port);
    }

    ui->ports_cmbox->addItems(ports);
}

void ConnectionProperties::module_cmbox_currentTextChanged(const QString &module)
{
    // Изменить интерфейс
    changeCmboxItems(module);

    // Считать значения с интерфейса
    getProps();
}

void ConnectionProperties::on_choosePage1_rb_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(0);
}

void ConnectionProperties::on_choosePage2_rb_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
}
