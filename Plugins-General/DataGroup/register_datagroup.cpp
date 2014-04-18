#include "datagroup.h"
#include <QFileDialog>

// this function is called by SortLib when a text datafile reader is creating when a project
// containing it is opened from file
SObject *loadtxt(QDataStream &str,SortProject *father){
	return new DataGroup(str,father);
}
// this function is called by SortLibGui when a new text datafile reader is added to the project
SObject *AddTxtData(SortProject *father, SObject*){
	return  new DataGroup(father);
}

#include "register_datagroup.h"
bool DataGroupLibLoader::Register(){
	return RegisterSoType(new SoTypeReg("Group of datafiles/0.1"
								 ,"Group of datafiles",SoCatData,SOT_DataGroup,&loadtxt,&AddTxtData));
}

bool DataGroupLibLoader::Unregister(){
	return UnregisterSoType(SOT_DataGroup);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(format_datagroup, DataGroupLibLoader)
QT_END_NAMESPACE

