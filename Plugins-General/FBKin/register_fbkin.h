#ifndef REGISTER_FBKIN_H
#define REGISTER_FBKIN_H
#include <plugin_interface.h>
class FBKinLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register()override;
	virtual bool Unregister()override;
};
#endif // REGISTER_FBKIN_H
