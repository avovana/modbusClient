#ifndef CELEMENTDATA_H
#define CELEMENTDATA_H

/**
    \defgroup interfaces Интерфейс взаимодействия плагинов с ПЛК Дизайнером
    \brief Содержит основные общие интерфейсы и типы
  **/
/*! \defgroup common_types Разделяемые типы
     \ingroup interfaces
*/
///@{


#include <QObject>
#include <QMimeData>
#include <QtXml>

/** @enum type_enum */
/// Перечисление, описывающее типы данных в соответствии со стандартом МЭК 61131-3 
enum type_enum
{
    st_BOOL,///< BOOL - 1 bit
    st_BYTE,///< BYTE - 8 bit (1 byte)
    st_WORD,///< WORD - 16 bit (2 byte)
    st_DWORD,///< DWORD - 32 bit (4 byte)
    st_LWORD,///< LWORD - 64 bit (8 byte)
    st_SINT,///< SINT - signed short (1 byte)
    st_INT,///< INT - signed integer (2 byte)
    st_DINT,///<DINT - double integer (4 byte)
    st_LINT,///<LINT - long integer (8 byte)
    st_USINT,///< USINT - unsigned short (1 byte)
    st_UINT,///< UINT - unsigned integer (2 byte)
    st_UDINT,///< UDINT - unsigned double integer (4 byte)
    st_ULINT,///< ULINT - unsigned long integer (8 byte)
    st_REAL,///< REAL - (4 byte)
    st_LREAL,///< LREAL - (8 byte)
    st_TIME,///< TIME - длительность работы таймеров, процессов
    st_DATE,///< DATE - календарная дата
    st_DT,///< DATE_AND_TIME: дата и время
    st_TOD,///< TIME_OF_DAY - время дня
    st_STRING,///< STRING - символьная строка. Для экранирования спецсимволов используется знак доллара.
    st_WSTRING,///< WSTRING - строка в многобайтовой кодировке
    st_ARRAY,///< ARRAY - несколько занчений хранится в одной переменой
    st_DERIVED,///< DERIVED - тип унаследован от одного из указанных выше
    st_ENUM,///< ENUM - перечисление
    st_SUBRANGESIGNED,///< SUBRANGESIGNED - знаковая ограниченная последовательность
    st_SUBRANGEUNSIGNED,///< SUBRANGEUNSIGNED - беззнаковая ограниченная последовательность
    st_STRUCT,///< STRUCT - структура
    st_ANY,///< ANY - любой тип
    st_ANY_DERIVED,///< ANY_DERIVED - любой унаследованный тип
    st_ANY_ELEMENTARY,///< ANY_ELEMENTARY - любой элементарный тип
    st_ANY_MAGNITUDE,///< ANY_MAGNITUDE
    st_ANY_NUM,///< ANY_NUM - любое число
    st_ANY_REAL,///< ANY_REAL - любое число с плавающей точкой
    st_ANY_INT,///< ANY_INT - любое целое число 
    st_ANY_BIT,///< ANY_BIT - любое тип длиной 1 бит
    st_ANY_STRING,///< ANY_STRING - любая строка
    st_ANY_DATE,///< ANY_DATE - любая дата
    st_POINTER,///< POINTER - любой указатель

    st_TEXT = QMetaType::User,///< TEXT - текст. <em>Тип не включен в стандарт МЭК 61131-3. Используйте st_STRING или st_WSTRING вместо этого типа.</em> @see st_STRING @see st_WSTRING
    st_HTML,///< HTML - текст с разметкой в формате HTML. <em>Тип не включен в стандарт МЭК 61131-3. Используйте st_STRING или st_WSTRING вместо этого типа.</em> @see st_STRING @see st_WSTRING
    st_TYPE,///<TYPE - тип, <em>Тип не включен в стандарт МЭК 61131-3. Используйте st_ENUM вместо этого типа.</em> @see st_ENUM
    st_BASIS_TYPE///<BASIS_TYPE - тип, который может быть использован в качестве типа переменной. <em>Тип не включен в стандарт МЭК 61131-3. Используйте st_ENUM вместо этого типа.</em> @see st_ENUM
};


