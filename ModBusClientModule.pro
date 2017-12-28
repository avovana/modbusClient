#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T11:09:00
#
#-------------------------------------------------

QT       += xml core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModBusClient


CONFIG (debug, debug|release) {
    DLLDESTDIR = ../../plc_debug/modules
    DESTDIR = build/debug
    TARGET = ModBusClientMaind
} else {
    DLLDESTDIR = ../../plc_release/modules
    DESTDIR = build/release
    TARGET = ModBusClientMain
}

#TEMPLATE = app
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += src interfaces


SOURCES += main.cpp\
    src/modbusclient.cpp \
    src/modbusclientmain.cpp \
    src/delegates/delegatecombobox.cpp \
    src/delegates/delegateid.cpp \
    src/delegates/delegatemask.cpp \
    src/delegates/delegatesymbols.cpp \
    src/widgets/connection/connectionproperties.cpp \
    src/widgets/lineedit/lineedit.cpp \
    src/widgets/variablelist/variablelistmodel/treeitem.cpp \
    src/widgets/variablelist/variablelistmodel/treemodel.cpp \
    src/widgets/variablelist/variablelist.cpp \
    src/widgets/tree/model/maintreeitem.cpp \
    src/widgets/tree/model/maintreemodel.cpp \
    src/widgets/tree/view/treeview.cpp \
    src/sharedinfo/col.cpp \
    src/delegates/delegatebutton.cpp \
    src/sharedinfo/type.cpp \
    src/sharedinfo/parser.cpp

HEADERS  += \
    src/modbusclient.h \
    src/modbusclientmain.h \
    interfaces/moduleinterface.h \
    interfaces/cElementData.h \
    interfaces/cElementProvider.h \
    interfaces/cInstance.h \
    interfaces/cobject.h \
    interfaces/contextmenumanagerinterface.h \
    interfaces/cpropertiesprovider.h \
    interfaces/creatorinterface.h \
    interfaces/cvariablesprovider.h \
    interfaces/editorinterface.h \
    interfaces/IDebuggable.h \
    interfaces/IEditable.h \
    interfaces/IElementsControllable.h \
    interfaces/IPrintable.h \
    interfaces/ISearchable.h \
    interfaces/IVerifiable.h \
    interfaces/mainpage.h \
    interfaces/moduleinterface.h \
    interfaces/objectinterface.h \
    interfaces/utilsinterface.h \
    interfaces/DeviceStateInterfaces/IRuntimeState.h \
    interfaces/DeviceStateInterfaces/ITagBase.h \
    src/delegates/delegatecombobox.h \
    src/delegates/delegateid.h \
    src/delegates/delegatemask.h \
    src/delegates/delegatesymbols.h \
    src/sharedinfo/enums.h \
    src/widgets/connection/connectionproperties.h \
    src/widgets/lineedit/lineedit.h \
    src/widgets/variablelist/variablelistmodel/treeitem.h \
    src/widgets/variablelist/variablelistmodel/treemodel.h \
    src/widgets/variablelist/variablelist.h \
    src/moduleinterface.h \
    src/widgets/tree/model/maintreeitem.h \
    src/widgets/tree/model/maintreemodel.h \
    src/widgets/tree/view/treeview.h \
    src/sharedinfo/col.h \
    src/delegates/delegatebutton.h \
    src/sharedinfo/type.h \
    src/sharedinfo/parser.h

FORMS    += \
    modbusclient.ui \
    src/widgets/variablelist/variablelist.ui \
    src/widgets/connection/connectionproperties.ui

DISTFILES += \
    interfaces/Doxyfile

RESOURCES += \
    data/resources.qrc \
    templates.qrc

MOC_DIR = ./build-files/MOC
OBJECTS_DIR = ./build-files/OBJ
RCC_DIR = ./build-files/RCC
UI_DIR = ./build-files/UI

CONFIG += C++11

HEADERS += src/version.h

win32 {
    # Создаем файл ресурсов
    win32:resourceCreator.commands = $$PWD/build_utils/version_creator.bat resource 1 29
    QMAKE_EXTRA_TARGETS += resourceCreator
    PRE_TARGETDEPS += resourceCreator
    # Собираем файл ресурсов в .o файл
    resbuild.depends=resourceCreator
    win32:resbuild.commands = windres -i resource.rc -o release\\resource_res.o --include-dir=. -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_PLUGIN -DQT_TESTLIB_LIB -DQT_SQL_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_HAVE_MMX -DQT_HAVE_3DNOW -DQT_HAVE_SSE -DQT_HAVE_MMXEXT -DQT_HAVE_SSE2 -DQT_THREAD_SUPPORT
    QMAKE_EXTRA_TARGETS += resbuild
    POST_TARGETDEPS += resbuild
    # Подключаем .o файл к проекту
    LIBS += release\\resource_res.o
}
