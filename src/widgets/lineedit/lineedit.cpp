#include "src/widgets/lineedit/lineedit.h"
#include <QToolButton>
#include <QStyle>
#include <QDebug>

/* @brief Конструктор для Дизайнер
 * @warning После вызова этого конструктора неободимо вызвать setVariablesProvider()
 */
LineEdit::LineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    _dataType = RECEIVED_ANY;
    _type = "";


    choose_btn = new QPushButton(this);
    choose_btn->resize(size().width()/5, size().height()/1.5);
    choose_btn->setText("...");
    choose_btn->setCursor(Qt::ArrowCursor);

    connect(choose_btn, &QPushButton::clicked, this, &LineEdit::createVarList);
    connect(this, &LineEdit::textChanged, this, &LineEdit::updateChooseButton);
}

/* @brief Конструктор для использования в коде
 *
 */
LineEdit::LineEdit(QWidget *parent, cVariablesProvider *variablesProvider, QString type)
    : QLineEdit(parent),
      _variablesProvider(variablesProvider),
      _dataType(RECEIVED_TYPE),
      _type(type)
{
    choose_btn = new QPushButton(this);
    choose_btn->resize(size().width()/5, size().height()/2);
    choose_btn->setText("...");
    choose_btn->setCursor(Qt::ArrowCursor);

    connect(choose_btn, &QPushButton::clicked, this, &LineEdit::createVarList);
    connect(this, &LineEdit::textChanged, this, &LineEdit::updateChooseButton);
}

void LineEdit::resizeEvent(QResizeEvent *)
{
    QSize sz = choose_btn->size();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    choose_btn->move(rect().right() - frameWidth - sz.width(),
                    (rect().bottom() + 1 - sz.height())/2);
}

void LineEdit::updateChooseButton(const QString& text)
{
    choose_btn->setVisible(text.isEmpty());
}

void LineEdit::createVarList()
{
    if(_variablesProvider == NULL)
        return;

    // Создать виджет со всеми переменными
    VariableList *varList = new VariableList(_variablesProvider, _type, _dataType);
    connect(varList, &VariableList::usingVariable, this, &LineEdit::receivedVar);

    varList->show();
}

void LineEdit::receivedVar(QString varName, ReceivedData /*dataType*/)
{
    setText(varName);
}

void LineEdit::setVariablesProvider(cVariablesProvider *variablesProvider)
{
    _variablesProvider = variablesProvider;
}
