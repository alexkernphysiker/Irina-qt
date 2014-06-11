#include "expression.h"
#include <QDataStream>
#include <QList>
#include <QStringList>

namespace Math_{
Expr *readexpr(QDataStream &str){
	char extype=0;
	str.readRawData(&extype,sizeof(char));
	switch(extype){
	case MATH__EXPR_NUMBER:
		return new Number(str);
	case MATH__EXPR_VARIABLE:
		return new Variable(str);
	case MATH__EXPR_UNARY:
		return new Unary(str);
	case MATH__EXPR_BINARY:
		return new Binary(str);
	default:
		return NULL;
	}
}

Expr::Expr(){m_type=MATH__EXPR_NOTYPE;}
Expr::Expr(QDataStream &){m_type=MATH__EXPR_NOTYPE;}
void Expr::Save(QDataStream &str){str.writeRawData(&m_type,sizeof(char));}
Expr::~Expr(){}
double Expr::Value(){return 0;}
char Expr::Type(){return m_type;}
void Expr::settype(char type){m_type=type;}
void Expr::catcherror(QString msg){error(msg);}
void Expr::acceptchanged(Math_::Expr *sender){changed(sender);}
void Expr::PassGettingValue(double &retval, char type, QString name,double x1, double x2){GetValue(retval,type,name,x1,x2);}
Expr *Expr::LookForParent(Expr *){return NULL;}

Number::Number():Expr(){settype(MATH__EXPR_NUMBER);m_value=0;}
Number::Number(QDataStream &str):Expr(str){settype(MATH__EXPR_NUMBER);str.readRawData((char*)&m_value,sizeof(double));}
void Number::Save(QDataStream &str){
	Expr::Save(str);
	str.writeRawData((char*)&m_value,sizeof(double));
}
Number::~Number(){}
double Number::Value(){return m_value;}
void Number::SetValue(double val){m_value=val;changed(this);}
QString Number::ExprStr(){return QString::number(m_value);}

QString readstr(QDataStream &str){
	QString m_name="";
	bool finished=false;
	while((!str.atEnd())&(!finished)){
		char c=0;
		str.readRawData(&c,1);
		if(c!=0)m_name= m_name.append(c);
		else finished=true;
	}
	return m_name.replace(" ","_");
}
void writestr(QString s, QDataStream &str){
	int len=s.length();
	for(int i=0; i<len; i++){
		char c=s.at(i).toAscii();
		if(c==0)c=' ';
		str.writeRawData(&c,1);
	}{
		char c=0;
		str.writeRawData(&c,1);
	}
}

Variable::Variable():Expr(){
	settype(MATH__EXPR_VARIABLE);m_name="";
}
Variable::Variable(QDataStream &str):Expr(str){
	settype(MATH__EXPR_VARIABLE);SetName(readstr(str));
}
void Variable::Save(QDataStream &str){
	Expr::Save(str);
	writestr(m_name,str);
}
Variable::~Variable(){}
double Variable::Value(){
	if(m_name=="")return 0;
	double res=0;
	GetValue(res,Type(),Name(),0,0);
	return res;
}
QString Variable::Name(){return m_name;}
void Variable::SetName(QString name){
	m_name=name.replace(" ","_");
	changed(this);
}
QString Variable::ExprStr(){return m_name;}

Unary::Unary():Expr(){
	settype(MATH__EXPR_UNARY);
	m_operand=NULL;m_name="";
}
Unary::Unary(QDataStream &str):Expr(str){
	settype(MATH__EXPR_UNARY);m_operand=NULL;
	SetName(readstr(str));
	SetOperand( readexpr(str));
}
void Unary::Save(QDataStream &str){
	Expr::Save(str);
	writestr(m_name,str);
	if(m_operand!=NULL)m_operand->Save(str);
	else{
		char c=0;
		str.writeRawData(&c,sizeof(char));
	}
}
Unary::~Unary(){
	if(m_operand!=NULL)delete m_operand;
}
double Unary::Value(){
	if(Operand()==NULL){
		error("Incomplete expression in unary '"+m_name+"'");
		return 0;
	}
	double res;
	GetValue(res,Type(),Name(),Operand()->Value(),0);
	return res;
}
QString Unary::Name(){return m_name;}
void Unary::SetName(QString name){
	m_name=name.replace(" ","_");changed(this);
}
Expr *Unary::Operand(){return m_operand;}
void Unary::SetOperand(Expr *opr){
	if(m_operand!=NULL)delete m_operand;
	m_operand=opr;
	if(opr!=NULL){
		connect(m_operand,SIGNAL(changed(Math_::Expr*)),
				this,SLOT(acceptchanged(Math_::Expr*)));
		connect(m_operand,SIGNAL(error(QString)),
				this,SLOT(catcherror(QString)));
		connect(m_operand,SIGNAL(GetValue(double&,char,QString,double,double)),
				this, SLOT(PassGettingValue(double&,char,QString,double,double)));
		changed(m_operand);
	}else
	changed(this);
}
Expr *Unary::LookForParent(Expr *expr){
	if(Operand()==expr)return this;
	return Operand()->LookForParent(expr);
}
QString Unary::ExprStr(){
	return QString("(")+m_name+" "+Operand()->ExprStr()+")";
}

Binary::Binary():Unary(){
	settype(MATH__EXPR_BINARY);m_operand2=NULL;
}
Binary::Binary(QDataStream &str):Unary(str){
	settype(MATH__EXPR_BINARY);m_operand2=NULL;
	SetOperand2(readexpr(str));
}
void Binary::Save(QDataStream &str){
	Unary::Save(str);
	if(m_operand2!=NULL)m_operand2->Save(str);
	else{
		char c=0;
		str.writeRawData(&c,sizeof(char));
	}
}
Binary::~Binary(){
	if(m_operand2!=NULL)delete m_operand2;
}
Expr *Binary::Operand2(){return m_operand2;}
void Binary::SetOperand2(Expr *opr){
	if(m_operand2!=NULL)delete m_operand2;
	m_operand2=opr;
	if(m_operand2!=NULL){
		connect(m_operand2,SIGNAL(changed(Math_::Expr*)),
				this,SLOT(acceptchanged(Math_::Expr*)));
		connect(m_operand2,SIGNAL(error(QString)),
				this,SLOT(catcherror(QString)));
		connect(m_operand2,SIGNAL(GetValue(double&,char,QString,double,double)),
				this, SLOT(PassGettingValue(double&,char,QString,double,double)));
		changed(m_operand2);
	}else
	changed(this);
}
double Binary::Value(){
	if(Operand()==NULL){
		error("Incomplete expression in binary '"+Name()+"'");
		return 0;
	}
	if(Operand2()==NULL){
		error("Incomplete expression in binary '"+Name()+"'");
		return 0;
	}
	double res=0;
	GetValue(res,Type(),Name(),Operand()->Value(),Operand2()->Value());
	return res;
}
Expr *Binary::LookForParent(Expr *expr){
	if(Operand()==expr)return this;
	if(Operand2()==expr)return this;
	Expr *e=Operand()->LookForParent(expr);
	if(e!=NULL)return e;
	return Operand2()->LookForParent(expr);
}
QString Binary::ExprStr(){
	return QString("(")+Operand()->ExprStr()+" "+Name()+" "+Operand2()->ExprStr()+")";
}


Expr* parseStringList(int lvl, QList<int> &levels, QStringList &subexpr, int &index){
	QList<Expr*> subres;
	QStringList thissubexpr;
	if(index<levels.count()){
		while((index<levels.count())&&(lvl<=levels.at(index))){
			if(lvl==levels.at(index)){
				thissubexpr<<subexpr.at(index);
				index++;
			}else{
				thissubexpr<<"";
				subres<<parseStringList(lvl+1,levels,subexpr,index);
			}
		}
		if(thissubexpr.count()>3){
			for(int i=0; i<subres.count();i++)if(subres.at(i))delete subres.at(i);
			return NULL;
		}
		if(thissubexpr.count()==3){
			if(thissubexpr.at(1)==""){
				for(int i=0; i<subres.count();i++)if(subres.at(i))delete subres.at(i);
				return NULL;
			}
			int exind=0;
			Expr* opr1=NULL;
			if(thissubexpr.at(0)==""){
				if(exind<subres.count()){
					opr1=subres.at(exind);
					exind++;
				}
			}else{
				bool isNumber=false;
				double val=thissubexpr.at(0).toDouble(&isNumber);
				if(isNumber){
					opr1=new Number();
					((Number*)opr1)->SetValue(val);
				}else{
					opr1=new Variable();
					((Variable*)opr1)->SetName(thissubexpr.at(0));
				}
			}
			Expr* opr2=NULL;
			if(thissubexpr.at(2)==""){
				if(exind<subres.count()){
					opr2=subres.at(exind);
					exind++;
				}
			}else{
				bool isNumber=false;
				double val=thissubexpr.at(2).toDouble(&isNumber);
				if(isNumber){
					opr2=new Number();
					((Number*)opr2)->SetValue(val);
				}else{
					opr2=new Variable();
					((Variable*)opr2)->SetName(thissubexpr.at(2));
				}
			}
			if(opr1)if(opr2){
				Binary* bin=new Binary();
				bin->SetName(thissubexpr.at(1));
				bin->SetOperand(opr1);
				bin->SetOperand2(opr2);
				return bin;
			}
			return NULL;
		}
		if(thissubexpr.count()==2){
			if(thissubexpr.at(0)==""){
				for(int i=0; i<subres.count();i++)if(subres.at(i))delete subres.at(i);
				return NULL;
			}
			int exind=0;
			Expr* opr1=NULL;
			if(thissubexpr.at(1)==""){
				if(exind<subres.count()){
					opr1=subres.at(exind);
					exind++;
				}
			}else{
				bool isNumber=false;
				double val=thissubexpr.at(1).toDouble(&isNumber);
				if(isNumber){
					opr1=new Number();
					((Number*)opr1)->SetValue(val);
				}else{
					opr1=new Variable();
					((Variable*)opr1)->SetName(thissubexpr.at(1));
				}
			}
			if(opr1){
				Unary* un=new Unary();
				un->SetName(thissubexpr.at(0));
				un->SetOperand(opr1);
				return un;
			}else return NULL;
		}
		if(thissubexpr.count()==1){
			int exind=0;
			Expr* opr1=NULL;
			if(thissubexpr.at(0)==""){
				if(exind<subres.count()){
					opr1=subres.at(exind);
					exind++;
				}
			}else{
				bool isNumber=false;
				double val=thissubexpr.at(0).toDouble(&isNumber);
				if(isNumber){
					opr1=new Number();
					((Number*)opr1)->SetValue(val);
				}else{
					opr1=new Variable();
					((Variable*)opr1)->SetName(thissubexpr.at(0));
				}
			}
			return opr1;
		}
		return NULL;
	}else return NULL;
}

Expr* ParseExprFromString(QString expr_str){
	QList<int> levels;
	QStringList subexpr;
	QString expr="";
	int level=0;
	for(int i=0; i<expr_str.length();i++){
		if(expr_str.at(i)=='('){
			if(expr!=""){
				subexpr<<expr;
				levels<<level;
				expr="";
			}
			level++;
		}else{
			if(expr_str.at(i)==')'){
				if(expr!=""){
					subexpr<<expr;
					levels<<level;
					expr="";
				}
				level--;
				if(level<0)return NULL;
			}else{
				if(expr_str.at(i)==' '){
					if(expr!=""){
						subexpr<<expr;
						levels<<level;
						expr="";
					}
				}else{
					expr.append(expr_str.at(i));
				}
			}
		}
	}
	if(level==0){
		if(expr!=""){
			subexpr<<expr;
			levels<<level;
			expr="";
		}
		int index=0;
		Expr* res=parseStringList(0,levels,subexpr,index);
		return res;
	}
	return NULL;
}

}
