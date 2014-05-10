#/bin/bash
cd ..
mkdir bin 
echo DESTDIR = $PWD/bin $'\n'DEPENDPATH += \$\$DESTDIR $'\n'win32: DLLDESTDIR=\$\$DESTDIR $'\n'OBJECTS_DIR = \$\$DESTDIR/obj$'\n'QMAKE_CXXFLAGS+= -std=c++11\#if c++11 not supported comment this line and uncomment the next one$'\n'\#DEFINES += override= $'\n'QMAKE_LFLAGS += -Wl,--rpath=\$\$DESTDIR > qt_build_path.pri
cd Irina-qt