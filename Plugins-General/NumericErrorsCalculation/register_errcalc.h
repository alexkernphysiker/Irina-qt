#ifndef REGISTER_ERRCALC_H
#define REGISTER_ERRCALC_H
#include <plugin_interface.h>
class ErrCalcLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register()override;
	virtual bool Unregister()override;
};
#endif // REGISTER_ERRCALC_H