///Описание типа переменной или значения
class cAbstractType
{
public:
    virtual ~cAbstractType(){}
    virtual QString name() const = 0; ///< Имя типа
    virtual QString text() const = 0;///< Название типа
    virtual type_enum type() const  = 0;///< Идентификатор типа
    virtual QVariantMap avaliableValues() const = 0;///< Карта допустимых значений.
    virtual QString CType() const  = 0; ///< Имя типа
    virtual int size() const  = 0; ///< Размер  типа

};
///Описание свойства элемента
class cAbstractProperty
{
public:
    virtual ~cAbstractProperty(){}
    virtual QVariant defaultValue() const = 0;///< Значение по умолчанию
    virtual QVariant value() const = 0;///< Текущее значение
    virtual QVariant max() const = 0;///< Максимальное значение
    virtual QVariant min() const = 0;///< Минимальное значение
    virtual QString name() const = 0;///< Имя свойства
    virtual QString text() const = 0;///< Наименование свойства
    virtual cAbstractType* type() const = 0;///< Тип свойства
    virtual QString documentation() const	= 0;///< Краткое описание свойства
    virtual QString F1HelpFile() const = 0;///< Файл справки, содержащий данное свойство
    virtual unsigned long F1HelpID() const = 0;///< Идентификатор свойства в файле документации
    virtual QString contextHelpFile() const = 0;///< Файл контекстной справки, содержащий данное свойство
    virtual unsigned long contextHelpID() const = 0;///< Идентификатор свойства в файле контекстной документации
    virtual QDomNode node() const = 0;///< Нода, ассоциированная с данным свойством

    virtual void setValue(QVariant) = 0;///< Установить текущее значение
};



/// Перечисление, описывающее область видимости переменной
typedef enum VariableVisiblityType {
    VS_LOCAL = 0,///< Локальная переменная
    VS_GLOBAL = 128, ///< Глобальная переменная
} VariableVisibleStatus_t;

/// Перечисление, описывающее тип переменной
typedef enum LocationDataFlowDirection {
    DFD_INPUT, ///< Входная переменная
    DFD_OUTPUT,///< Выходная переменная
    DFD_MEMORY///< Переменная, отображаемая в память
} LocationDataflowDirection_t;

