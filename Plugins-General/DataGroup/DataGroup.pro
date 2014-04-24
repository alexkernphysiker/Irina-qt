#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../../Core/Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
QT       += core gui
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(format_datagroup)
SOURCES += datagroup.cpp \
	register_datagroup.cpp \
	datagroupview.cpp
HEADERS += datagroup.h \
	register_datagroup.h \
	datagroupview.h
FORMS += datagroupview.ui
