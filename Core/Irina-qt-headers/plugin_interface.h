#ifndef ___PLUGIN__
#define ___PLUGIN__
#include <QtPlugin>
/******************************************
		Current plugin interface version
 ******************************************/
#define PLUGIN_INTERFACE "irina-qt/plugin/2.3"
QT_BEGIN_NAMESPACE
class QObject;
QT_END_NAMESPACE
/*******************************
	PLUGIN INTERFACE CLASS
 *******************************/
class LoaderInterface{// Plugin interface for additional SObject types
public:
	virtual ~LoaderInterface(){}
	virtual bool Register()=0;// registers all SObject types implemented in plugin with SortLib functions
	virtual bool Unregister()=0;// unregisters all SObject types implemented in plugin with SortLib functions
};
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(LoaderInterface,PLUGIN_INTERFACE)
QT_END_NAMESPACE
#endif
