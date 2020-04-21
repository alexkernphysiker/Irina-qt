// this file is distributed under
// GPL v 3.0 license
#include "sbdi_theor.h"
#include "elatheorview.h"
#include <Core/Irina-qt-headers/SortLibGui.h>
//Functions that should be called from sortlib and sortlibgui
SObject *loadElaTh(QDataStream &str,SortProject *father){
	return new ElaTheor(str,father);
}
SObject * AddElaTh(SortProject *pr, SObject*){
	return (new ElaTheor(pr));
}
#include "register_sbdi.h"
bool SBDILoader::Register(){
	return RegisterSoType(new SoTypeReg(
					   "Sub-barrier deuterons interaction theory calculations (experimental)",
					   "Table: SBDI. Elastic/Ruth",
					   SoCatOther,
					   0x01,SOT_SBDI,&loadElaTh, &AddElaTh));
}
bool SBDILoader::Unregister(){
	return UnregisterSoType(0x01, SOT_SBDI);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(z_experimental_sbdi, SBDILoader)
QT_END_NAMESPACE
