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
INCLUDEPATH += ../Irina-qt-headers
#https://github.com/alexkernphysiker/MathLibs
INCLUDEPATH += ../../../MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
LIBS += -L$$DEPENDPATH -lmath_expression
#project configuration
QT       -= gui
TARGET = SortLib
TEMPLATE = lib
DEFINES += SORTLIB_LIBRARY
SOURCES += sortlib_integration.cpp \
	sortlib_sobject.cpp \
	sortlib_datafile.cpp \
	sortlib_counters.cpp \
	sortlib_filters.cpp \
	sortlib_functions.cpp \
	sortlib_operators.cpp \
	sortlib_plotdata.cpp \
	sortlib_sortproject.cpp
HEADERS +=	../Irina-qt-headers/SortLib.h \
	newtype.h \
	sortlib_.h
