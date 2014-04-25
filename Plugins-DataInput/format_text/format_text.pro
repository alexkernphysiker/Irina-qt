include("../../../qt_build_path.pri")
include("../../qt_inc_paths.pri")
#Irina-qt
LIBS += -L$$DESTDIR -lSortLib
#project configuration
QT       += core gui
CONFIG += plugin
TEMPLATE = lib
TARGET   = $$qtLibraryTarget(format_txt)
SOURCES += textfile.cpp register_textfilelib.cpp
HEADERS += textfile.h register_textfilelib.h
