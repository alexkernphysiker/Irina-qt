// this file is distributed under
// GPL v 3.0 license
#include "fbkin.h"
#include "kinview.h"
#include "ruthview.h"
#include "splineview.h"
#include <math_h/functions.h>

double BinaryKIN(bool &error,
				 char returntype,//'e'- product energy(lab);
				 //'E' - energy of collision(CM); 'T' - product emission angle(lab); I - jacobian(lab->CM)
				 double Mp,double MT,double Mx,
				 double ThetaLab,double Ep_lab, double Q){
	error=false;
	double E_cm = 0;
	double A_cm = 0;
	double I = 0;
	try {//Kinemathics
		double em = 931.48;//a.m.u. -> MeV
		double M = Mp + MT;
		double b = pow(M * em,2) + 2 * MT * em * Ep_lab
				+ pow(Mx * em,2) - pow((Mp + MT - Mx)*em, 2);
		double pc2 = pow( Mp * em + Ep_lab ,2) - pow(Mp * em,2); //projectiles (p*c)^2
		double bc = pc2 * pc2 / ( Mp * em + Ep_lab );
		double xx = Mp * em / sqrt(1 - bc);
		bc *= xx / ( xx + MT * em );
		double ec2 = ( Ep_lab + M * em - bc * sqrt(pc2) ) / ( 1 - bc );
		xx = ( ec2 - pow(MT * em,2) + pow((Mp + MT - Mx)*em, 2) ) /
			( ec2 - pow(Mp * MT * em * em ,2) )*
			sqrt(
					( ec2 - pow(M * em,2 )) * ( ec2 -pow(( Mp - MT ) *em ,2)) /
			( ec2 -pow ( M * em - Q ,2)) * ( ec2 - pow(Mx * em - ( ( M - Mx ) * em - Q ), 2) )
			);//If this magnitude is greater than 1, then one should get two
			// energy values for given theta value
		double E = M * em + Ep_lab;//full energy
		double stm = (
					( b * b - 4 * pow(Mx * em,2) * ( pow(M * em,2) + 2 * MT * em * Ep_lab ) ) /
					( 4 * pow(Mx * em,2) * pc2 )
			);// sin^2(theta_max)
		if(( ( pow(sin(ThetaLab * 3.14 / 180), 2) < stm ) & ( ThetaLab < 90 ) ) | ( stm > 1.00009 )) {
			//control if the angle value is valid
			//only one energy value is calculated even
			//if two possible values exist
			double Ex = ( b * E + cos(ThetaLab * 3.14 / 180) * sqrt(
				pc2 * ( b * b - 4 * pow(Mx * em ,2) * ( E * E - pc2 * pow(cos(ThetaLab * 3.14 / 180), 2) ) )
				)
				) / (
				2 * ( E * E - pc2 * pow(cos(ThetaLab * 3.14 / 180), 2) )
				);
			if(returntype=='e'){
				Ex-=Mx*em;
				return Ex;
			}
			//this value is proportional to center-of-mass speed
			double vC = sqrt(Ep_lab) * Mp / ( Mp + MT );
			//proportional to registered particle speed components
			double vyc = sqrt(E) * sin(ThetaLab * 3.14 / 180);
			double vxc = sqrt(E) * cos(ThetaLab * 3.14 / 180);
			try {
				A_cm = atan2(vyc, ( vxc - vC ));//theta_CM
			} catch (int){
				A_cm = 3.14;
			}
			//Jakobian
			I = ( 1 + ( vC / vyc ) * cos(ThetaLab * 3.14 / 180) ) / pow(pow(sin(ThetaLab * 3.14 / 180), 2) + pow(vC / vyc + cos(ThetaLab * 3.14 / 180), 2), 1.5);
			A_cm *= 180 / 3.1415926;
			E_cm = pow(sqrt(Ep_lab) - vC, 2);
		} else {
			I = 1;
		}
	} catch(int) {
		error=true;
	}
	switch(returntype){
	case 'T':
		return A_cm;
	case 'E':
		return E_cm;
	case 'I':
		return I;
	default:
		return 0;
	}
}


