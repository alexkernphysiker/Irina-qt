#ifndef ___PLUGIN__PLOTTER
#define ___PLUGIN__PLOTTER
#include <QtPlugin>
/******************************************
		Current plugin interface version
 ******************************************/
#define OUTPUT_PLUGIN_INTERFACE "irina-qt/output/2.0"
QT_BEGIN_NAMESPACE
class QObject;
class IDataOutput;
QT_END_NAMESPACE
/*******************************
	PLUGIN INTERFACE CLASS
 *******************************/
class OutputPluginLoaderInterface{//Plugin interface for data output extension
public:
	virtual ~OutputPluginLoaderInterface(){}
	virtual IDataOutput* DataOutput()=0;// returns the instance of data output extension class
};
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(OutputPluginLoaderInterface,OUTPUT_PLUGIN_INTERFACE)
QT_END_NAMESPACE
#endif// ___PLUGIN__PLOTTER
