#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../../Core/Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
#project configuration
TARGET = output-gnuplot
TEMPLATE = lib
CONFIG += plugin
TARGET   = $$qtLibraryTarget(output-gnuplot)
SOURCES += gnuplotter.cpp \
	plotterview.cpp \
	settingswindow.cpp \
	register-gnuplot-ext.cpp
HEADERS +=gnuplotter.h \
	plotterview.h \
	settingswindow.h \
	register-gnuplot-ext.h
FORMS += plotterview.ui \
	settingswindow.ui
