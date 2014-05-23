#include <enlos.h>
#include "enlosview.h"
#include "enlossintargetview.h"

#define SOT_EnLos_gl 0x1010
SObject *loadenloss(QDataStream &str, SortProject *proj){
	return new EnLos(str,proj);
}
SObject *loadenlossintarget(QDataStream &str, SortProject *proj){
	return new EnLossInTarget(str,proj);
}
SObject * addenloss(SortProject *, SObject *so){
	if(so!=NULL){
		if(so->Is(SOT_SoTblFunc)){
			return (new EnLos((SoTblFunc*)so));
		}
	}
	MsgBox("Please select a table function with stopping power");
	return NULL;
}
SObject * addenlossintarget(SortProject *pr, SObject *){
	return (new EnLossInTarget(pr));
}

#include "register_enlos.h"
bool EnLosLoader::Register(){
	return RegisterSoType(new SoTypeReg("Energy loss/1.6 (Charged particle energy loss calculation)",
								 "Charged particle energy loss", SoCatFormula,
								 0x01,SOT_EnLos_gl,&loadenloss, &addenloss)) &&
	RegisterSoType(new SoTypeReg("","Calculator of energy loss in the target", SoCatFormula,
								 0x02,SOT_EnLos_gl,&loadenlossintarget,&addenlossintarget));
}
bool EnLosLoader::Unregister(){
	return UnregisterSoType(0x01,SOT_EnLos_gl) &&
	UnregisterSoType(0x02,SOT_EnLos_gl);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(target_enlos, EnLosLoader)
QT_END_NAMESPACE
