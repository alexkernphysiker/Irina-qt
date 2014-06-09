include("../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
#project configuration
TARGET = output-gnuplot
TEMPLATE = lib
CONFIG += plugin
TARGET   = $$qtLibraryTarget(output-gnuplot)
SOURCES += gnuplotter.cpp plotterview.cpp settingswindow.cpp register-gnuplot-ext.cpp
HEADERS +=gnuplotter.h plotterview.h settingswindow.h register-gnuplot-ext.h
FORMS += plotterview.ui settingswindow.ui
