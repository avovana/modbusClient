#include "modbusclient.h"
#include "ui_modbusclient.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QObject>
#include "src/widgets/tree/view/treeview.h"
#include "src/widgets/lineedit/lineedit.h"

ModBusClient::ModBusClient(QDomNode root, cVariablesProvider *variablesProvider, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModBusClient),
    _root(root),
    _isTreeModified(false),
    _isConnectionModified(false),
    _variablesProvider(variablesProvider)
{
    ui->setupUi(this);
    initUI();

    /// Создание основной модели
    mainTreeModel = new MainTreeModel(root, variablesProvider, this);
    ui->treeView->setModel(mainTreeModel);
    ui->treeView->setVariablesProvider(variablesProvider);
    connect(mainTreeModel, &MainTreeModel::error, this, &ModBusClient::errorShow);
    connect(mainTreeModel, &MainTreeModel::dataChanged, this, &ModBusClient::treeUpdated);

    /// Создание делегатов
    // Связать название столбца с делегатом
    delegate["Регистры"]                                = new DelegateSymbols(this, QRegExp("\\d*"));
    delegate["Идентификатор привязанной переменной"]    = new DelegateId(this, QRegExp("[A-Za-z_][\\w\\.]{0,253}"));
    delegate["Привязка по значению"]                    = new DelegateId(this, QRegExp("([A-Za-z_][\\w\\.]{0,253})|"));
    delegate["Наименование"]                            = new DelegateSymbols(this, QRegExp("[^<>\"\\\\/]*"));
    delegate["Тип"]                                     = new DelegateComboBox(this);
    delegate["Маска"]                                   = new DelegateMask(this);
    delegate["Приоритет"]                               = new DelegateSymbols(this, QRegExp("([0-9]|[1-8][0-9]|9[0-9]|[1-8][0-9]{2}|9[0-8][0-9]|99[0-9]|[1-8][0-9]{3}|9[0-8][0-9]{2}|99[0-8][0-9]|999[0-9]|[1-8][0-9]{4}|9[0-8][0-9]{3}|99[0-8][0-9]{2}|999[0-8][0-9]|9999[0-9]|[1-8][0-9]{5}|9[0-8][0-9]{4}|99[0-8][0-9]{3}|999[0-8][0-9]{2}|9999[0-8][0-9]|99999[0-9]|[1-8][0-9]{6}|9[0-8][0-9]{5}|99[0-8][0-9]{4}|999[0-8][0-9]{3}|9999[0-8][0-9]{2}|99999[0-8][0-9]|999999[0-9]|[1-8][0-9]{7}|9[0-8][0-9]{6}|99[0-8][0-9]{5}|999[0-8][0-9]{4}|9999[0-8][0-9]{3}|99999[0-8][0-9]{2}|999999[0-8][0-9]|9999999[0-9]|[1-8][0-9]{8}|9[0-8][0-9]{7}|99[0-8][0-9]{6}|999[0-8][0-9]{5}|9999[0-8][0-9]{4}|99999[0-8][0-9]{3}|999999[0-8][0-9]{2}|9999999[0-8][0-9]|99999999[0-9]|[1-3][0-9]{9}|4[01][0-9]{8}|42[0-8][0-9]{7}|429[0-3][0-9]{6}|4294[0-8][0-9]{5}|42949[0-5][0-9]{4}|429496[0-6][0-9]{3}|4294967[01][0-9]{2}|42949672[0-8][0-9]|429496729[0-5])"));
    delegate["Статус записи"]                           = new DelegateButton(this, QRegExp("([A-Za-z_][\\w\\.]{0,253})|"), _variablesProvider, "INT");
    delegate["Статус чтения"]                           = new DelegateButton(this, QRegExp("([A-Za-z_][\\w\\.]{0,253})|"), _variablesProvider, "INT");
    delegate["Экстренное чтение"]                       = new DelegateButton(this, QRegExp("([A-Za-z_][\\w\\.]{0,253})|"), _variablesProvider, "BOOL");
    delegate["Экстренная запись"]                       = new DelegateButton(this, QRegExp("([A-Za-z_][\\w\\.]{0,253})|"), _variablesProvider, "BOOL");
    delegate["№ группы"]                                = new DelegateSymbols(this, QRegExp("\\d*"));

    // Привязать делегат к столбцу.
    QHash<QString, QAbstractItemDelegate*>::const_iterator i = delegate.constBegin();
    while (i != delegate.constEnd())
    {
        QString header = i.key();
        if(Col::headers().contains(header))
            ui->treeView->setItemDelegateForColumn(Col::num(header), (delegate.value(i.key())));
        ++i;
    }

    /// Создание виджета для установки опций соединения
    connectionProps = new ConnectionProperties(root, this);
    ui->connect_layout->addWidget(connectionProps);
    connect(connectionProps, &ConnectionProperties::modified, this, &ModBusClient::modified);

    /// Первый запуск модуля
    if(!_root.hasChildNodes())
    {
        _isTreeModified = true;
        _isConnectionModified = true;
    }

    ui->treeView->expandAll();  // Раскрыть группу с добавленными строками
    ui->treeView->header()->setSectionResizeMode( QHeaderView::ResizeToContents); // Выровнять по ширине
}

