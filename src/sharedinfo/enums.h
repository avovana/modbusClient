#ifndef ENUMS
#define ENUMS

// Модификация тега
enum ValueAction           /* меняя здесь, меняй везде! */
{
    VALUE_PREPEND,
    VALUE_APPEND,
    VALUE_DEL_START,
    VALUE_DEL_END
};

// Наличие смещения у тега
enum TypeOfShift              /* меняя здесь, меняй везде! */
{
    TAG_OFFSET,
    TAG_SHIFT,
    TAG_NONE,
    TAG_STAT                // Особый тег - "Статистики"
};

// Позиция для вставки тега
enum PositionToInsert           /* меняя здесь, меняй везде! */
{
    INSERT_BEFORE,
    INSERT_AFTER,
    INSERT_START,
    INSERT_END
};

// Группы тегов
enum RegistrGroup               /* меняя здесь, меняй везде! */
{
    REG_GROUP_IR,
    REG_GROUP_HR,
    REG_GROUP_DI,
    REG_GROUP_CO,
    REG_GROUP_STAT,
    REG_GROUP_NONE
};

// Дополнительные роли для моделей
enum DataRoles
{
    DATA_FULL_NAME = 0x0101,
    DATA_SHORT_NAME,
    DATA_TYPE
};

enum ReceivedData
{
    RECEIVED_TYPE,
    RECEIVED_NAME,
    RECEIVED_ANY
};

enum IsVisible
{
    PROP_UNVISIBLE,
    PROP_VISIBLE
};

#endif // ENUMS

