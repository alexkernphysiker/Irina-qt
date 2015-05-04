#include "enlos.h"
#include "enlosview.h"
#include "enlossintargetview.h"
#include <math_h/randomfunc.h>
#include <QMutexLocker>

double GetFinalEnergy(double E, double de, double X, SortProject *proj, QString stp,SoDFReader *fr, DataEvent *event){
	for(double x=0;(E>de)&(x<X);E-=de){
		double k1=1/proj->GetUnary(stp,E,fr,event);
		double k2=1/proj->GetUnary(stp,E-de,fr,event);
		x+=(k1+k2)*de/2;
	}
	if(E<de)return 0;
	else return E;
}

EnLos::EnLos(SoTblFunc *stoppingpower):SoFormula(stoppingpower->Owner()){
	AddType(0x10);AddType(0x01);m_stoppingpower=stoppingpower;
	m_dx=0.01;m_energyfuncname=""; m_lengthfuncname="";
}
EnLos::EnLos(QDataStream &str, SortProject *proj):SoFormula(str,proj){
	AddType(0x10);AddType(0x01);
	str.readRawData((char*)&m_dx,sizeof(double));
	m_energyfuncname=readstr(str);
	m_lengthfuncname=readstr(str);
	int ind=-1;
	str.readRawData((char*)&ind,sizeof(int));
	SObject *so=Owner()->at(ind);
	if(so!=NULL){
		if(so->Is(SOT_SoTblFunc)){
			m_stoppingpower=(SoTblFunc*)so;
			connect(m_stoppingpower,SIGNAL(changed(SObject*)),this,SLOT(change()));
			connect(m_stoppingpower,SIGNAL(deleting()),this,SLOT(remove()));
		}else
			error("Cannot find table function for stopping power");
	}else
		error("Cannot find table function for stopping power");
}
void EnLos::Save(QDataStream &str){
	SoFormula::Save(str);
	double dx=0.01;
	str.writeRawData((char*)&dx,sizeof(double));
	set_dx(dx);
	writestr(m_energyfuncname,str);
	writestr(m_lengthfuncname,str);
	int ind=Owner()->Number(m_stoppingpower);
	str.writeRawData((char*)&ind,sizeof(int));
}
EnLos::~EnLos(){}
QString EnLos::DisplayName(){return "FUNC::EnLoss "+Name();}
SObject *EnLos::DisplParrent(){return m_stoppingpower;}
QString EnLos::Energy(){
	QMutexLocker locker(&m_mutex);
	return m_energyfuncname;
}
void EnLos::SetEnergy(QString energy){
	{
		QMutexLocker locker(&m_mutex);
		m_energyfuncname=energy;
	}
	changed(this);
}
QString EnLos::Length(){
	QMutexLocker locker(&m_mutex);
	return m_lengthfuncname;
}
void EnLos::SetLength(QString length){
	{
		QMutexLocker locker(&m_mutex);
		m_lengthfuncname=length;
	}
	changed(this);
}
double EnLos::dx(){
	QMutexLocker locker(&m_mutex);
	return m_dx;
}
void EnLos::set_dx(double val){
	{
		QMutexLocker locker(&m_mutex);
		if(val<=0)return;
		m_dx=val;
	}
	changed(this);
}
void* EnLos::GetForm(){
	return new EnLosView(this);
}

double EnLos::Value(SoDFReader *fr, DataEvent *event){
	double e=0;
	Owner()->GetVar(m_energyfuncname,&e,fr,event);
	double l=0;
	Owner()->GetVar(m_lengthfuncname,&l,fr,event);
	for(double x=0; (x<l)&(e>0);x+=m_dx){
		double stp=m_stoppingpower->F(e,fr,event);
		double diff=l-x;
		if(diff>m_dx)diff=m_dx;
		e-=diff*stp;
	}
	if(e<0)e=0;
	return e;
}

