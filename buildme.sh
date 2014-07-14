#/bin/bash
cd MathLibs
cd expression
qmake expression.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd ..
cd Core
cd SortLib
qmake SortLib.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd SortLibGui
qmake SortLibGui.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd Irina-qt-executable
qmake irina-qt-executable.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd ..
cd DataOutput
cd output-gnuplot
qmake output-gnuplot.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd ..
cd Plugins-DataInput
cd format_text
qmake format_text.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd formats_KINR
qmake formats_KINR.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd ..
cd Plugins-General
cd DataGroup
qmake DataGroup.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd FBKin
qmake FBKin.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd Masks
qmake Masks.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd NumericErrorsCalculation
qmake NumericErrorsCalculation.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd ..
cd Plugins-MonteCarlo
cd Simulation
qmake Simulation.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd EnLos
qmake EnLos.pro
if (( $? )); then exit 1;fi
make
if (( $? )); then exit 1;fi
cd ..
cd ..
