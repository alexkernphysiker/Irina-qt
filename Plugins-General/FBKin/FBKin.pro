include("../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#MathLibs
LIBS += -L$$DEPENDPATH -lmath_expression
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(fbkin)
HEADERS += fbkin.h kinview.h ruthview.h splineview.h register_fbkin.h
SOURCES += fbkin.cpp kinview.cpp ruthview.cpp splineview.cpp register_fbkin.cpp
FORMS += kinview.ui ruthview.ui splineview.ui
