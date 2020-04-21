include("../../Irina-qt/qt_build_path.pri")
INCLUDEPATH += ../../Irina-qt
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(montecarlo_enlos)
SOURCES += enlos.cpp enlosview.cpp enlossintargetview.cpp register_enlos.cpp
HEADERS += enlos.h enlosview.h enlossintargetview.h register_enlos.h
FORMS += enlosview.ui enlossintargetview.ui
