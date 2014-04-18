#include <masks.h>
#include "extmaskview.h"


SObject *loadexmask(QDataStream &str,SortProject *father){
	return new ExtendedMask(str,father);
}
SObject * addexmask(SortProject *pr, SObject *){
	return (new ExtendedMask(pr));
}
#include "register_masks.h"
bool MasksLoader::Register(){
	return RegisterSoType(new SoTypeReg("Extended event filter/0.3","Predicate",
								 SoCatFilter,4,SOT_EFilter,&loadexmask, &addexmask));
}
bool MasksLoader::Unregister(){
	return UnregisterSoType(4,SOT_EFilter);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(masks, MasksLoader)
QT_END_NAMESPACE
