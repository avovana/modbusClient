#ifndef MODBUSCLIENT_H
#define MODBUSCLIENT_H

#include <QMainWindow>
#include <QtXml>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include "src/widgets/connection/connectionproperties.h"
#include "src/widgets/tree/model/maintreemodel.h"

#include "delegates/delegatecombobox.h"
#include "delegates/delegatesymbols.h"
#include "delegates/delegatemask.h"
#include "delegates/delegateid.h"
#include "delegates/delegatebutton.h"

#include "sharedinfo/type.h"
#include "sharedinfo/col.h"
#include "sharedinfo/enums.h"

namespace Ui {
class ModBusClient;
}

class cVariablesProvider;

class ModBusClient : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool modified READ isModified)

public:
    explicit ModBusClient(QDomNode root, cVariablesProvider *variablesProvider, QWidget *parent = 0);
    ~ModBusClient();
    bool isModified();

    void initConnection();
public slots:
    void            open           (void);          // открытие     XML файла
    void            saveAs         (void);          // сохранение   XML файла
    void            about          (void);          // открытие     окна о программе
    void            errorShow      (QString message);
    void            receivedTagInfo(RegistrGroup currentRegGroupArg, int currentTagArg);
    void            save           ();              //Необходимо сохранить состояние виджета [Леша] не понятно
    void            treeUpdated    (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void            modified       ();
    void            itemsAreSelected(bool isSelected);

signals:
    void            newTagInfo     (const QStringList newTagInfo);
                                                    // отправить    сигнал с информацией для будущего тэга

private slots:
    void on_addTagBefore_btn_clicked();
    void on_addTagAfter_btn_clicked ();

    void setMask                    (const QString &text);

    void on_set_suffix_or_prefix_btn_clicked();
    void on_delete_suffix_or_prefix_btn_clicked();

    void on_up_btn_clicked();
    void on_down_btn_clicked();

    void on_deleteTag_btn_clicked();

    void on_bind_lnedit_textChanged(const QString &varName);

private:
    void            createActions  (void);          // связывание   сигнал-слотов
    void            initUI         (void);          // создание     основных элементов UI
    void            addTag(TypeOfShift shiftType, PositionToInsert position);

    //ProcessTree     *processTree;

    QMenu           *fileMenu;                      // указатель на объект для создания меню
    QMenu           *helpMenu;                      // указатель на объект для создания меню
    QAction         *openAct;                       // указатель на объект для вызова функции
    QAction         *saveAsAct;                     // указатель на объект для вызова функции
    QAction         *exitAct;                       // указатель на объект для вызова функции
    QAction         *aboutAct;                      // указатель на объект для вызова функции
    RegistrGroup     currentRegGroup;
    int              currentTag;

    ConnectionProperties    *connectionProps;
    MainTreeModel           *mainTreeModel;

    QHash<QString, QAbstractItemDelegate*> delegate;         // для установки соответствия "Название столбца" = делегат

private:
    Ui::ModBusClient *ui;
    QDomNode         _root;
    bool             _isTreeModified;
    bool             _isConnectionModified;
    cVariablesProvider *_variablesProvider;
};

#endif // MODBUSCLIENT_H
