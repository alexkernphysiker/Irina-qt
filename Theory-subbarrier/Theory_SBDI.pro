include("../Irina-qt/qt_build_path.pri")
INCLUDEPATH += ../Irina-qt
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(theory_sbdi)
SOURCES += sbdi_theor.cpp elatheorview.cpp register_sbdi.cpp
HEADERS += sbdi_theor.h elatheorview.h register_sbdi.h
FORMS += elatheorview.ui
