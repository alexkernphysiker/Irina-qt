#ifndef REGISTER_GnuplotExtLoader_H
#define REGISTER_GnuplotExtLoader_H
#include <plugin_output_interface.h>
class GnuplotExtLoader: public QObject, public OutputPluginLoaderInterface{
	Q_OBJECT
	Q_INTERFACES(OutputPluginLoaderInterface)
public:
virtual IDataOutput* DataOutput();
};
#endif // REGISTER_GnuplotExtLoader_H
