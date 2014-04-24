#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../../Core/Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
#project configuration
QT       += core gui
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(format_txt)
SOURCES += textfile.cpp \
	register_textfilelib.cpp
HEADERS += textfile.h \
	register_textfilelib.h
