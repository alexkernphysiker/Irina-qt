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
#project configuration
QT       += core gui
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(formats_kinr)
SOURCES += KINR.cpp \
	register_kinr.cpp
HEADERS += KINR.h \
	register_kinr.h
