#ifndef REGISTER_SIMULATION_H
#define REGISTER_SIMULATION_H
#include <plugin_interface.h>
class SimulationLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual ~SimulationLoader(){}
	virtual bool Register();
	virtual bool Unregister();
};
#endif // REGISTER_SIMULATION_H
