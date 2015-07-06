// this file is distributed under
// GPL v 3.0 license
#ifndef REGISTER_GnuplotExtLoader_H
#define REGISTER_GnuplotExtLoader_H
#include <plugin_output_interface.h>
class GnuplotExtLoader: public QObject, public OutputPluginLoaderInterface{
	Q_OBJECT
	Q_INTERFACES(OutputPluginLoaderInterface)
public:
virtual IDataOutput* DataOutput()override;
};
#endif // REGISTER_GnuplotExtLoader_H
