#include "outputroot.h"
#include <SortLibGui.h>
#include "register-root-output.h"
IDataOutput* RootOutputLoader::DataOutput(){
	return new RootOutput();
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(output-root, RootOutputLoader)
QT_END_NAMESPACE

