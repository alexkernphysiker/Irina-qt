#include <SortLibGui.h>
#include <simulation.h>
#include <functions/functions.h>
#include "simulationview.h"
#include "randomview.h"
#include <QLabel>
#include "gaussview.h"
#include "mixedview.h"
#include "binsumview.h"
#include <QMutexLocker>

Simulation::Simulation(SortProject *father):SoDFReader("",father){
	AddType(4);
	SetNADC(1);
}
Simulation::Simulation(QDataStream &str, SortProject *father):SoDFReader(str,father){
	AddType(4);
	char nadc=1;
	str.readRawData((char*)&nadc,sizeof(char));
	SetNADC(nadc);
	QStringList lst;
	for(char i=0; i<NADC();i++){
		lst.append(readstr(str));
	}
	SetFuncs(lst);
	uint cnt;
	str.readRawData((char*)&cnt,sizeof(uint));
	SetN(cnt);
}
void Simulation::Save(QDataStream &str){
	SoDFReader::Save(str);
	char nadc=NADC();
	str.writeRawData((char*)&nadc,sizeof(char));
	for(char i=0; i<NADC();i++){
		writestr(Func(i),str);
	}
	uint cnt=N();
	str.writeRawData((char*)&cnt,sizeof(uint));
}
QString Simulation::DisplayName(){
	return "SIMULATION "+Name();
}
Simulation::~Simulation(){clearfuncs();}

DataEvent *Simulation::NextEvent(QDataStream &/*datastr*/){
	DataEvent *event=new DataEvent(NADC());
	for(char i=0; i<NADC(); i++){
		double val;
		Owner()->GetVar(Func(i),&val,this,event);
		event->Set_ADC(i,val);
	}
	return event;
}

void Simulation::ReadHeader(QDataStream &/*datastr*/){}
void Simulation::DataEventIsGot(DataEvent */*Event*/){}
void Simulation::FinalDataCheck(){}
ReaderAsync *Simulation::_getReaderAsync(QString /*m_path*/, SoDFReader *father){
	return new SimulationAsync(father,n);
}

uint Simulation::N(){
	QMutexLocker locker(&m_mutex);
	return n;
}
void Simulation::SetN(uint val){
	{
		QMutexLocker locker(&m_mutex);
		n=val;
	}
	changed(this);
}
void Simulation::SetNADC(char val){
	{
		QMutexLocker locker(&m_mutex);
		clearfuncs();
		Set_NADC(val);
		initfuncs();
	}
	changed(this);
}

SimulationAsync::SimulationAsync(SoDFReader *father, uint N):
	ReaderAsync(father){
	n=N;
}
void SimulationAsync::Open(){
	FileOpened(NULL);
}

void SimulationAsync::process(){
	statusmsg(master->DisplayName()+": started");
	QTime timer;
	timer.start();
	for(uint i=0; i<n; i++){
		DataEvent *event=Owner()->NextEvent(*Stream());
		DataEventGot(event);
		if(master->CanReadAsync()){
			if(timer.elapsed()>1000){
				statusmsg(master->DisplayName()+": "+
						  QString::number(i)+" of "+QString::number(n)+" events processed.");
				timer.start();
			}
		}
	}
	DataReadingFinnished();
	statusmsg(master->DisplayName()+": finished");
	finnished();
}

void Simulation::initfuncs(){
}
void Simulation::clearfuncs(){
	functions.clear();
}

QString Simulation::Func(char ind){
	QMutexLocker locker(&m_mutex);
	if((ind>=0)&(ind<functions.count()))
		return functions.at(ind);
	else
		return "";
}

void Simulation::SetFuncs(QStringList val){
	{
		QMutexLocker locker(&m_mutex);
		functions.clear();
		for(char i=0; (i<NADC())&(i<val.count());i++){
			functions.append(val.at(i));
		}
	}
	changed(this);
}

void *Simulation::GetForm(){
	QWidget *contr=new SimulationView(this);
	DataFileView *form=new DataFileView(this);
	form->AddTab("Data Simulation",contr);
	return (void *)(form);
}


RandomMagnitude::RandomMagnitude(SortProject *proj):SoFormula(proj){
	AddType(8);
	last=NULL;lastreader=NULL;lastval=0;
	maxval=1000;
}
RandomMagnitude::RandomMagnitude(QDataStream &str, SortProject *proj):SoFormula(str,proj){
	AddType(8);
	last=0;lastreader=NULL;lastval=0;
	maxval=1000;
}
void RandomMagnitude::Save(QDataStream &str){
	SoFormula::Save(str);
}
RandomMagnitude::~RandomMagnitude(){}
QString RandomMagnitude::DisplayName(){
	return "FUNC::RAND "+Name();
}
double RandomMagnitude::Value(SoDFReader */*fr*/, DataEvent *event){
	if(last!=event){
		last=event;
		lastval=Math_::RandomUniformly(0,1);
	}
	return lastval;
}
void *RandomMagnitude::GetForm(){
	RandomView *form=new RandomView(this);
	QLabel *label=new QLabel("Generates random\nuniformly distributed\nvalues in range 0<=x<="+QString::number(maxval));
	form->AddWidget(label,"uniformly distributed");
	return (void*)(form);
}
double RandomMagnitude::MaxVal(){return maxval;}

