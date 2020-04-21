// this file is distributed under
// GPL v 3.0 license
#include <Core/Irina-qt-headers/SortLib.h>
#include <simulation.h>
#include "simulationview.h"
#include "randomview.h"
#include <QLabel>
#include "gaussview.h"
#include "mixedview.h"
#include "binsumview.h"

QString Caption_sim(){return "Monte Carlo/1.8";}

SObject *loadsim(QDataStream &str,SortProject *father){
	return new Simulation(str,father);
}
SObject * AddSim(SortProject *father, SObject *){
	return ( new Simulation(father));
}

SObject *loadrand(QDataStream &str,SortProject *father){
	return new RandomMagnitude(str,father);
}
SObject * AddRand(SortProject *father, SObject *){
	return ( new RandomMagnitude(father));
}
SObject *loadgauss(QDataStream &str,SortProject *father){
	return new GaussMagnitude(str,father);
}
SObject * AddGauss(SortProject *father, SObject *){
	return ( new GaussMagnitude(father));
}
SObject *loadmix(QDataStream &str,SortProject *father){
	return new MixDistributions(str,father);
}
SObject * AddMix(SortProject *father, SObject *){
	return ( new MixDistributions(father));
}
SObject *loadfuncdistr(QDataStream &str,SortProject *father){
	return new DistributedByFunction(str,father);
}
SObject * AddFuncDistr(SortProject *, SObject *so){
	if(so->Is(SOT_SoTblFunc)){
		return ( new DistributedByFunction((SoTblFunc*)so));
	}else
		MsgBox("Please select a table function");
	return NULL;
}

SObject * AddBinSumm(SortProject *father, SObject *){
	return ( new SoBinarySumm(father));
}
SObject *loadbinsumm(QDataStream &str,SortProject *father){
	return new SoBinarySumm(str,father);
}

#include "register_simulation.h"
bool SimulationLoader::Register(){
	return RegisterSoType(new SoTypeReg(Caption_sim()
								 ,"Monte Carlo Simulation", SoCatData,
								 0x04, SOT_DFReader,&loadsim, &AddSim)) &&
	RegisterSoType(new SoTypeReg(""
								 , "Random variable (uniform)", SoCatFormula,
								 SOT_RandomMagnitude, &loadrand, &AddRand)) &&
	RegisterSoType(new SoTypeReg(""
								 ,"Random variable (gauss)",SoCatFormula,
								 0x01,SOT_RandomMagnitude,&loadgauss, &AddGauss)) &&
	RegisterSoType(new SoTypeReg(""
								 ,"Random function selector",SoCatFormula,
								 0x02,SOT_RandomMagnitude,&loadmix, &AddMix)) &&
	RegisterSoType(new SoTypeReg(""
								 ,"Random variable with custom distribution",SoCatFormula,
								 0x04,SOT_RandomMagnitude,&loadfuncdistr, &AddFuncDistr)) &&
	RegisterSoType(new SoTypeReg(""
								 ,"Binary: summ of several binaries",SoCatOther,
								0x03,SOT_Binary,&loadbinsumm, &AddBinSumm));
}
bool SimulationLoader::Unregister(){
	return UnregisterSoType(0x04, SOT_DFReader) &&
	UnregisterSoType(SOT_RandomMagnitude) &&
	UnregisterSoType(0x01,SOT_RandomMagnitude) &&
	UnregisterSoType(0x02,SOT_RandomMagnitude) &&
	UnregisterSoType(0x04,SOT_RandomMagnitude) &&
	UnregisterSoType(0x03,SOT_Binary);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(simulation, SimulationLoader)
QT_END_NAMESPACE
