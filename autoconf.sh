#/bin/bash
git submodule init
git submodule update
cd MathLibs
./autoconf.sh
cd ..