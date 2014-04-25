include("../../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(errcalc)
SOURCES += numericerrorscalculation.cpp thiswidget.cpp register_errcalc.cpp
HEADERS += numericerrorscalculation.h thiswidget.h register_errcalc.h
FORMS += thiswidget.ui
