include("../../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
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
SOURCES += register-root-output.cpp outputroot.cpp
HEADERS += register-root-output.h outputroot.h
