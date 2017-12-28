#ifndef VARIABLELIST_H
#define VARIABLELIST_H
#include <QWidget>
#include "variablelistmodel/treemodel.h"
#include "src/sharedinfo/enums.h"

/* @brief Виджет для предоставления списка переменных.
 * Отсылает название выбранной переменной.
 */

namespace Ui {
class VariableList;
}

class VariableList : public QWidget
{
    Q_OBJECT

public:
    explicit VariableList(cVariablesProvider *variablesProvider, QString data, ReceivedData dataType, QWidget *parent = 0);
    virtual ~VariableList(); // Почему не вызывается?
    virtual void closeEvent(QCloseEvent *event) override;

signals:
    void setEnabled(bool);          // сигнал верхнему уровню о блокировки остального интерфейса
    //void usingVariable(QString);    // сигнал с выбранным названием
    void usingVariable(QString, ReceivedData);    // сигнал с выбранным названием

private slots:
    void on_accept_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::VariableList *ui;
    ReceivedData _dataType;
};

#endif // VARIABLELIST_H
