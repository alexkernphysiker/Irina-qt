#/bin/bash
cd ..
mkdir bin 
echo DESTDIR = $PWD/bin $'\n'DEPENDPATH += \$\$DESTDIR $'\n'win32: DLLDESTDIR=\$\$DESTDIR $'\n'win32: DEFINES += WINBUILD $'\n'OBJECTS_DIR = \$\$DESTDIR/obj$'\n'QMAKE_CXXFLAGS+= -std=c++11\#if c++11 not supported comment this line and uncomment the next one$'\n'\#DEFINES += override= $'\n'unix:QMAKE_LFLAGS += -Wl,--rpath=\$\$DESTDIR $'\n'unix:QMAKE_LFLAGS_RPATH=$'\n' > qt_build_path.pri
cd Irina-qt