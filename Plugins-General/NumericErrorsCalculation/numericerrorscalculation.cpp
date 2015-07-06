// this file is distributed under
// GPL v 3.0 license
#include "numericerrorscalculation.h"
#include "thiswidget.h"

class VarError{
public:
	VarError(QString name);
	QString Name();
	double Error();
	void SetError(double val);
private:
	QString m_name;
	double m_error;
};
VarError::VarError(QString name){
	m_name=name;m_error=0;
}
QString VarError::Name(){return m_name;}
double VarError::Error(){return m_error;}
void VarError::SetError(double val){
	if(val>=0)m_error=val;
}

NumericErrorsCalculation::NumericErrorsCalculation(SortProject *proj):SoFormula(proj){
	AddType(4);m_errors=QList<void *>();m_delta=0;m_curvar="";m_reader=NULL;
	m_funcname="";
}
NumericErrorsCalculation::NumericErrorsCalculation(QDataStream &str, SortProject *proj):SoFormula(str,proj){
	AddType(4);m_errors=QList<void *>();m_delta=0;m_curvar="";m_reader=NULL;
	m_funcname=readstr(str);
	QString vn="";
	do{
		vn=readstr(str);
		if(vn!=""){
			double v=0;
			str.readRawData((char *)&v,sizeof(double));
			SetError(vn,v);
		}
	}while(vn!="");
}
void NumericErrorsCalculation::Save(QDataStream &str){
	SoFormula::Save(str);
	writestr(m_funcname,str);
	for(int i=0;i<Count();i++){
		QString vn=ErrName(i);
		double v=ErrValue(i);
		if(vn!=""){
			writestr(vn,str);
			str.writeRawData((char *)&v,sizeof(double));
		}
	}
	writestr("",str);
}
NumericErrorsCalculation::~NumericErrorsCalculation(){
	while(Count()>0)
		RemoveErr(0);
}
QString NumericErrorsCalculation::DisplayName(){return "FUNC::ERROR "+Name();}
QString NumericErrorsCalculation::FuncName(){return m_funcname;}
void NumericErrorsCalculation::SetFuncName(QString nme){
	m_funcname=nme;
	changed(this);
}
int NumericErrorsCalculation::Count(){
	return m_errors.count();
}
QString NumericErrorsCalculation::ErrName(int N){
	if(N<0)return "";
	if(N>=Count())return "";
	VarError *err=(VarError*)m_errors.at(N);
	return err->Name();
}
double NumericErrorsCalculation::ErrValue(int N){
	if(N<0)return 0;
	if(N>=Count())return 0;
	VarError *err=(VarError*)m_errors.at(N);
	return err->Error();
}
int NumericErrorsCalculation::ErrNum(QString name){
	int res=-1;
	for(int i=0; i<Count();i++){
		VarError *err=(VarError*)m_errors.at(i);
		if(err->Name()==name)
			res=i;
	}
	return res;
}
double NumericErrorsCalculation::ErrValue(QString name){
	return ErrValue(ErrNum(name));
}
void NumericErrorsCalculation::RemoveErr(int N){
	if(N<0)return;
	if(N>=Count())return;
	VarError *err=(VarError*)m_errors.at(N);
	delete err;
	m_errors.removeAt(N);
	changed(this);
}
void NumericErrorsCalculation::SetError(QString name, double val){
	int n=ErrNum(name);
	if(n>=0){
		VarError *err=(VarError*)m_errors.at(n);
		err->SetError(val);
	}else{
		VarError *err=new VarError(name);
		err->SetError(val);
		m_errors.append((void *)err);
	}
	changed(this);
}
void NumericErrorsCalculation::varvalueintersector(QString name, double *val, SoDFReader *dr,DataEvent*){
	if((m_curvar==name)&(dr==m_reader)){
		val[0]+=m_delta;
	}
}
void NumericErrorsCalculation::testdfr(double *res, double zer, double d, QString func,SoDFReader *fr, DataEvent *event){
	m_delta=d;
	double plus=0;
	Owner()->GetVar(func,&plus,fr,event);
	m_delta=-d;
	double minus=0;
	Owner()->GetVar(func,&minus,fr,event);
	double err1=plus-zer;
	double err2=minus-zer;
	if(err1<0)err1=-err1;
	if(err2<0)err2=-err2;
	if(err1<err2)err1=err2;
	res[0]+=err1*err1;
}
double NumericErrorsCalculation::Value(SoDFReader *fr, DataEvent *event){
	connect(Owner(),SIGNAL(variable_getting(QString,double*,SoDFReader*,DataEvent*)),
			this,SLOT(varvalueintersector(QString,double*,SoDFReader*,DataEvent*)));
	double res=0;
	m_delta=0;
	double zer=0;
	Owner()->GetVar(m_funcname,&zer,fr,event);
	for(int i=0; i<Count();i++){
		m_curvar=ErrName(i);
		double d=ErrValue(i);
		if(d==0){//take error values from another variable
			Owner()->GetVar(m_curvar+"_ERROR", &d, fr,event);
			if(d<=0){//calculate square root
				Owner()->GetVar(m_curvar, &d, fr,event);
				d=sqrt(d);
			}
		}
		for(int i=0; i<Owner()->Count();i++){
			SObject *so=Owner()->at(i);
			if(so->Is(SOT_DFReader)){
				m_reader=(SoDFReader*)so;
				testdfr(&res,zer,d,m_funcname,fr,event);
			}
		}
		m_reader=NULL;
		testdfr(&res,zer,d,m_funcname,fr,event);
	}
	m_delta=0;
	disconnect(Owner(),SIGNAL(variable_getting(QString,double*,SoDFReader*,DataEvent*)),
			   this,SLOT(varvalueintersector(QString,double*,SoDFReader*,DataEvent*)));
	return sqrt(res);
}
void *NumericErrorsCalculation::GetForm(){
	return new ThisWidget(this);
}

