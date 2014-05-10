#/bin/bash
cd ..
cd MathLibs
./buildme.sh
cd ..
cd Irina-qt
cd Core
cd SortLib
qmake SortLib.pro
make
cd ..
cd SortLibGui
qmake SortLibGui.pro
make
cd ..
cd Irina-qt-executable
qmake irina-qt-executable.pro
make
cd ..
cd Irina-qt-devtool
qmake Irina-qt-devtool.pro
make
cd ..
cd ..
cd DataOutput
cd output-gnuplot
qmake output-gnuplot.pro
make
cd ..
cd ..
cd Plugins-DataInput
cd format_text
qmake format_text.pro
make
cd ..
cd formats_KINR
qmake formats_KINR.pro
make
cd ..
cd ..
cd Plugins-General
cd DataGroup
qmake DataGroup.pro
make
cd ..
cd FBKin
qmake FBKin.pro
make
cd ..
cd Masks
qmake Masks.pro
make
cd ..
cd NumericErrorsCalculation
qmake NumericErrorsCalculation.pro
make
cd ..
cd ..
cd Plugins-MonteCarlo
cd Simulation
qmake Simulation.pro
make
cd ..
cd EnLos
qmake EnLos.pro
make
cd ..
cd ..