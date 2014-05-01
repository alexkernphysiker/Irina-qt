#ifndef REGISTER_MASKS_H
#define REGISTER_MASKS_H
#include <plugin_interface.h>
class MasksLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register()override;
	virtual bool Unregister()override;
};
#endif // REGISTER_MASKS_H