ModBusClient::~ModBusClient()
{
    delete ui;
}

bool ModBusClient::isModified()
{
    return _isTreeModified || _isConnectionModified;
}

/* *
@brief Функция последовательно создаёт основные элементы UI
@param[in] void
@return void
@warning
*/
void ModBusClient::initUI(void)
{
    ui->bind_lnedit->setVariablesProvider(_variablesProvider); // Инициализировать поле "Привязки"

    ui->bind_lnedit             ->setValidator(new QRegExpValidator(QRegExp("[A-Za-z_][\\w\\.]{0,253}|")));
    ui->mask_lnedit             ->setValidator(new QRegExpValidator(QRegExp("[0-1]*")));
    ui->id_lnedit               ->setValidator(new QRegExpValidator(QRegExp("[A-Za-z_][\\w\\.]{0,253}")));
    ui->suffix_or_preffix_lnedit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z_][\\w\\.]{0,253}")));

    ui->type_cmbox->addItems(QStringList(Type::names()));

    connect(ui->saveFile_btn, SIGNAL(clicked(bool)), this, SLOT(saveAs()));
    connect(ui->type_cmbox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMask(QString)));
    connect(ui->openFile_btn, SIGNAL(clicked(bool)), this, SLOT(open()));
}

void ModBusClient::setMask(const QString &text)
{
    int quantityOfBools = 0;

    quantityOfBools = 16 * Type::bytes(text);

    QString mask;
    mask.fill(QChar('B'),quantityOfBools);
    mask.append(tr(";_"));
    ui->mask_lnedit->setInputMask(mask);
}

