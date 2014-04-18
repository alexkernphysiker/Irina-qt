#ifndef REGISTERROOTPLOTTER_H
#define REGISTERROOTPLOTTER_H
#include <plugin_output_interface.h>
class RootOutputLoader: public QObject, public OutputPluginLoaderInterface{
	Q_OBJECT
	Q_INTERFACES(OutputPluginLoaderInterface)
public:
	virtual IDataOutput* DataOutput();
};
#endif // REGISTERROOTPLOTTER_H