SoFBKin::SoFBKin(SortProject *father):SoFormula(father){
	AddType(2);AddType(1);
	mt="";mp="";mr="";Eplab="";thetapcm="";returntype='e';
}
SoFBKin::SoFBKin(QDataStream &str, SortProject *father):SoFormula(str,father){
	AddType(2);AddType(1);
	str.readRawData(&returntype, sizeof(char));
	mt=readstr(str);mp=readstr(str);mr=readstr(str);
	Eplab=readstr(str);thetapcm=readstr(str);
}
void SoFBKin::Save(QDataStream &str){
	SoFormula::Save(str);
	str.writeRawData(&returntype, sizeof(char));
	writestr(mt,str);writestr(mp,str);writestr(mr,str);
	writestr(Eplab,str);writestr(thetapcm,str);
}
SoFBKin::~SoFBKin(){}
QString SoFBKin::DisplayName(){return "FUNC::2BKIN::"+ReturnType()+" "+Name();}
double SoFBKin::Value(SoDFReader *fr, DataEvent *event){
	double Mp=0;
	Owner()->GetVar(mp,&Mp,fr,event);
	double MT=0;
	Owner()->GetVar(mt,&MT,fr,event);
	double Mr=0;
	Owner()->GetVar(mr,&Mr,fr,event);
	double Angle=0;
	Owner()->GetVar(thetapcm,&Angle,fr,event);
	double Ep=0;
	Owner()->GetVar(Eplab,&Ep,fr,event);
	double Q=0;
	Owner()->GetVar(Qreact,&Q,fr,event);
	bool err=false;
	double res= BinaryKIN(err,returntype,Mp,MT,Mr,Angle,Ep,Q);
	if(err)error("Kinemathics calculation error");
	return res;
}
QString SoFBKin::ReturnType(){
	switch(returntype){
	case 'T':
		return "THETA_CM";
	case 'E':
		return "Ep_CM";
	case 'e':
		return "Ex_lab";
	case 'I':
		return "I_labToCM)";
	default:
		return "not set";
	}
}
void SoFBKin::SetAngle(){returntype='T';changed(this);}
void SoFBKin::SetEcm(){returntype='E';changed(this);}
void SoFBKin::SetElab(){returntype='e';changed(this);}
void SoFBKin::SetI(){returntype='I';changed(this);}
void *SoFBKin::GetForm(){return new KinView(this);}

QString SoFBKin::MT(){return mt;}
void SoFBKin::SetMT(QString name){mt=name;changed(this);}
QString SoFBKin::Mp(){return mp;}
void SoFBKin::SetMp(QString name){mp=name;changed(this);}
QString SoFBKin::Mx(){return mr;}
void SoFBKin::SetMx(QString name){mr=name;changed(this);}
QString SoFBKin::Ep(){return Eplab;}
void SoFBKin::SetEp(QString name){Eplab=name;changed(this);}
QString SoFBKin::ThetaCM(){return thetapcm;}
void SoFBKin::SetThetaCM(QString name){thetapcm =name;changed(this);}
QString SoFBKin::Q(){return Qreact;}
void SoFBKin::SetQ(QString name){Qreact=name;changed(this);}

SoRuth::SoRuth(SortProject *proj):SoFormula(proj){
	AddType(2);AddType(2);
	m_zp="";m_zt="";m_theta="";m_e="";
}
SoRuth::SoRuth(QDataStream &str, SortProject *proj):SoFormula(str,proj){
	AddType(2);AddType(2);
	m_zp=readstr(str);
	m_zt=readstr(str);
	m_theta=readstr(str);
	m_e=readstr(str);
}
void SoRuth::Save(QDataStream &str){
	SoFormula::Save(str);
	writestr(m_zp,str);
	writestr(m_zt,str);
	writestr(m_theta,str);
	writestr(m_e,str);
}
SoRuth::~SoRuth(){}

QString SoRuth::DisplayName(){return "FUNC::2BKIN::RUTH "+Name();}

