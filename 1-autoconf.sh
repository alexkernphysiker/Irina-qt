#/bin/bash
cd ..
mkdir bin 
echo DESTDIR = $PWD/bin $'\n'DEPENDPATH += \$\$DESTDIR $'\n'win32: DLLDESTDIR=\$\$DESTDIR $'\n'OBJECTS_DIR = \$\$DESTDIR/obj$'\n'QMAKE_CXXFLAGS+= -std=c++11$'\n'QMAKE_LFLAGS += -Wl,--rpath=\$\$DESTDIR > qt_build_path.pri
cd Irina-qt