#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../../Core/Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(eefilter)
SOURCES += masks.cpp \
	extmaskview.cpp \
	register_masks.cpp
HEADERS += masks.h \
	extmaskview.h \
	register_masks.h
FORMS += extmaskview.ui