QString SoRuth::Zp(){return m_zp;}
void SoRuth::SetZp(QString name){m_zp=name;changed(this);}
QString SoRuth::Zt(){return m_zt;}
void SoRuth::SetZt(QString name){m_zt=name;changed(this);}
QString SoRuth::E(){return m_e;}
void SoRuth::SetE(QString name){m_e=name;changed(this);}
QString SoRuth::Theta(){return m_theta;}
void SoRuth::SetTheta(QString name){m_theta=name;changed(this);}

double SoRuth::Value(SoDFReader *fr, DataEvent *event){
	double z1=0;
	Owner()->GetVar(m_zp,&z1,fr,event);
	double z2=0;
	Owner()->GetVar(m_zt,&z2,fr,event);
	double E=0;
	Owner()->GetVar(m_e,&E,fr,event);
	double th=0;
	Owner()->GetVar(m_theta,&th,fr,event);
	th*=3.1415926/180;
	th=sin(th/2);
	th*=th;
	double res=z1*z2/E;
	res/=th;
	res*=res;
	return res*1.44;
}

void *SoRuth::GetForm(){return new RuthView(this);}


SoSP1Line::SoSP1Line(SortProject *proj):SoBinaryOperator(proj){
	AddType(2);resetlast();
	m_sigma="";m_N="";
	mt="";mp="";mr="";thetapcm="";Qreact="";
	connect(this,SIGNAL(changed(SObject*)),this,SLOT(resetlast()));
}
SoSP1Line::SoSP1Line(QDataStream &str, SortProject *proj):SoBinaryOperator(str,proj){
	AddType(2);resetlast();
	m_sigma=readstr(str);
	m_N=readstr(str);
	mt=readstr(str);
	mp=readstr(str);
	mr=readstr(str);
	thetapcm=readstr(str);
	Qreact=readstr(str);
	connect(this,SIGNAL(changed(SObject*)),this,SLOT(resetlast()));
}
void SoSP1Line::Save(QDataStream &str){
	SoBinaryOperator::Save(str);
	writestr(m_sigma,str);
	writestr(m_N,str);
	writestr(mt,str);
	writestr(mp,str);
	writestr(mr,str);
	writestr(thetapcm,str);
	writestr(Qreact,str);
}
SoSP1Line::~SoSP1Line(){}

void SoSP1Line::resetlast(){lastEp=0;lastRes=0;}

QString SoSP1Line::Sigma(){return m_sigma;}
void SoSP1Line::SetSigma(QString name){m_sigma=name;changed(this);}
QString SoSP1Line::N(){return m_N;}
void SoSP1Line::SetN(QString name){m_N=name;changed(this);}


QString SoSP1Line::MT(){return mt;}
void SoSP1Line::SetMT(QString name){mt=name;changed(this);}
QString SoSP1Line::Mp(){return mp;}
void SoSP1Line::SetMp(QString name){mp=name;changed(this);}
QString SoSP1Line::Mx(){return mr;}
void SoSP1Line::SetMx(QString name){mr=name;changed(this);}
QString SoSP1Line::ThetaCM(){return thetapcm;}
void SoSP1Line::SetThetaCM(QString name){thetapcm =name;changed(this);}
QString SoSP1Line::Q(){return Qreact;}
void SoSP1Line::SetQ(QString name){Qreact=name;changed(this);}

double SoSP1Line::F(double ex, double Ep, SoDFReader *dr, DataEvent *event){
	double Ex=0;
	double Mp=0;
	Owner()->GetVar(mp,&Mp,dr,event);
	double MT=0;
	Owner()->GetVar(mt,&MT,dr,event);
	double Mr=0;
	Owner()->GetVar(mr,&Mr,dr,event);
	double Angle=0;
	Owner()->GetVar(thetapcm,&Angle,dr,event);
	double Q=0;
	Owner()->GetVar(Qreact,&Q,dr,event);
	bool err=false;
	Ex= BinaryKIN(err,'e',Mp,MT,Mr,Angle,Ep,Q);
	if(err)error("Kinemathics calculation error");
	double sigma=1;
	Owner()->GetVar(m_sigma,&sigma,dr,event);
	double n=1;
	Owner()->GetVar(m_N,&n,dr,event);
	return n*Gaussian(ex,Ex,sigma);
}

void* SoSP1Line::GetForm(){return new splineview(this);}
