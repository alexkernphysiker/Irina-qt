// this file is distributed under
// GPL v 3.0 license
#include "gnuplotter.h"
#include <SortLibGui.h>
#include "register-gnuplot-ext.h"
IDataOutput* GnuplotExtLoader::DataOutput(){
	return new Gnuplotter();
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(output-gnuplot, GnuplotExtLoader)
QT_END_NAMESPACE
