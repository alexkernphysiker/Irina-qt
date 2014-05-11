#include <SortLib.h>
#include "sortlib_.h"
#include "newtype.h"
#include <QList>

QString sortlibversion(){
	return "4.0";
}

QList<SoTypeReg*> registered_types;

int RegisteredTypesCount(){
	return registered_types.count();
}

SoTypeReg* GetRegisteredType(int index){
	if(index<0)return NULL;
	if(index>=registered_types.count())return NULL;
	return registered_types.at(index);
}

bool RegisterSoType(SoTypeReg* type_){
	uint id=type_->Signature();
	bool exists=false;
	for(int i=0; (i<registered_types.count())&&(!exists);i++)
		exists|=(id==registered_types.at(i)->Signature());
	if(!exists){
		registered_types.append(type_);
		return true;
	}
	return false;
}
bool UnregisterSoType(uint m_sig){
	bool done=false;
	for(int i=0; i<registered_types.count(); i++){
		SoTypeReg* reg=registered_types.at(i);
		if(reg->Signature()==m_sig){
			registered_types.removeAt(i);
			i--; done=true;
		}
	}
	return done;
}
QString aboutsortlibcaption(){
	return " Irina-qt/core/"+sortlibversion();
}
QString aboutsortlibmsg(){
	QString res = "Software version:\n   "+aboutsortlibcaption()+" \nAnalysis extensions:";
	for(int i=0; i<registered_types.count(); i++){
		QString reg=registered_types.at(i)->About();
		if(reg!="")
			res=res+"\n   "+reg;
	}
	return res;
}

uint newtype(uchar subtype, uint m_type){
	uint mask=0xff;
	uint index=0;bool set=false;
	while(!set&(index<sizeof(uint))){
		if((m_type&(mask<<(index<<3)))==0){
			uint st=subtype;
			st=st<<(index*8);
			m_type|=st;
			set=true;
		}
		index++;
	}
	return m_type;
}

bool UnregisterSoType(uint addedsig, uint basesig){
	return UnregisterSoType(newtype(addedsig,basesig));
}

SoTypeReg::SoTypeReg(QString about_, QString caption_, SoTypeCategory cat_, uint addedsig, uint basesig, SOTypeReader read_, SOTypeCreator create_){
	m_about=about_;
	m_caption=caption_;
	m_cat=cat_;
	m_sig=newtype(addedsig,basesig);
	m_read=read_;
	m_create=create_;
}
SoTypeReg::SoTypeReg(QString about_, QString caption_, SoTypeCategory cat_, uint sig, SOTypeReader read_, SOTypeCreator create_){
	m_about=about_;
	m_caption=caption_;
	m_cat=cat_;
	m_sig=sig;
	m_read=read_;
	m_create=create_;
}
SoTypeReg::~SoTypeReg(){}
QString SoTypeReg::About(){return m_about;}
QString SoTypeReg::Caption(){return m_caption;}
SoTypeCategory SoTypeReg::Category(){return m_cat;}
uint SoTypeReg::Signature(){return m_sig;}
SObject* SoTypeReg::createInstance(SortProject *parentproj, SObject *selectedobject){
	return m_create(parentproj,selectedobject);
}
SObject* SoTypeReg::readInstance(QDataStream &str, SortProject *father){
	return m_read(str,father);
}

