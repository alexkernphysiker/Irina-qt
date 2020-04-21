include("../../Irina-qt/qt_build_path.pri")
INCLUDEPATH += ../../Irina-qt
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
LIBS += -L$$DESTDIR -lSortLibGui
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
