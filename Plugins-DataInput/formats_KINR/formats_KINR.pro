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
TARGET   = $$qtLibraryTarget(formats_kinr)
SOURCES += KINR.cpp \
	register_kinr.cpp
HEADERS += KINR.h \
	register_kinr.h
