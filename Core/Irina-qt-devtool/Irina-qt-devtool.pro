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
LIBS += -L$$DEPENDPATH -lgenerateSource
#project configuration
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Irina-qt-devtool
TEMPLATE = app
SOURCES +=	masterwindow.cpp \
	main-createproject.cpp
HEADERS  += masterwindow.h
FORMS    += masterwindow.ui
