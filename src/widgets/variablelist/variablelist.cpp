#include "variablelist.h"
#include "ui_variablelist.h"
#include <QTreeView>

VariableList::VariableList(cVariablesProvider *variablesProvider,
                           QString data,
                           ReceivedData dataType,
                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VariableList),
    _dataType(dataType)
{
    //ui->listWidget->setWindowTitle("Доступные переменные");
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    // Сам удалится?
    TreeModel *model = new TreeModel(variablesProvider, data, dataType, this);

    ui->treeView->setModel(model);
    ui->treeView->setWindowTitle(QObject::tr("Simple Tree Model"));

    connect(ui->treeView, &QTreeView::doubleClicked, this, &VariableList::on_accept_btn_clicked);
}

VariableList::~VariableList()
{
    delete ui;
}

void VariableList::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event) // почему-то и без него закрывает
    emit setEnabled(true);
}

void VariableList::on_accept_btn_clicked()
{
    QString currText = ui->treeView->currentIndex().data(DATA_FULL_NAME).toString();

    if(!currText.isEmpty())
    {
        //emit usingVariable(currText);
        if(_dataType == RECEIVED_TYPE)
            emit usingVariable(currText, RECEIVED_TYPE);
        else if(_dataType == RECEIVED_NAME)
            emit usingVariable(currText, RECEIVED_NAME);
        else if(_dataType == RECEIVED_ANY)
            emit usingVariable(currText, RECEIVED_ANY);
    }

    this->close();
}

void VariableList::on_cancel_btn_clicked()
{
    this->close();
}
