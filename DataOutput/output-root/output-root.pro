#author: alex_kernphysiker@privatdemail.net

#build location
unix: DESTDIR = /home/bin
win32: DESTDIR = D:/bin
win32: DLLDESTDIR = $$DESTDIR
DEPENDPATH += $$DESTDIR
#configuration
win32: DEFINES += WINBUILD
CONFIG (debug, debug|release) {
	OBJECTS_DIR=$$DESTDIR/debug
} else {
	OBJECTS_DIR=$$DESTDIR/release
}
unix:{
  QMAKE_LFLAGS += -Wl,--rpath=$$DESTDIR
  QMAKE_LFLAGS_RPATH=
}
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
SOURCES += \
	register-root-output.cpp \
	outputroot.cpp
HEADERS += \
	register-root-output.h \
	outputroot.h
