// this file is distributed under
// GPL v 3.0 license
#ifndef REGISTER_ENLOS_H
#define REGISTER_ENLOS_H
#include <Core/Irina-qt-headers/plugin_interface.h>
class EnLosLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register()override;
	virtual bool Unregister()override;
};
#endif // REGISTER_ENLOS_H
