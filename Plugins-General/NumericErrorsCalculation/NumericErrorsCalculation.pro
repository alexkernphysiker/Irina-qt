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
TARGET   = $$qtLibraryTarget(errcalc)
SOURCES += numericerrorscalculation.cpp \
	thiswidget.cpp \
	register_errcalc.cpp
HEADERS += numericerrorscalculation.h \
	thiswidget.h \
	register_errcalc.h
FORMS += thiswidget.ui
