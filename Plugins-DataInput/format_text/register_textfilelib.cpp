#include "textfile.h"
#include <QFileDialog>

// this function is called by SortLib when a text datafile reader is creating when a project
// containing it is opened from file
SObject *loadtxt(QDataStream &str,SortProject *father){
	return new TxtData(str,father);
}

// this function is called by SortLibGui when a new text datafile reader is added to the project
SObject * AddTxtData(SortProject *father, SObject*){
	QString str=QFileDialog::getOpenFileName(NULL,"Choose text file",QDir::currentPath(),"*.*",NULL,	QFileDialog::DontResolveSymlinks);
	str=QDir::current().relativeFilePath(str);
	if((str!=NULL)&(str!=""))
		return ( new TxtData(str,father));
	return NULL;
}

#include "register_textfilelib.h"
bool TextLibLoader::Register(){
	return RegisterSoType(new SoTypeReg("Data format/Text/0.2"
								 ,"Data from multicolumn text file",SoCatData,0x02, SOT_DFReader,&loadtxt,&AddTxtData));
}

bool TextLibLoader::Unregister(){
	return UnregisterSoType(0x02, SOT_DFReader);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(format_txt, TextLibLoader)
QT_END_NAMESPACE