EnLossInTarget::EnLossInTarget(SortProject *proj):SoFormula(proj){
	AddType(0x10);AddType(0x02);
	m_energyfuncname="";
	m_thetadetectorfunc="";
	m_thetatargetfunc="";
	m_targetthicknessfunc="";
	m_projectilestp="";
	m_xstp="";
	m_d3cs_domegadE="";
	m_dx=1.0;m_dE=0.01;m_Emax=10;
	prob_tbl=NULL;
	allocate_cstbl();
}
EnLossInTarget::EnLossInTarget(QDataStream &str, SortProject *proj):SoFormula(str,proj){
	AddType(0x10);AddType(0x02);
	m_energyfuncname=readstr(str);
	m_thetadetectorfunc=readstr(str);
	m_thetatargetfunc=readstr(str);
	m_targetthicknessfunc=readstr(str);
	m_projectilestp=readstr(str);
	m_xstp=readstr(str);
	m_d3cs_domegadE=readstr(str);
	m_dx=1.0;m_dE=0.01;m_Emax=10;
	str.readRawData((char*)&m_dx,sizeof(double));
	str.readRawData((char*)&m_dE,sizeof(double));
	str.readRawData((char*)&m_Emax,sizeof(double));
	prob_tbl=NULL;
	allocate_cstbl();
}
void EnLossInTarget::Save(QDataStream &str){
	SoFormula::Save(str);
	writestr(m_energyfuncname,str);
	writestr(m_thetadetectorfunc,str);
	writestr(m_thetatargetfunc,str);
	writestr(m_targetthicknessfunc,str);
	writestr(m_projectilestp,str);
	writestr(m_xstp,str);
	writestr(m_d3cs_domegadE,str);
	str.writeRawData((char*)&m_dx,sizeof(double));
	str.writeRawData((char*)&m_dE,sizeof(double));
	str.writeRawData((char*)&m_Emax,sizeof(double));
}
EnLossInTarget::~EnLossInTarget(){
	remove_cstbl();
}
QString EnLossInTarget::DisplayName(){return "FUNC::EnLoss::target "+Name();}
QString EnLossInTarget::Energy(){
	QMutexLocker locker(&m_mutex);
	return m_energyfuncname;
}
void EnLossInTarget::SetEnergy(QString energy){
	{
		QMutexLocker locker(&m_mutex);
		m_energyfuncname=energy;
	}
	changed(this);
}
QString EnLossInTarget::ThetaDetector(){
	QMutexLocker locker(&m_mutex);
	return m_thetadetectorfunc;
}
void EnLossInTarget::SetThetaDetector(QString theta){
	{
		QMutexLocker locker(&m_mutex);
		m_thetadetectorfunc=theta;
	}
	changed(this);
}
QString EnLossInTarget::ThetaTarget(){
	QMutexLocker locker(&m_mutex);
	return m_thetatargetfunc;
}
void EnLossInTarget::SetThetaTarget(QString theta){
	{
		QMutexLocker locker(&m_mutex);
		m_thetatargetfunc=theta;
	}
	changed(this);
}
QString EnLossInTarget::TargetThickness(){
	QMutexLocker locker(&m_mutex);
	return m_targetthicknessfunc;
}
void EnLossInTarget::SetTargetThickness(QString thickness){
	QMutexLocker locker(&m_mutex);
	m_targetthicknessfunc=thickness;changed(this);
}
QString EnLossInTarget::ProjectileStoppingPower(){
	QMutexLocker locker(&m_mutex);
	return m_projectilestp;
}
void EnLossInTarget::SetProjectileStoppingPower(QString unary){
	{
		QMutexLocker locker(&m_mutex);
		m_projectilestp=unary;
	}
	changed(this);
}
QString EnLossInTarget::XStoppingPower(){
	QMutexLocker locker(&m_mutex);
	return m_xstp;
}
void EnLossInTarget::SetXStoppingPower(QString unary){
	{
		QMutexLocker locker(&m_mutex);
		m_xstp=unary;
	}
	changed(this);
}
double EnLossInTarget::dx(){
	QMutexLocker locker(&m_mutex);
	return m_dx;
}
void EnLossInTarget::set_dx(double val){
	{
		QMutexLocker locker(&m_mutex);
		m_dx=val;
	}
	changed(this);
}
double EnLossInTarget::dE(){
	QMutexLocker locker(&m_mutex);
	return m_dE;
}
void EnLossInTarget::set_dE(double val){
	{
		QMutexLocker locker(&m_mutex);
		m_dE=val;
	}
	changed(this);
}
double EnLossInTarget::Emax(){
	QMutexLocker locker(&m_mutex);
	return m_Emax;
}
void EnLossInTarget::set_Emax(double val){
	{
		QMutexLocker locker(&m_mutex);
		m_Emax=val;
	}
	changed(this);
}
QString EnLossInTarget::CrossSection(){
	QMutexLocker locker(&m_mutex);
	return m_d3cs_domegadE;
}
void EnLossInTarget::SetCrossSection(QString binary){
	QMutexLocker locker(&m_mutex);
	m_d3cs_domegadE=binary;changed(this);
}
void *EnLossInTarget::GetForm(){return new EnLossInTargetView(this);}

