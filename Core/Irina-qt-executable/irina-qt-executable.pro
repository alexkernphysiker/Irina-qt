include("../../qt_inc_paths.pri")
include("../../../qt_build_path.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
#project configuration
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Irina-qt
TEMPLATE = app
SOURCES += irina-qt-executable.cpp outputextensionselector.cpp irinaqterrorlog.cpp
HEADERS += outputextensionselector.h irinaqterrorlog.h
FORMS += outputextensionselector.ui irinaqterrorlog.ui
