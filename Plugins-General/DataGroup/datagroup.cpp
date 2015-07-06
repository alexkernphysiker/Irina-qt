// this file is distributed under
// GPL v 3.0 license
#include "datagroup.h"
#include "datagroupview.h"
#include <SortLibGui.h>

DataGroup::DataGroup(SortProject *father):SoDFReader("",father){
	AddType( 3);Set_NADC(0);
	m_dgtype=DGSumm;
}
DataGroup::DataGroup(QDataStream &str, SortProject *father):SoDFReader(str,father){
	AddType(3);Set_NADC(0);
	str.readRawData((char*)&m_dgtype,sizeof(DataGroupType));
	uint n=0;
	str.readRawData((char*)&n,sizeof(uint));
	for(uint i=0; i<n;i++){
		SObject* so=ReadObj(str,Owner());
		if(so!=NULL){
			if(so->Is(SOT_DFReader)){
				m_list.append((SoDFReader*)so);
			}else{
				delete so;
			}
		}
	}
}
void DataGroup::Save(QDataStream &str){
	SoDFReader::Save(str);
	str.writeRawData((char*)&m_dgtype,sizeof(DataGroupType));
	uint n=m_list.count();
	str.writeRawData((char*)&n, sizeof(uint));
	for(uint i=0; i<n;i++){
		m_list[i]->Save(str);
	}
}

DataGroup::~DataGroup(){
	for(int i=0; i< m_list.count();i++)
		delete m_list[i];
}

QString DataGroup::DisplayName(){return "FILE::GROUP "+Name();}
void *DataGroup::GetForm(){
	QWidget *contr=new DataGroupView(this);
	DataFileView *form=new DataFileView(this);
	form->AddTab("Group of DataFiles",contr);
	return (void *)(form);
}
DataEvent* DataGroup::NextEvent(QDataStream &){return NULL;}//Should be implemented if grouped readers

int DataGroup::GetDFReaderCount(){return m_list.count();}
SObject* DataGroup::GetDFReader(int index){
	if(index<0)return NULL;
	if(index>=m_list.count())return NULL;
	return m_list[index];
}

void DataGroup::RemDFReader(int index){
	if(index<0)return;
	if(index>=m_list.count())return;
	delete m_list[index];
	m_list.removeAt(index);
	changed(this);
}
void DataGroup::AddDFReader(SoTypeReg *factory){
	SObject* so=factory->createInstance(Owner(),NULL);
	if(so!=NULL){
		if(so->Is(SOT_DFReader)){
			m_list.append((SoDFReader*)so);
			changed(this);
		}else
			delete so;
	}
}

DataGroupType DataGroup::GetType(){return m_dgtype;}
void DataGroup::SetType(DataGroupType type){
	if(type!=m_dgtype){
		m_dgtype=type;
		changed(this);
	}
}

void DataGroup::setstatusmsg(QString msg){statusmsg(msg);}
void DataGroup::CallDataEventGot(DataEvent *event){
	reg_event(event->Is_Valid());
	DataEventGot(event);
}
void DataGroup::errorreport(QString msg){error(msg);}

void DataGroup::__connect(){
	for(int i=0; i<m_list.count();i++){
		SoDFReader* dr=m_list[i];
		connect(dr,SIGNAL(statusmsg(QString)), this, SLOT(setstatusmsg(QString)));
		connect(dr,SIGNAL(DataEventGot(DataEvent*)),this,SLOT(CallDataEventGot(DataEvent*)));
		connect(dr,SIGNAL(error(QString)),this,SLOT(errorreport(QString)));
		if(i>0){
			connect(m_list[i-1],SIGNAL(DataReadingFinnished()),dr,SLOT(Read()));
		}
	}
	connect(m_list[m_list.count()-1],SIGNAL(DataReadingFinnished()),this,SLOT(AfterRead()));
}
void DataGroup::__disconnect(){
	for(int i=0; i<m_list.count();i++){
		SoDFReader* dr=m_list[i];
		disconnect(dr,SIGNAL(statusmsg(QString)), this, SLOT(setstatusmsg(QString)));
		disconnect(dr,SIGNAL(DataEventGot(DataEvent*)),this,SLOT(CallDataEventGot(DataEvent*)));
		disconnect(dr,SIGNAL(error(QString)),this,SLOT(errorreport(QString)));
		if(i>0){
			disconnect(m_list[i-1],SIGNAL(DataReadingFinnished()),dr,SLOT(Read()));
		}
	}
	disconnect(m_list[m_list.count()-1],SIGNAL(DataReadingFinnished()),this,SLOT(AfterRead()));
}

void DataGroup::Read(){
	if(m_list.count()==0)return;
	if(Owner()->IsBusy())return;
	__connect();
	Set_OK();ReadingStarted();
	clear_constants();clear_descr();
	clear_ev_cnt();
	m_list[0]->Read();
}
void DataGroup::AfterRead(){
	__disconnect();
	for(int i=0; i<m_list.count();i++){
		SoDFReader* dr=m_list[i];
		QStringList constlist=dr->ConstantNames();
		for(int i=0; i<constlist.count();i++){
			if((constlist[i]!="EventsCount")&&(constlist[i]!="BrokenEventsCount")){
				double oldval=Constant(constlist[i]);
				double thisval=dr->Constant(constlist[i]);
				ApplyConstant(constlist[i],oldval+thisval);
			}
		}
	}
	QStringList lst=ConstantNames();
	if(GetType()==DGAverage){
		for(int i=0; i<lst.count();i++){
			double oldval=Constant(lst[i]);
			ApplyConstant(lst[i],oldval/m_list.count());
		}
	}
	for(int i=0; i<lst.count();i++){
		double oldval=Constant(lst[i]);
		ApplyConstant(lst[i],oldval);
	}
	clear_descr();
	ReportFinished();
}