void EnLossInTarget::allocate_cstbl(){
	remove_cstbl();
	int sz=Emax()/dE();
	prob_tbl=new double[sz+1];
}
void EnLossInTarget::remove_cstbl(){
	if(prob_tbl!=NULL)delete [] prob_tbl;
	prob_tbl=NULL;
}

double EnLossInTarget::Value(SoDFReader *fr, DataEvent *event){
	//Getting values
	double E=0;
	Owner()->GetVar(Energy(),&E,fr,event);
	double thetad=0;
	Owner()->GetVar(ThetaDetector(),&thetad,fr,event);
	double thetat=0;
	Owner()->GetVar(ThetaTarget(),&thetat,fr,event);
	double thickness=0;
	Owner()->GetVar(TargetThickness(),&thickness,fr,event);
	///geometry: obtain interaction point position (using random) and calculate particles' runs
	double Xp=thickness;
	double Xx=thickness;
	{
		//describes the position of interaction point inside the target
		double pos_rel=RandomUniformlyR<double>(0,1);
		{//Xp - projectile particle's run through target before interaction
			double cos_th=Owner()->GetUnary("abs",	Owner()->GetUnary("cosd",thetat,fr,event),fr,event);
			if(cos_th==0)return 0;//wrong target position
			Xp*=pos_rel/cos_th;
		}
		{//Xx - registered particle's run through target after interaction
			double cos_th_rel=Owner()->GetUnary("cosd",thetad-thetat,fr,event);
			if(cos_th_rel==0)return 0;//wrong relative detector-target position
			if(cos_th_rel>0){
				Xx*=(1-pos_rel)/cos_th_rel;
			}else{
				Xx*=-(pos_rel)/cos_th_rel;
			}
		}
	}
	//obtain projectile energy in interaction point
	E=GetFinalEnergy(E,dE()/m_dx,Xp,Owner(),ProjectileStoppingPower(),fr,event);
	// obtain interaction product energy (using random)
	{//E contains projectile energy
		//obtain product energy
		int sz=Emax()/dE();
		if(sz<3)return 0; // Cross section table is too small
		prob_tbl[0]=0;
		for(int i=1; i<=sz;i++)
			prob_tbl[i]=Owner()->GetBinary(CrossSection(),dE()*i,E,fr,event)*dE()+prob_tbl[i-1];
		double en_det=RandomUniformlyR<double>(0,prob_tbl[sz]);
		int i=WhereToInsert(0,sz,prob_tbl,en_det);
		E=i*m_dE;
		E-=m_dE*(prob_tbl[i] - en_det)/(prob_tbl[i]-prob_tbl[i-1]);
	}//E contains product energy just after interaction
	// obtain final product energy
	E=GetFinalEnergy(E,dE()/m_dx,Xx,Owner(),XStoppingPower(),fr,event);
	//return the result got
	return E;
}
