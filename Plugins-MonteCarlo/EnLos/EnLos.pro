#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
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
SOURCES += enlos.cpp enlosview.cpp enlossintargetview.cpp register_enlos.cpp
HEADERS += enlos.h enlosview.h enlossintargetview.h register_enlos.h
FORMS += enlosview.ui enlossintargetview.ui