/// Описание локации переменной
typedef class cLocationInfo {
public:
    /// Размещение переменной в иерархии устройств и параметров.
    /// @return Список идентификаторов устройств, блоков параметров, который отображает расположение параметра, и идентификатор параметра.
    QList< int > loc () {
        QList< int > loc;

        if (0 < _loc.length()) {
            loc = _loc;
        }
        else {
            if (isIECNameValid ()) {
                QStringList splited = _IEC_name
                        .replace(QRegularExpression("\%[^0-9]*"), "")
                        .split(".");

                std::for_each(
                    splited.begin(),
                    splited.end(),
                    [&](QString part) {
                        loc.append(part.toInt());
                    });
                _loc = loc;
            }
        }

        return loc;
    }

    /// Метод для задания размещения переменной в иерархии устройств и параметров.
    /// @param i_loc новое значение размещения переменной.
    void setLoc (const QList< int >& i_loc) {
        _loc = i_loc;
    }

    /// Локация в формате IEC 61131-3 раздел 2.4.
    /// @return Локация в формате IEC 61131-3. В случае, если поле не было заполнено, будет выполнена попытка формирования на основании @see cLocationInfo_t::loc().
    QString IECName () {
        QString IEC_name;

        if (!isIECNameValid ()) {
            _IEC_name = formatIECName();
        }

        if (isIECNameValid ()) {
            IEC_name = _IEC_name;
        }

        return IEC_name;
    }

    /// Метод для задания локации в формате IEC 61131-3 раздел 2.4.
    /// @param i_IEC_name новое значение локации в формате IEC 61131-3 раздел 2.4.
    void setIECName (const QString& i_IEC_name) {
        _IEC_name = i_IEC_name;
    }

   /// Метод для определения валидности текущего значения локации в формате IEC 61131-3 раздел 2.4.
   /// @return true - локация валидна, false - локация не валидна.
    bool isIECNameValid () {
        return QRegExp("\\%[IQX][XBWDL](\\d+\\.){4}\\d+").exactMatch(_IEC_name);
    }

    /// Метод для определения валидности всей структуры описания локации переменной.
    /// return true - описание валидно, false - описание не валидно.
    bool isValid () {
        _IEC_name = formatIECName();

        return isIECNameValid();
    }

    /// Метод получения IEC-типа переменной.
    /// @return Название типа переменной.
    QString IECType() const {
        return _IEC_type;
    }

    /// Метод задания IEC-типа переменной.
    /// @param i_IEC_type новый IEC-тип переменной.
    void setIECType(const QString& i_IEC_type) {
        _IEC_type = i_IEC_type;
    }

    /// Метод получения имени C-переменной, которое следует использовать при генерации кода.
    /// @return Имя C-переменной, которое следует использовать при генерации кода.
    QString CName() {
        QString C_name;

        if (!isIECNameValid ()) {
            _IEC_name = formatIECName();
        }

        if (isIECNameValid ()) {
            if (0 == _C_name.length()) {
                _C_name = formatCName();
            }

            C_name = _C_name;
        }

        return C_name;
    }

    /// Метод задания имени C-переменной, которое следует использовать при генерации кода.
    /// @param i_C_name новое имя C-переменной, которое следует использовать при генерации кода.
    void setCName(const QString& i_C_name) {
        _C_name = i_C_name;
    }

    /// Метод получения класса локации: входная, выходная, в памяти.
    /// @return Текущее значение класса локации.
    QString Class() const {
        return _class;
    }

    /// Метод задания класса локации: входная, выходная, в памяти.
    /// @param i_class Новое значение класса локации.
    void setClass(const QString& i_class) {
        _class = i_class;
    }

    /// Метод получения размера локации.
    /// @return Размер локации
    QString size() const {
        return _size;
    }

    /// Метод установления размера локации.
    void setSize(const QString& i_size) {
        _size = i_size;
    }

private:
    QString formatCName() {
        QString C_name = "__" + _class + _size;

        auto current_loc = loc();

        for (auto term : current_loc) {
            C_name += QString::number(term) +
                    "_";
        }

        C_name.truncate(C_name.length() - 1);

        return C_name;
    }


    QString formatIECName() {
        QString IEC_name = "%" + _class + _size;

        auto current_loc = loc();

        for (auto term : current_loc) {
            IEC_name += QString::number(term) +
                    ".";
        }

        IEC_name.truncate(IEC_name.length() - 1);

        return IEC_name;
    }


private:
    QList< int > _loc;///< Локация в формате номер ПЛК - номер крейта - номер слот - номер канала - номер параметра
    QString _IEC_type;///< Тип локации в формате IEC 61131-3 раздел 2.4
    QString _IEC_name;///< Локация в формате IEC 61131-3 раздел 2.4
    QString _C_name;///< Имя, по которому производится обращение к переменной в C-коде
    QString _class;///< Тип направления @see LocationDataflowDirection_t
    QString _size;///< Размер
} cLocationInfo_t;

Q_DECLARE_METATYPE(cLocationInfo_t)

/// \brief Класс, описывающий текст
/// \warning Использование класса требует включения C++11.\n Для этого достаточно включить в .pro-файл запись CONFIG += C++11

class Text : public QString{
public:
    explicit Text(const QChar *unicode, int size = -1): QString(unicode,size) {}
    Text(QChar c): QString(c) {}
    Text(int size, QChar c): QString(size,c) {}
    inline Text(QLatin1String latin1): QString(latin1) {}
    inline Text(const QString &other = QString()): QString(other) {}
    inline Text(QString && other) : QString(other) {}
    /// Преобразование текста в QVariant
    operator QVariant() 
    {
        QVariant value;
        value.setValue<Text> (*this);
        return value;
    }
};
Q_DECLARE_METATYPE(Text)

/// \brief Класс, описывающий текст в формате HTML
/// \warning Использование класса требует включения C++11.\n Для этого достаточно включить в .pro-файл запись CONFIG += C++11

class HTML : public Text
{
public:
    explicit HTML(const QChar *unicode, int size = -1): Text(unicode,size) {}
    HTML(QChar c): Text(c) {}
    HTML(int size, QChar c): Text(size,c) {}
    inline HTML(QLatin1String latin1): Text(latin1) {}
    inline HTML(const QString &other = QString()): Text(other) {}
    inline HTML(QString && other) : Text(other) {}
    /// Преобразование HTML в QVariant
    operator QVariant()
    {
        QVariant value;
        value.setValue<HTML> (*this);
        return value;
    }
};
Q_DECLARE_METATYPE(HTML)

