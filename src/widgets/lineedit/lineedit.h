#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QPushButton>
#include "src/widgets/variablelist/variablelist.h"
#include "src/sharedinfo/enums.h"
#include "sharedinfo/col.h"

class cVariablesProvider;

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(QWidget *parent = 0);
    LineEdit(QWidget *parent, cVariablesProvider *variablesProvider, QString type);

    void setVariablesProvider(cVariablesProvider *variablesProvider);

signals:
    void newVarAdded(const QString &text);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateChooseButton(const QString &text);
    void createVarList();

    void receivedVar(QString varName, ReceivedData);
private:
    QPushButton *choose_btn;
    cVariablesProvider *_variablesProvider;
    ReceivedData _dataType;
    QString      _type;
};

#endif // LINEEDIT_H
