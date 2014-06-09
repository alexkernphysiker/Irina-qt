include("../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(eefilter)
SOURCES += masks.cpp extmaskview.cpp register_masks.cpp
HEADERS += masks.h extmaskview.h register_masks.h
FORMS += extmaskview.ui
