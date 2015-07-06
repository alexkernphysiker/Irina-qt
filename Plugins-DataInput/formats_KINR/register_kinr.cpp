// this file is distributed under
// GPL v 3.0 license
#include "KINR.h"
#include <QFileDialog>

// these functions are called by SortLib when
// KINR datafile readers are creating when a project
// containing them is opened from file
SObject *loadd00v(QDataStream &str,SortProject *father){
	return new SoD00V(str,father);
}
SObject *loadd00z(QDataStream &str,SortProject *father){
	return new SoD00Z(str,father);
}

// these functions are called by SortLibGui when
// new KINR datafile readers are added to the project
SObject * AddD00V(SortProject *father, SObject*){
	QString str=QFileDialog::getOpenFileName(NULL,"Choose text file",QDir::currentPath(),"*.*",NULL,	QFileDialog::DontResolveSymlinks);
	str=QDir::current().relativeFilePath(str);
	if((str!=NULL)&(str!=""))
		return ( new SoD00V(str,father));
	return NULL;
}
SObject * AddD00Z(SortProject *father, SObject*){
	QString str=QFileDialog::getOpenFileName(NULL,"Choose text file",QDir::currentPath(),"*.*",NULL,	QFileDialog::DontResolveSymlinks);
	str=QDir::current().relativeFilePath(str);
	if((str!=NULL)&(str!=""))
		return ( new SoD00Z(str,father));
	return NULL;
}
#include "register_kinr.h"
bool KINRLoader::Register(){
	return RegisterSoType(new SoTypeReg("Data format/KINR/1.4","KINR D00V",
								 SoCatData,0x01,SOT_KINR,&loadd00v,&AddD00V)) &&
	RegisterSoType(new SoTypeReg("","KINR D00Z",
								 SoCatData,0x02,SOT_KINR,&loadd00z,&AddD00Z));
}
bool KINRLoader::Unregister(){
	return UnregisterSoType(0x01, SOT_KINR) &&
	UnregisterSoType(0x02, SOT_KINR);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(formats_kinr, KINRLoader)
QT_END_NAMESPACE

