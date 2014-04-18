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
LIBS += -L$$DESTDIR -lmontecarlo
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(montecarlo_enlos)
SOURCES += enlos.cpp \
	enlosview.cpp \
	enlossintargetview.cpp \
	register_enlos.cpp
HEADERS += enlos.h \
	enlosview.h \
	enlossintargetview.h \
	register_enlos.h
FORMS += \
	enlosview.ui \
	enlossintargetview.ui
