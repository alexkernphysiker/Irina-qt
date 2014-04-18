#ifndef REGISTER_ERRCALC_H
#define REGISTER_ERRCALC_H
#include <plugin_interface.h>
class ErrCalcLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register();
	virtual bool Unregister();
};
#endif // REGISTER_ERRCALC_H