GaussMagnitude::GaussMagnitude(SortProject *proj):RandomMagnitude(proj){
	AddType(1);average=0;sigma=1;maxval=5;
}
GaussMagnitude::GaussMagnitude(QDataStream &str, SortProject *proj):RandomMagnitude(str,proj){
	AddType(1);
	str.readRawData((char*)&average,sizeof(double));
	str.readRawData((char*)&sigma,sizeof(double));
	str.readRawData((char*)&maxval,sizeof(uint));
}
void GaussMagnitude::Save(QDataStream &str){
	RandomMagnitude::Save(str);
	str.writeRawData((char*)&average,sizeof(double));
	str.writeRawData((char*)&sigma,sizeof(double));
	str.writeRawData((char*)&maxval,sizeof(uint));
}
GaussMagnitude::~GaussMagnitude(){}
QString GaussMagnitude::DisplayName(){
	return "FUNC::RAND::GAUSS "+Name();
}

double GaussMagnitude::Value(SoDFReader */*fr*/, DataEvent *event){
	if(last!=event){
		last=event;
		lastval=Math_::RandomGauss(sigma,average,maxval);
	}
	return lastval;
}
double GaussMagnitude::Average(){return average;}
double GaussMagnitude::Sigma(){return sigma;}
uint GaussMagnitude::Discr(){return maxval;}
void GaussMagnitude::SetAverage(double val){
	average=val;
	changed(this);
}
void GaussMagnitude::SetSigma(double val){
	if(sigma>0){
		sigma=val;
		changed(this);
	}else
		error("sigma should be greater than zero");
}
void GaussMagnitude::SetDiscr(uint val){
	maxval=val;
	changed(this);
}
void *GaussMagnitude::GetForm(){
	RandomView *form=new RandomView(this);
	GaussView *widg=new GaussView(this);
	form->AddWidget(widg,"Gauss distribution");
	return (void*)(form);
}


class SubDistr{
public:
	SubDistr();
	~SubDistr();
	QString name;
	double probability;
};

SubDistr::SubDistr(){name="";probability=0;}
SubDistr::~SubDistr(){}

MixDistributions::MixDistributions(SortProject *proj):RandomMagnitude(proj){
	AddType(2);maxval=1;
}
MixDistributions::MixDistributions(QDataStream &str, SortProject *proj):RandomMagnitude(str,proj){
	AddType(2);maxval=1;
	QString nm="";
	do{
		nm=readstr(str);
		if(nm!=""){
			double prob=0;
			str.readRawData((char*)&prob,sizeof(double));
			Add(nm,prob);
		}
	}while(nm!="");
}
void MixDistributions::Save(QDataStream &str){
	RandomMagnitude::Save(str);
	for(int i=0;i<data.count();i++){
		SubDistr sd=data.at(i);
		writestr(sd.name,str);
		str.writeRawData((char*)&(sd.probability),sizeof(double));
	}
	writestr("",str);
}
MixDistributions::~MixDistributions(){}
QString MixDistributions::DisplayName(){return "FUNC::RAND::MIXEDDISTR "+Name();}
uint MixDistributions::Count(){return data.count();}
double MixDistributions::Probability(uint n){
	if(n<(uint)data.count()){
		return data.at(n).probability;
	}else
		return 0;
}
QString MixDistributions::Function(uint n){
	if(n<(uint)data.count()){
		return data.at(n).name;
	}else
		return 0;
}
void MixDistributions::Remove(uint n){
	if(n<(uint)data.count()){
		data.removeAt(n);
		changed(this);
	}else
		error("invalid index for remove");
}
void MixDistributions::Add(QString name, double probability){
	if(probability<0){
		error("probability cannot be less than 0");
		return;
	}
	SubDistr sd;
	sd.name=name;
	sd.probability=probability;
	double total=0;
	for(uint i=0; i<(uint)data.count();i++)
		total+=data.at(i).probability;
	if((total+probability)<=1){
		data.append(sd);
		changed(this);
	}else
		error("total probability cannot be greater than 1");
}
double MixDistributions::Value(SoDFReader *fr, DataEvent *event){
	double table[Count()];
	double val=0;
	for(uint i=0; i<Count();i++){
		val+=data.at(i).probability;
		table[i]=val;
	}
	double rand=RandomMagnitude::Value(fr,event);
	uint index=0;
	while(((table[index])<=rand)&(index<(Count()-1)))
		index++;
	val=0;
	Owner()->GetVar(Function(index),&val,fr,event);
	return val;
}
void *MixDistributions::GetForm(){
	RandomView *form=new RandomView(this);
	MixedView *view=new MixedView(this);
	form->AddWidget(view,"Function selector");
	return (void*)(form);
}


