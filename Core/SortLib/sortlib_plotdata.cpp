#include <SortLib.h>

SoDependency::SoDependency(SortProject *father):SObject(father){
	AddType(SOT_Dependency);
	m_analyse=QList<SoDFReader*>();
	m_argument="";
	m_value="";
	m_xerror="";
	m_yerror="";
}
SoDependency::SoDependency(QDataStream &str, SortProject *father):SObject(str,father){
	AddType(SOT_Dependency);
	m_analyse=QList<SoDFReader*>();
	m_argument=readstr(str);
	m_value=readstr(str);
	m_xerror=readstr(str);
	m_yerror=readstr(str);
	int dn=-1;
	do{
		str.readRawData((char*)&dn,sizeof(int));
		if(dn>=0){
			AddDF((SoDFReader*)Owner()->at(dn));
		}
	}while(dn>=0);
}
void SoDependency::Save(QDataStream &str){
	SObject::Save(str);
	writestr(m_argument,str);
	writestr(m_value,str);
	writestr(m_xerror,str);
	writestr(m_yerror,str);
	for(int i=0; i<DFCnt();i++){
		int dn=Owner()->Number(GetDF(i));
		str.writeRawData((char*)&dn,sizeof(int));
	}{
		int dn=-1;
		str.writeRawData((char*)&dn,sizeof(int));
	}
}
SoDependency::~SoDependency(){}
QString SoDependency::Argument(){return m_argument;}
QString SoDependency::Value(){return m_value;}
QString SoDependency::XError(){return m_xerror;}
QString SoDependency::YError(){return m_yerror;}
void SoDependency::SetArgument(QString str){m_argument=str;changed(this);}
void SoDependency::SetValue(QString str){m_value=str;changed(this);}
void SoDependency::SetXError(QString str){m_xerror=str;changed(this);}
void SoDependency::SetYError(QString str){m_yerror=str;changed(this);}
int SoDependency::DFCnt(){return m_analyse.count();}
SoDFReader *SoDependency::GetDF(int N){
	if((N<0)|(N>=m_analyse.count()))return NULL;
	return m_analyse.at(N);
}
void SoDependency::AddDF(SoDFReader *source){
	if(!m_analyse.contains(source))
		m_analyse.append(source);
	connect(source, SIGNAL(deleting(SObject*)), this, SLOT(dr_deleting(SObject*)));
	changed(this);
}
void SoDependency::RemDF(SoDFReader *source){
	if(m_analyse.contains(source))
		m_analyse.removeOne(source);
	disconnect(source, SIGNAL(deleting(SObject*)), this, SLOT(dr_deleting(SObject*)));
	changed(this);
}
void SoDependency::dr_deleting(SObject *so){
	if(so->Is(SOT_DFReader)){
		RemDF((SoDFReader*)so);
	}
}
QString SoDependency::DisplayName(){return "PLOTDATA "+Name();}
void SoDependency::Export(){
	out_1d(Name());
	ExportTo();
	out_end();
}

void SoDependency::exprerr(QString msg){
	error(DisplayName()+" "+msg);
}
void SoDependency::ExportTo(){
	for(int i=0; i<DFCnt();i++){
		SoDFReader *source=GetDF(i);
		double arg=0;
		Owner()->GetVar(m_argument,&arg,source,NULL);
		double val=0;
		Owner()->GetVar(m_value,&val,source,NULL);
		double xerr=0;
		Owner()->GetVar(m_xerror,&xerr,source,NULL);
		double yerr=0;
		Owner()->GetVar(m_yerror,&yerr,source,NULL);
		DataEvent *item=new DataEvent(4);
		item->Set_ADC(0,arg);
		item->Set_ADC(1,val);
		item->Set_ADC(2,xerr);
		item->Set_ADC(3,yerr);
		item->Freeze();
		out_dataitem(item);
	}
}


SoDSewed::SoDSewed(SortProject *father):SoDependency(father){
	AddType(1); m_sewwith="";
}
SoDSewed::SoDSewed(QDataStream &str, SortProject *father):SoDependency(str,father){
	AddType(1);m_sewwith=readstr(str);
}
void SoDSewed::Save(QDataStream &str){
	SoDependency::Save(str);
	writestr(m_sewwith,str);
}
SoDSewed::~SoDSewed(){}
QString SoDSewed::DisplayName(){return "xPLOTDATA "+Name();}
QString SoDSewed::Sewed(){return m_sewwith;}
void SoDSewed::SetSewed(QString str){m_sewwith=str;changed(this);}
void SoDSewed::Export(){
	out_1d(Name());
	{
		ExportTo();
		for(int i=0; i<Owner()->Count();i++){
			SObject *so=Owner()->at(i);
			if(so->Is(SOT_Dependency)){
				SoDependency *dep=(SoDependency*)so;
				if(dep->Name().compare(m_sewwith,Qt::CaseInsensitive)==0)
					dep->ExportTo();
			}
		}
	}
	out_end();
}