/// \brief Класс, описывающий размещение переменной в EEPROM
/// \warning Использование класса требует включения C++11.\n Для этого достаточно включить в .pro-файл запись CONFIG += C++11

class EEPROM;
/// Описание переменной
class cAbstractVariable
{
public:
    enum VariableDestination
    {
        INPUT_VARIABLE,///< Входная переменная
        OUTPUT_VARIABLE,///< Выходная переменная
        INPUT_OUTPUT_VARIABLE,///< Входная-выходная переменная
        LOCAL_VARIABLE,///< Локальная переменная
        EXTERNAL_VARIABLE///< Разделяемая переменная
    };
public:
    virtual ~cAbstractVariable(){}
    virtual bool checkSameType() const = 0; ///< Флаг необходимости проверки соответствия типов
    virtual QString extend_by() const = 0;///< Имя свойства, описывающего расширяемость переменной
    virtual bool extensible() const = 0;///< Флаг возможности расширения
    virtual QString name() const = 0;///< Имя переменной
    virtual QList<cAbstractType*> types() const = 0;///< Список допустимых типов
    virtual QVariant value() const = 0;///< Значение переменной
    virtual QVariant value(long tick) const = 0;///< Значение переменной на tick - срезе

    virtual QVariant initialValue() const = 0;///< Значение переменной по умолчанию
    virtual QDateTime timestamp() const = 0;///< Метка времени последнего обновления значения переменной
    virtual QDateTime timestamp(long tick) const = 0;///< Метка времени обновления значения переменной на tick - срезе

    virtual QString documentation() const = 0;///< Краткое описание переменной
    virtual QString F1HelpFile() const = 0;///< Файл справки, содержащий описание данной переменной
    virtual unsigned long F1HelpID() const = 0;///< Идентификатор описания переменной в файле документации
    virtual QString contextHelpFile() const = 0;///< Файл контекстной справки, содержащий описание данной переменной
    virtual unsigned long contextHelpID() const 	= 0;///< Идентификатор описания переменной в файле контекстной документации
    virtual QDomNode node() const = 0;///< Нода, ассоциированная с данной переменной
    virtual QVariantMap additionalAttributes() const = 0;///< Карта дополнительных атрибутов


    virtual void setCheckSameType(bool) = 0;///< Установить флаг необходимости проверки соответствия типов
    virtual void setExtend_by(QString) = 0;///< Установить имя свойства, описывающего расширяемость переменной
    virtual void setExtensible(bool) = 0;///< Установить флаг возможности расширения
    virtual void setName(QString) = 0;///< Установить имя переменной
    virtual void setValue(QVariant) = 0;///< Установить значение переменной
    virtual void setDocumentation(QString) = 0;///< Установить краткое описание переменной
    virtual void setF1HelpFile(QString ) = 0;///< Установить файл справки, содержащий описание данной переменной
    virtual void setF1HelpID(unsigned long) = 0;///< Установить идентификатор описания переменной в файле документации
    virtual void setContextHelpFile(QString ) = 0;///< Установить файл контекстной справки, содержащий описание данной переменной
    virtual void contextHelpID(unsigned long ) = 0;///< Установить идентификатор описания переменной в файле контекстной документации

    virtual void setAdditionalAttributes(QVariantMap) = 0;///< Задать карту дополнительных атрибутов
    virtual void setType(QString) = 0;///< Установить тип переменной
    virtual void setType(type_enum) = 0;///< Установить тип переменной
    virtual void setInitialValue(QVariant) = 0;///< Установить значение по умолчанию


    virtual VariableDestination destination() const = 0;///< Возвращает тип направления переменной
    virtual void setDestination(const VariableDestination &destination) = 0;///< Устанавливает тип направления переменной