void ModBusClient::open(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                    QDir::currentPath(),
                                                    tr("XBEL Files (*.xbel *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Открытие файла"),
                             tr("Не получается прочитать файл %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    MainTreeModel *model = static_cast<MainTreeModel*>(ui->treeView->model());
    model->open(&file);
}

/* *
@brief Функция сохраненяет информацию с таблицы в XML файл
@param[in] void
@return void
@warning
*/
void ModBusClient::saveAs(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QDir::currentPath(), tr("XML Files (*.xml)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Ошибка сохранения"), tr("Не получается сохранить файл %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    MainTreeModel *model = static_cast<MainTreeModel*>(ui->treeView->model());
    model->save(&file);
}

/* *
@brief Функция вызывает окно о программе
@param[in] void
@return void
@warning
*/
void ModBusClient::about(void)
{
    QMessageBox::about(this, tr("ModBusClient"), tr("The <b>ModBusClient</b> ModBusClient конфигуратора"));
}

/* *
@brief Функция связывает сигналы от кнопок со cлотами
@param[in] void
@return void
@warning
*/
void ModBusClient::createActions(void)
{
    openAct = new QAction(tr("&Открыть..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("&Сохранить как..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("&Выход"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&О программе"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void ModBusClient::on_addTagBefore_btn_clicked()
{
    PositionToInsert position = INSERT_BEFORE;

    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    if(index.parent() == QModelIndex())
        position = INSERT_START;

    // Делаем с shift'ом?
    if(ui->shift_ckbox->isChecked())
        addTag(TAG_SHIFT, position);
    else
        addTag(TAG_NONE, position);
}

void ModBusClient::addTag(TypeOfShift shiftType, PositionToInsert insertType)
{

    // Создать хэш аттрибутов
    QHash<QString, QString> attributesUI;
    attributesUI["name"]        = ui->name_lnedit->text();
    attributesUI["type"]        = ui->type_cmbox->currentText();
    attributesUI["bin_mask"]    = ui->mask_lnedit->text();
    attributesUI["id"]          = ui->id_lnedit->text();
    attributesUI["description"] = ui->description_lnedit->text();
    attributesUI["shiftType"]   = QString::number(shiftType);
    attributesUI["external"]    = ui->bind_lnedit->text();
    attributesUI["number"]      = "";
    attributesUI["grp_number"]  = "0";

    // Вставить новую строку
    QModelIndex      index      = ui->treeView->selectionModel()->currentIndex();
    MainTreeModel   *model      = static_cast<MainTreeModel*>(ui->treeView->model());//или dynamic_cast<derv_class *>(base_class_ptr_expr)?

    // Установить новое выделение
    QModelIndex newIndex        = model->insertTag(insertType, index, attributesUI);
    if(newIndex != QModelIndex())
        ui->treeView->setCurrentIndex(newIndex);
}

void ModBusClient::on_addTagAfter_btn_clicked()
{
    PositionToInsert position = INSERT_AFTER;

    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    if(index.parent() == QModelIndex())
        position = INSERT_END;

    // Делаем с shift'ом?
    if(ui->shift_ckbox->isChecked())
        addTag(TAG_SHIFT, position);
    else
        addTag(TAG_NONE, position);
}

void ModBusClient::errorShow(QString message)
{
    if(message == "id absent")
        QMessageBox::warning(this, tr("Идентификатор"), tr("Поле <b>Идентификатор</b> у одного или более тегов не заполнено. Такие теги не будут сохранены"));
    else if(message == "id exists")
        QMessageBox::warning(this, tr("Идентификатор"), tr("Тег с указанным <b>идентификатором</b> уже существует."));
    else if(message == "id used")
        QMessageBox::warning(this, tr("Идентификатор"), tr("<b>Идентификатор</b> уже используется"));
    else if(message == "id's don't unique")
        QMessageBox::warning(this, tr("Идентификатор"), tr("Есть теги с одинаковыми <b>идентификаторами</b>. Повторяющиеся теги не будут сохранены"));
    else if(message == "id empty")
        QMessageBox::warning(this, tr("Идентификатор"), tr("Поле <b>идентификатор</b> не может быть пустым"));
    else if(message == "readFile")
        QMessageBox::warning(this, tr("Чтение файла"), tr("Ошибка чтения файла"));
    else if(message == "types wrong")
        QMessageBox::warning(this, tr("Интерфейс"), tr("Ошибка заполнения поля типов регистра"));
    else if(message == "var binded")
        QMessageBox::warning(this, tr("Интерфейс"), tr("<b>Переменная</b> не может быть привязана повторно"));

}

void ModBusClient::receivedTagInfo(RegistrGroup /*currentRegGroupArg*/, int /*currentTagArg*/)
{
    /*
    currentRegGroup = currentRegGroupArg;
    currentTag      = currentTagArg;

    //qDebug() << Q_FUNC_INFO << "currentRegGroup: " << currentRegGroup;
    //qDebug() << Q_FUNC_INFO << "currentTag     : " << currentTag;

    switch(currentRegGroupArg)
    {
    case REG_GROUP_IR:
        ui->type_cmbox->setEnabled(true);
        ui->mask_lnedit->setEnabled(true);
        break;

    case REG_GROUP_HR:
        ui->type_cmbox->setEnabled(true);
        ui->mask_lnedit->setEnabled(true);
        break;

    case REG_GROUP_DI:
        ui->type_cmbox->setDisabled(true);
        ui->mask_lnedit->setDisabled(true);
        break;

    case REG_GROUP_CO:
        ui->type_cmbox->setDisabled(true);
        ui->mask_lnedit->setDisabled(true);
        break;

    case REG_GROUP_NONE:
        break;

    default:
        break;
    }
    */
}

// Обновление ноды
void ModBusClient::save()
{

   // Убедиться в наличии базовой ноды
    if(!_root.hasChildNodes())
    {
        QDomElement     baseNode;
        QDomDocument    domDocument;
        baseNode      = domDocument.createElement("base");
        _root.appendChild(baseNode);
    }

    if(mainTreeModel && _isTreeModified)
    {
        MainTreeModel *modelInside = static_cast<MainTreeModel*>(ui->treeView->model());
        modelInside->save();
        _isTreeModified = false;
    }

    if(connectionProps && _isConnectionModified)
    {
        connectionProps->save();
        _isConnectionModified = false;
    }
}

void ModBusClient::treeUpdated(const QModelIndex &/*topLeft*/,
                               const QModelIndex &/*bottomRight*/,
                               const QVector<int> &/*roles = QVector<int>()*/)
{
    _isTreeModified = true;
}

// пока для каждого виджета свой слот
void ModBusClient::modified()
{
    _isConnectionModified = true;
}

void ModBusClient::itemsAreSelected(bool isSelected)
{
    if(!isSelected)
    {
        ui->groupBox->setDisabled(true);
    }
    else
    {
        ui->groupBox->setEnabled(true);
    }
}

void ModBusClient::on_set_suffix_or_prefix_btn_clicked()
{
    QString text;
    ValueAction action = VALUE_APPEND;                 /* дефолт */

    // Есть ли текст?
    if(ui->suffix_or_preffix_lnedit->text() != "")
        text = ui->suffix_or_preffix_lnedit->text();
    else
        return;

    // Выбор нажатой rb
    if(ui->id_choose_prefix_rb->isChecked())
        action = VALUE_PREPEND;

    if(ui->id_choose_suffix_rb->isChecked())
        action = VALUE_APPEND;

    // Изменить ID
    QModelIndexList indexes = ui->treeView->selectionModel()->selectedRows(Col::num("Идентификатор привязанной переменной"));
    MainTreeModel   *model  = static_cast<MainTreeModel*>(ui->treeView->model());
    model->modifyTags(indexes, text, action);
}

void ModBusClient::on_delete_suffix_or_prefix_btn_clicked()
{
    QString text;
    ValueAction action = VALUE_DEL_START;                 /* дефолт */

    // Есть ли текст?
    if(ui->suffix_or_preffix_lnedit->text() != "")
        text = ui->suffix_or_preffix_lnedit->text();
    else
        return;

    // Выбор нажатой rb
    if(ui->id_choose_prefix_rb->isChecked())
        action = VALUE_DEL_START;

    if(ui->id_choose_suffix_rb->isChecked())
        action = VALUE_DEL_END;

    // Изменить ID
    QModelIndexList indexes = ui->treeView->selectionModel()->selectedRows(Col::num("Идентификатор привязанной переменной"));
    MainTreeModel   *model  = static_cast<MainTreeModel*>(ui->treeView->model());
    model->modifyTags(indexes, text, action);
}

void ModBusClient::on_up_btn_clicked()
{
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    ui->treeView->keyPressEvent(event);
}

void ModBusClient::on_down_btn_clicked()
{
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    ui->treeView->keyPressEvent(event);
}

void ModBusClient::on_deleteTag_btn_clicked()
{

    QModelIndexList indexes = ui->treeView->selectionModel()->selectedRows(Col::num("Идентификатор привязанной переменной"));
    MainTreeModel   *model  = static_cast<MainTreeModel*>(ui->treeView->model());

    // Удалить
    for(int i = indexes.count() - 1; i >= 0 ; --i)
    {
        // Если не тег с названием группы
        if(indexes.at(i).parent() != QModelIndex())
        {
            QModelIndex index = indexes.at(i);
            model->removeRow(index.row(), index.parent());
        }
    }

}
void ModBusClient::on_bind_lnedit_textChanged(const QString &varName)
{
    if(varName.isEmpty())
    {
        ui->bind_lnedit->setStyleSheet("");
        return;
    }

    // Проверить наличие переменной
    if(_variablesProvider->variablesMap().value(varName) == NULL)
    {
        ui->bind_lnedit->setStyleSheet("background-color: lightsalmon");
    }
    else
    {
        ui->bind_lnedit->setStyleSheet("background-color: Mediumspringgreen");

        cAbstractVariable *varPLC = _variablesProvider->variablesMap().value(varName);

        if(ui->name_lnedit->text().isEmpty())
            ui->name_lnedit->setText(varPLC->name());

        ui->type_cmbox->setCurrentText(varPLC->types().first()->name());
        ui->description_lnedit->setText(varPLC->documentation());

        ui->id_lnedit->setText(varName);
        MainTreeModel   *model  = static_cast<MainTreeModel*>(ui->treeView->model());
        if(model->getListID().contains(varName))
            model->error("id exists");

    }
}
