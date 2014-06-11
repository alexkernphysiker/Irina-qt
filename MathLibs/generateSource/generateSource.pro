#author: alex_kernphysiker@privatdemail.net
include("../../qt_build_path.pri")
#project settings
QT       -= gui
TARGET = generateSource
TEMPLATE = lib
DEFINES += GEN_SOURCE_LIBRARY
SOURCES += GenerateSource.cpp
HEADERS += GenerateSource.h
