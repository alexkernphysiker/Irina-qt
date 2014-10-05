#/bin/bash
mkdir bin 
echo DESTDIR = $PWD/bin$'\n'DEPENDPATH += \$\$DESTDIR $'\n'QMAKE_CXXFLAGS+= -std=c++11$'\n'QMAKE_LFLAGS += -Wl,--rpath=\$\$DESTDIR > qt_build_path.pri
