// this file is distributed under
// GPL v 3.0 license
#ifndef REGISTER_SIMULATION_H
#define REGISTER_SIMULATION_H
#include <Core/Irina-qt-headers/plugin_interface.h>
class SimulationLoader: public QObject, public LoaderInterface{
	Q_OBJECT
	Q_INTERFACES(LoaderInterface)
public:
	virtual ~SimulationLoader(){}
	virtual bool Register()override;
	virtual bool Unregister()override;
};
#endif // REGISTER_SIMULATION_H