    virtual bool isConst() const = 0;///< Возвращает true, если переменная является константой
    virtual void setIsConst(bool isConst) = 0;///< Устанавливает флаг константности переменной
    virtual QMap<QString, cAbstractProperty *> props() = 0;///< Возвращает карту свойств переменной
    virtual QHash<QString, cAbstractVariable *> internalVariables() const = 0;///< Возвращает карту внутренних переменных

    virtual bool isPersistent() const = 0;///< Возвращает true, если переменная сохраняется в EEPROM
    virtual void setIsPersistent(bool isPersistent) = 0;///< Устанавливает флаг сохраняемости переменной

    virtual EEPROM persistentAddress() const = 0;
    virtual void setPersistentAddress(EEPROM persistentAddress) = 0;

};


class EEPROM : public Text
{
public:
    explicit EEPROM(const QChar *unicode, int size = -1): Text(unicode,size) {}
    EEPROM(QChar c): Text(c) {}
    EEPROM(int size, QChar c): Text(size,c) {}
    inline EEPROM(QLatin1String latin1): Text(latin1) {}
    inline EEPROM(const QString &other = QString()): Text(other) {}
    inline EEPROM(QString && other) : Text(other) {}
    cAbstractVariable *variable(){return _variable;}
    void setVariable(cAbstractVariable *val){_variable = val;}
    /// Преобразование EEPROM в QVariant
    operator QVariant()
    {
        QVariant value;
        value.setValue<EEPROM> (*this);
        return value;
    }
protected:
    cAbstractVariable *_variable;
};
Q_DECLARE_METATYPE(EEPROM)

/// Описание программного блока
class cPOUBlock
{
public:
    virtual ~cPOUBlock(){}
    virtual QString name() const = 0;///< Имя блока
    virtual QIcon icon() const = 0;///< Иконка блока в дереве проектов
    virtual QString pouType() const = 0;///< Тип блока
    virtual QMap<QString, cAbstractProperty*> properties() const = 0;///< Список свойств блока
    virtual QMap<QString, cAbstractVariable*> inputVars() const = 0;///< Список входных переменных
    virtual QMap<QString, cAbstractVariable*> outputVars() const  = 0;///< Список выходных переменных
    virtual QMap<QString, cAbstractVariable*> inputOutputVars() const = 0;///< Список переменных без направления
    virtual QMap<QString, cAbstractVariable*> localVars() const = 0;///< Список локальных переменных 
    virtual QMap<QString, QString> body() const = 0;///< Тело программного блока
    virtual QString F1HelpFile() const = 0;///< Файл справки, содержащий описание данного блока
    virtual unsigned long F1HelpID() const = 0;///< Идентификатор описания блока в файле документации
    virtual QString contextHelpFile() const = 0;///< Файл контекстной справки, содержащий описание данного блока
    virtual unsigned long contextHelpID() const = 0;///< Идентификатор описания блока в файле контекстной документации
    virtual QDomNode node() const = 0;///< Нода, ассоциированная с данным блоком
    virtual QString documentation() const = 0;///< Краткая документация по блоку
};

///@brief Mime-тип, с которым передаются блоки из библиотеки переменных
const QString VariableDataMimeType = "application/Vympel.Variable";

///@brief Описание блока-переменной. 
/**
    Блок-переменная используется при перетаскивании переменной из окна навигатора в окно редактора.
    Будучи унаследованным от QMimeData, cVariableData содержит в себе идентификатор mime-типа @see VariableDataMimeType и данные
*/
class cVariableData : public QMimeData //type = "application/Vympel.Variable"
{
    Q_OBJECT
public:
    virtual cAbstractVariable* variable() const = 0;///< Описание переменной. @see cAbstractVariable
};

///@brief Mime-тип, с которым передаются блоки из библиотеки компонентов
const QString ElementDataMimeType = "application/Vympel.POUBlock";

///@brief Описание блока-компонента. 
/**
    Блок-компонента используется при перетаскивании элемента из библиотеки компонентов в окно редактора.
    Будучи унаследованным от QMimeData, cElementData содержит в себе идентификатор mime-типа @see ElementDataMimeType и данные
*/
class cElementData : public QMimeData //type = "application/Vympel.POUBlock"
{
    Q_OBJECT
public:
    virtual cPOUBlock* pouBlock() const = 0;///< Описание блока. @see cPOUBlock
};
/// @}
#endif // CELEMENTDATA_H
