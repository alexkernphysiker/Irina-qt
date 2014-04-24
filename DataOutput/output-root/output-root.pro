#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../../Core/Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#root
LIBS += -L/usr/lib64/root/ -lCore
LIBS += -L/usr/lib64/root/ -lGui
LIBS += -L/usr/lib64/root/ -lGQt
LIBS += -L/usr/lib64/root/ -lCint
LIBS += -L/usr/lib64/root/ -lTree
LIBS += -L/usr/lib64/root/ -lThread
INCLUDEPATH += /usr/include/root
DEPENDPATH += /usr/include/root
#project configuration
QT += core gui
TEMPLATE = lib
CONFIG += plugin
TARGET   = $$qtLibraryTarget(output-root)
INCLUDEPATH += ../../DataSorting/Irina-qt-headers
SOURCES += register-root-output.cpp \
	outputroot.cpp
HEADERS += register-root-output.h \
	outputroot.h
