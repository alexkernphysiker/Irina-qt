#author: alex_kernphysiker@privatdemail.net
include("../../../qt_build_path.pri")
#Irina-qt
INCLUDEPATH += ../Irina-qt-headers
INCLUDEPATH += ../..
LIBS += -L$$DESTDIR -lSortLib
#https://github.com/alexkernphysiker/MathLibs
INCLUDEPATH += ../../../MathLibs
LIBS += -L$$DEPENDPATH -lmath_func
LIBS += -L$$DEPENDPATH -lmath_expression
#project configuration
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = SortLibGui
TEMPLATE = lib
DEFINES += SORTLIBGUI_LIBRARY
HEADERS += ../Irina-qt-headers/SortLibGui.h \
	datafileview.h \
	valuegb.h \
	ecounterview.h \
	additional.h \
	filterview.h \
	formulaview.h \
	spaxisview.h \
	sp1view.h \
	sp1open.h \
	sp2view.h \
	locview.h \
	sp2open.h \
	dependecyview.h \
	groupdfview.h \
	ecalview.h \
	tblfuncview.h \
	txtview.h \
	sp1normview.h \
	sp1normerrview.h \
	tbltblview.h \
	expressionstringdialog.h
FORMS += sortprojectview.ui \
	datafileview.ui \
	valuegb.ui \
	ecounterview.ui \
	filterview.ui \
	formulaview.ui \
	spaxisview.ui \
	sp1view.ui \
	sp1open.ui \
	sp2view.ui \
	locview.ui \
	sp2open.ui \
	dependecyview.ui \
	groupdfview.ui \
	ecalview.ui \
	tblfuncview.ui \
	txtview.ui \
	sp1normview.ui \
	sp1normerrview.ui \
	tbltblview.ui \
	expressionstringdialog.ui
SOURCES += sortprojectview.cpp \
	datafileview.cpp \
	valuegb.cpp \
	ecounterview.cpp \
	filterview.cpp \
	formulaview.cpp \
	spaxisview.cpp \
	sp1view.cpp \
	sp1open.cpp \
	sp2view.cpp \
	locview.cpp \
	sp2open.cpp \
	dependecyview.cpp \
	groupdfview.cpp \
	ecalview.cpp \
	tblfuncview.cpp \
	txtview.cpp \
	sp1normview.cpp \
	sp1normerrview.cpp \
	tbltblview.cpp \
	expressionstringdialog.cpp
