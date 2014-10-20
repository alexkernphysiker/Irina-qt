#/bin/bash
(cd MathLibs/expression;qmake expression.pro;make)
if (( $? )); then exit 1;fi
(cd Core/SortLib;qmake SortLib.pro;make)
if (( $? )); then exit 1;fi
(cd Core/SortLibGui;qmake SortLibGui.pro;make)
if (( $? )); then exit 1;fi
(cd Core/Irina-qt-executable;qmake irina-qt-executable.pro;make)
if (( $? )); then exit 1;fi
(cd DataOutput/output-gnuplot;qmake output-gnuplot.pro;make)
if (( $? )); then exit 1;fi
(cd Plugins-DataInput/format_text;qmake format_text.pro;make)
if (( $? )); then exit 1;fi
cd Plugins-DataInput/formats_KINR;qmake formats_KINR.pro;make)
if (( $? )); then exit 1;fi
(cd Plugins-General/DataGroup;qmake DataGroup.pro;make)
if (( $? )); then exit 1;fi
cd Plugins-General/FBKin;qmake FBKin.pro;make)
if (( $? )); then exit 1;fi
(cd Plugins-General/Masks;qmake Masks.pro;make)
if (( $? )); then exit 1;fi
(cd Plugins-General/NumericErrorsCalculation;qmake NumericErrorsCalculation.pro;make)
if (( $? )); then exit 1;fi
(cd Plugins-MonteCarlo/Simulation;qmake Simulation.pro;make)
if (( $? )); then exit 1;fi
(cd Plugins-MonteCarlo/EnLos;qmake EnLos.pro;make)
if (( $? )); then exit 1;fi
