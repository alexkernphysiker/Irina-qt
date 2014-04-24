#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#https://github.com/alexkernphysiker/MathLibs
INCLUDEPATH += ../../../MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
#project configuration
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Irina-qt
TEMPLATE = app
SOURCES += irina-qt-executable.cpp \
	outputextensionselector.cpp \
	irinaqterrorlog.cpp
HEADERS += outputextensionselector.h \
	irinaqterrorlog.h
FORMS += outputextensionselector.ui \
	irinaqterrorlog.ui
