#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../../Core/Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
#https://github.com/alexkernphysiker/MathLibs
INCLUDEPATH += ../../../MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
#project configuration
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(montecarlo)
DEFINES += SIMULATION_LIBRARY
SOURCES += simulation.cpp simulationview.cpp randomview.cpp gaussview.cpp \
	mixedview.cpp binsumview.cpp register_simulation.cpp
HEADERS += simulation.h simulationview.h randomview.h gaussview.h \
	mixedview.h binsumview.h register_simulation.h
FORMS += simulationview.ui randomview.ui gaussview.ui mixedview.ui binsumview.ui
