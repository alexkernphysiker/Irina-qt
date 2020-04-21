// this file is distributed under
// GPL v 3.0 license
#include "sbdi_theor.h"
#include "elatheorview.h"
#include <Core/Irina-qt-headers/SortLibGui.h>

SBDI::SBDI(SortProject *father):SoTblFunc(father){AddType(4);}
SBDI::SBDI(QDataStream &str, SortProject *father):SoTblFunc(str,father){AddType(4);}
SBDI::~SBDI(){}
void SBDI::Save(QDataStream &str){SoTblFunc::Save(str);}
QString SBDI::DisplayName(){return SoTblFunc::DisplayName()+"; SBDI";}

//general mathimatics
const double pi=3.1415926;
const double D=1.54;
const double ed=2.225;
const double md=1875.612793;



double elath(uint Zt, double Ed, double theta){
	double theta_r=theta*pi/180;
	double ksi=ed/Ed;
	double Edfull=Ed+md;
	double pc_d=sqrt((Edfull*Edfull)-(md*md));
	double zomm=(md*Zt/pc_d)/137;
	double sq_ksi=sqrt(ksi);
	double Q=qSqrt(zomm/(sq_ksi*sq_ksi*sq_ksi));
	double ro0=zomm*(1+(1/sin(theta_r/2)));
	return exp(
				-((D*zomm)/(2*qSqrt(ksi)))
				*((3*pi*Q*Q*ro0*ro0)/((ro0-zomm)*(ro0*ro0+Q*Q)*(2*ro0*ro0+5*Q*Q)))
				*exp(-(2*ro0*ro0)/(3*Q*Q))
			);
}


ElaTheor::ElaTheor(SortProject *father):SBDI(father){
	AddType(0x01);
	m_en=false;min=0;max=0;step=0;secvar=0;
}
ElaTheor::ElaTheor(QDataStream &str, SortProject *father):SBDI(str,father){
	AddType(0x01);
	str.readRawData((char*)&m_en,sizeof(bool));
	str.readRawData((char*)&zt,sizeof(uint));
	str.readRawData((char*)&min,sizeof(double));
	str.readRawData((char*)&max,sizeof(double));
	str.readRawData((char*)&step,sizeof(double));
	str.readRawData((char*)&secvar,sizeof(double));
	recalc();
}
void ElaTheor::Save(QDataStream &str){
	SBDI::Save(str);
	str.writeRawData((char*)&m_en,sizeof(bool));
	str.writeRawData((char*)&zt,sizeof(uint));
	str.writeRawData((char*)&min,sizeof(double));
	str.writeRawData((char*)&max,sizeof(double));
	str.writeRawData((char*)&step,sizeof(double));
	str.writeRawData((char*)&secvar,sizeof(double));
}
ElaTheor::~ElaTheor(){}
QString ElaTheor::DisplayName(){	return SBDI::DisplayName()+" elastic scattering";}
bool ElaTheor::ArgIsEnergy(){return m_en;}
void ElaTheor::SetArgIsEnergy(bool flag){m_en=flag;}

uint ElaTheor::Zt(){return zt;}
void ElaTheor::SetZt(uint z){zt=z;changed(this);}

double ElaTheor::Min(){return min;}
void ElaTheor::SetMin(double val){min=val;changed(this);}

double ElaTheor::Max(){return max;}
void ElaTheor::SetMax(double val){max=val;changed(this);}

double ElaTheor::Step(){return step;}
void ElaTheor::SetStep(double val){step=val;changed(this);}

double ElaTheor::SecondVar(){return secvar;}
void ElaTheor::SetSecondVar(double val){secvar=val;changed(this);}

void ElaTheor::recalc(){
	clear_values();
	if((step>0)&(max>min))
		for(double v=min;v<=max;v+=step){
			if(m_en)
				add_item(v,elath(zt,v,secvar),0);
			else
				add_item(v,elath(zt,secvar,v),0);
	}
}

void *ElaTheor::GetForm(){
	ElaTheorView *view=new ElaTheorView(this);
	TblFuncView *form=new TblFuncView(this);
	form->AddTab("Elastic scattering of deuterons",view);
	return (void *)form;
}
