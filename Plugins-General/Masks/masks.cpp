#include <masks.h>
#include "extmaskview.h"

ExtendedMask::ExtendedMask(SortProject *father):SoEFilter(father){
	AddType(4);
	opr1="";opr2="";predicate="=";
}
ExtendedMask::ExtendedMask(QDataStream &str, SortProject *father):SoEFilter(str,father){
	AddType(4);
	opr1=readstr(str);
	opr2=readstr(str);
	predicate=readstr(str);
}
void ExtendedMask::Save(QDataStream &str){
	SoEFilter::Save(str);
	writestr(opr1,str);
	writestr(opr2,str);
	writestr(predicate,str);
}
ExtendedMask::~ExtendedMask(){}

QString ExtendedMask::Func1(){
	return opr1;
}
void ExtendedMask::SetFunc1(QString name){
	opr1=name;
	changed(this);
}
QString ExtendedMask::Func2(){
	return opr2;
}
void ExtendedMask::SetFunc2(QString name){
	opr2=name;
	changed(this);
}
QString ExtendedMask::Predicate(){
	return predicate;
}

bool ExtendedMask::getpredicate(double opr1, double opr2, bool errorreport){
	if(predicate=="=")
		return opr1==opr2;
	if(predicate==">")
		return opr1>opr2;
	if(predicate=="<")
		return opr1<opr2;
	if(predicate==">=")
		return opr1>=opr2;
	if(predicate=="<=")
		return opr1<=opr2;
	if(errorreport)
		error("Extended event filter: unknown predicate "+predicate);
	return false;
}

void ExtendedMask::SetPredicate(QString value){
	predicate=value;
	getpredicate(0,0,true);
	changed(this);
}

bool ExtendedMask::In(SoDFReader *dr, DataEvent *event){
	double o1=0;
	Owner()->GetVar(opr1,&o1, dr, event);
	double o2=0;
	Owner()->GetVar(opr2,&o2, dr, event);
	return getpredicate(o1,o2);
}
QString ExtendedMask::DisplayName(){
	return "FILTER::PREDICATE "+opr1+" "+predicate+" "+opr2;
}
void *ExtendedMask::GetForm(){
	return new ExtMaskView(this);
}
