#include <SortLib.h>
#include "newtype.h"

SObject::SObject(SortProject *father){m_master=father;m_type=0;m_name="";}
SObject::SObject(QDataStream &str, SortProject *father){
	m_master=father;m_type=0;
	m_name=readstr(str);
}
SortProject *SObject::Owner(){return m_master;}
uint SObject::SType(){return m_type;}
bool SObject::Is(uint type){
	if(type==0)return true;
	uint mask=0xff;
	bool res=true;
	uint index=0;
	while(((type&mask)!=0)&(index<sizeof(uint))){
		uint flag=type&mask;
		res&=flag==(m_type&mask);
		mask=(mask<<8);index++;
	}
	return res;
}
bool SObject::IsJust(uint type){return m_type==type;}
void SObject::Save(QDataStream &str){
	str.writeRawData((char*)&m_type, sizeof(uint));
	writestr(m_name,str);
}
void SObject::LoadCache(QDataStream &){}
void SObject::SaveCache(QDataStream &){}
void *SObject::GetForm(){return NULL;}
void SObject::AddType(uchar subtype){
	m_type=newtype(subtype,m_type);
}
QString SObject::Name(){return m_name;}
QString SObject::DisplayName(){return m_name;}
SObject *SObject::DisplParrent(){return NULL;}
void SObject::Set_Name(QString name){m_name=name.replace(" ","_");changed(this);}
void SObject::BeforeDelete(){deleting();deleting(this);}
SObject::~SObject(){}
void SObject::remove(){
	m_master->Remove(this);
}
