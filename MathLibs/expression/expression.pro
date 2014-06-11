#author: alex_kernphysiker@privatdemail.net
include("../../qt_build_path.pri")
#project settings
QT       -= gui
TARGET = math_expression
TEMPLATE = lib
DEFINES += EXPRESSION_LIBRARY
SOURCES += expression.cpp
HEADERS += expression.h
