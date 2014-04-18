#ifndef REGISTER_ENLOS_H
#define REGISTER_ENLOS_H
#include <plugin_interface.h>
class EnLosLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register();
	virtual bool Unregister();
};
#endif // REGISTER_ENLOS_H
