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
#https://github.com/alexkernphysiker/MathLibs
INCLUDEPATH += ../../../MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
LIBS += -L$$DEPENDPATH -lmath_expression
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(fbkin)
HEADERS += fbkin.h \
	kinview.h \
	ruthview.h \
	splineview.h \
	register_fbkin.h
SOURCES += fbkin.cpp \
	kinview.cpp \
	ruthview.cpp \
	splineview.cpp \
	register_fbkin.cpp
FORMS += kinview.ui \
	ruthview.ui \
	splineview.ui