DistributedByFunction::DistributedByFunction(SoTblFunc *func):RandomMagnitude(func->Owner()){
	AddType(4);m_func=func;m_distr=NULL;maxval=1;
	getdata();
	connect(m_func,SIGNAL(deleting()),this, SLOT(remove()));
	connect(m_func,SIGNAL(changed(SObject*)),this,SLOT(getdata()));
}
DistributedByFunction::DistributedByFunction(QDataStream &str, SortProject *father):RandomMagnitude(str,father){
	AddType(4);m_func=NULL;m_distr=NULL;maxval=1;
	int ind=-1;
	str.readRawData((char*)&ind,sizeof(int));
	SObject *so=Owner()->at(ind);
	if(so!=NULL){
		if(so->Is(SOT_SoTblFunc)){
			m_func=(SoTblFunc*)so;
			connect(m_func,SIGNAL(deleting()),this, SLOT(remove()));
		}else
			error("Cannot find function for distribution");
	}else error("Cannot find function for distribution");
	getdata();
	connect(m_func,SIGNAL(deleting()),this, SLOT(remove()));
	connect(m_func,SIGNAL(changed(SObject*)),this,SLOT(getdata()));
}
void DistributedByFunction::Save(QDataStream &str){
	if(m_func==NULL)return;
	RandomMagnitude::Save(str);
	int ind=Owner()->Number(m_func);
	str.writeRawData((char*)&ind,sizeof(int));
}
DistributedByFunction::~DistributedByFunction(){if(m_distr!=NULL)delete [] m_distr;}
QString DistributedByFunction::DisplayName(){return "FUNC::RAND::CUSTOMDISTR "+Name();}
void *DistributedByFunction::GetForm(){
	RandomView *form=new RandomView(this);
	QLabel *label=new QLabel("Generates random values distributes by this function");
	form->AddWidget(label,"uniformly distributed");
	return (void*)(form);
}
void DistributedByFunction::getdata(){
	if(m_distr!=NULL)delete [] m_distr;
	if(m_func==NULL){
		m_distr=NULL;
		return;
	}
	int n=m_func->Count();
	if(n>1){
		m_distr = new double[n];
		m_distr[0]=0;
		{
			double x=0;
			double y=0;
			double dy=0;
			m_func->GetItem(0,x,y,dy);
			for(int i=1; i<n; i++){
				double nx=0;
				double ny=0;
				m_func->GetItem(i,nx,ny,dy);
				m_distr[i]=m_distr[i-1]+((nx-x)*(ny+y)/2);
				x=nx;y=ny;
			}
		}
		double k=m_distr[n-1];
		for(int i=0; i<n; i++)
			m_distr[i]/=k;
	}else m_distr=NULL;
}
double DistributedByFunction::Value(SoDFReader *fr, DataEvent *event){
	int n=m_func->Count();
	if(n>1){
		double rand=RandomMagnitude::Value(fr,event);
		double x=0;
		double y=0;
		double dy=0;
		int index=0;
		while((index<(n-1))&(m_distr[index]<rand)){
			index++;
		}
		double px=0;
		if(index>0)
			m_func->GetItem(index-1,px,y,dy);
		else{
			m_func->GetItem(0,px,y,dy);
			return px;
		}
		m_func->GetItem(index,x,y,dy);
		return px+(rand-m_distr[index-1])*(x-px)/(m_distr[index]-m_distr[index-1]);
	}else return 0;
}
SObject *DistributedByFunction::DisplParrent(){
	return m_func;
}


SoBinarySumm::SoBinarySumm(SortProject *proj):SoBinaryOperator(proj){
	AddType(3);
}
SoBinarySumm::SoBinarySumm(QDataStream &str, SortProject *proj):SoBinaryOperator(str,proj){
	AddType(3);
	QString N="";
	do{
		N=readstr(str);
		if(N!="")
			AddSummand(N);
	}while(N!="");
}
void SoBinarySumm::Save(QDataStream &str){
	SoBinaryOperator::Save(str);
	for(int i=0; i<Count();i++)
		writestr(Summand(i),str);
	writestr("",str);
}
SoBinarySumm::~SoBinarySumm(){}
int SoBinarySumm::Count(){return m_oprs.count();}
QString SoBinarySumm::Summand(int index){
	if(index<0)
		return "";
	if(index>=m_oprs.count())
		return "";
	return m_oprs.at(index);
}
void SoBinarySumm::Reset(){
	m_oprs.clear();
	changed(this);
}
void SoBinarySumm::AddSummand(QString name){
	m_oprs.append(name);
	changed(this);
}
double SoBinarySumm::F(double x, double y, SoDFReader *dr, DataEvent *event){
	double res=0;
	for(int i=0; i<Count();i++)
		res+=Owner()->GetBinary(Summand(i),x,y,dr,event);
	return res;
}
void *SoBinarySumm::GetForm(){return new BinSumView(this);}
