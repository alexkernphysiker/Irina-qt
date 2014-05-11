include("../../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
LIBS += -L$$DESTDIR -lmontecarlo
LIBS += -L$$DESTDIR -lmath_func
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(montecarlo_enlos)
SOURCES += enlos.cpp enlosview.cpp enlossintargetview.cpp register_enlos.cpp
HEADERS += enlos.h enlosview.h enlossintargetview.h register_enlos.h
FORMS += enlosview.ui enlossintargetview.ui
