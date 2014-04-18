#ifndef REGISTER_TEXTFILE_H
#define REGISTER_TEXTFILE_H
#include <plugin_interface.h>
class TextLibLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual bool Register();
	virtual bool Unregister();
};
#endif // REGISTER_TEXTFILE_H
