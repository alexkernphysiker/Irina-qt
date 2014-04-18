#include <fbkin.h>
#include "kinview.h"
#include "ruthview.h"
#include "splineview.h"

#define SOT_kinbase 0x0210
SObject * add2bkin(SortProject *pr, SObject *){
	return (new SoFBKin(pr));
}
SObject *loadkin(QDataStream &str,SortProject *father){
	return new SoFBKin(str,father);
}
SObject * addruth(SortProject *pr, SObject *){
	return (new SoRuth(pr));
}
SObject *loadruth(QDataStream &str,SortProject *father){
	return new SoRuth(str,father);
}
SObject * addsp1line(SortProject *pr, SObject *){
	return (new SoSP1Line(pr));
}
SObject *loadsp1line(QDataStream &str,SortProject *father){
	return new SoSP1Line(str,father);
}

#include "register_fbkin.h"
bool FBKinLoader::Register(){
	return RegisterSoType(new SoTypeReg("Binary Kinematic/2.4 (relativistic formula)",
								 "Binary kinematics formula", SoCatFormula,
								 1,SOT_kinbase,&loadkin, &add2bkin)) &&
	RegisterSoType(new SoTypeReg("",
								 "Rutherford formula", SoCatFormula,
								 2,SOT_kinbase, &loadruth, &addruth)) &&
	RegisterSoType(new SoTypeReg("",
								 "Binary: Spectrum line dN/dEx(Ex,Ep)", SoCatOther,
								 2,SOT_Binary, &loadsp1line, &addsp1line));
}
bool FBKinLoader::Unregister(){
	return UnregisterSoType(1,SOT_kinbase) &&
	UnregisterSoType(2,SOT_kinbase)&&
	UnregisterSoType(2,SOT_Binary);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(fbkin, FBKinLoader)
QT_END_NAMESPACE
