include("../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#MathLibs
LIBS += -L$$DEPENDPATH -lmath_expression
#project configuration
QT       -= gui
TARGET = SortLib
TEMPLATE = lib
DEFINES += SORTLIB_LIBRARY
SOURCES += sortlib_integration.cpp sortlib_sobject.cpp sortlib_datafile.cpp \
	sortlib_counters.cpp sortlib_filters.cpp sortlib_functions.cpp \
	sortlib_operators.cpp sortlib_plotdata.cpp sortlib_sortproject.cpp
HEADERS += ../Irina-qt-headers/SortLib.h newtype.h sortlib_.h
