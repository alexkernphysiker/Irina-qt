Irina-qt
===========
This is the program for analysis of data obtained in multiparameter
nuclear measurements either inclusive or measurements with coincidents.
Provides interface for dynamically loaded plugins.

Core/ - the description and the source code of core modules and executable.

DataOutput/ - the source code of modules for data output

Plugins-DataInput - the source code of modules for providing different
      formats of input data.

Plugins-General/ - the description and the source code of plugins
      for general purpose.

Build requires libqt and libqt-devel packages installed on your system.
Data output plugin uses gnuplot, so it should also be installed.
The configuration and build can be done automatically.
Run script "autoconf.sh" for creating directory for binaries
and file qt_build_path.pri required for all qmake projects here.
Edit this file if needed (View it to see if it's needed!!!!)
Then you may run "buildme.sh" for building 
and the binaries should appear in ../bin directory
