include("../../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
QT       += core gui
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(format_datagroup)
SOURCES += datagroup.cpp register_datagroup.cpp datagroupview.cpp
HEADERS += datagroup.h register_datagroup.h datagroupview.h
FORMS += datagroupview.ui
