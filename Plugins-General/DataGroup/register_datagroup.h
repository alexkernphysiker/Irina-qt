// this file is distributed under
// GPL v 3.0 license
#ifndef REGISTER_TEXTFILE_H
#define REGISTER_TEXTFILE_H
#include <plugin_interface.h>
class DataGroupLibLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register()override;
	virtual bool Unregister()override;
};
#endif // REGISTER_TEXTFILE_H
