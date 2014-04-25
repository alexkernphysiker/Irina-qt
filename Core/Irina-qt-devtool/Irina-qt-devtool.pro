include("../../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
LIBS += -L$$DEPENDPATH -lgenerateSource
#project configuration
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Irina-qt-devtool
TEMPLATE = app
SOURCES += masterwindow.cpp main-createproject.cpp
HEADERS  += masterwindow.h
FORMS    += masterwindow.ui
