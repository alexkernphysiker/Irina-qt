#include <SortLib.h>
#include <functions/math_templates.h>


SoUnaryOperator::SoUnaryOperator(SortProject *proj):SObject(proj){AddType(5);}
SoUnaryOperator::SoUnaryOperator(QDataStream &str, SortProject *proj):SObject(str,proj){AddType(5);}
SoUnaryOperator::~SoUnaryOperator(){}
QString SoUnaryOperator::DisplayName(){return "UNARY "+Name();}


SoTblFunc::SoTblFunc(SortProject *father):SoUnaryOperator(father){
	AddType(0x1);clear_values();connect(this,SIGNAL(changed(SObject*)),this,SLOT(recalc()));
}
SoTblFunc::SoTblFunc(QDataStream &str, SortProject *father):SoUnaryOperator(str,father){
	AddType(0x1);clear_values();connect(this,SIGNAL(changed(SObject*)),this,SLOT(recalc()));
}
void SoTblFunc::Save(QDataStream &str){
	SObject::Save(str);
}
SoTblFunc::~SoTblFunc(){}

QString SoTblFunc::DisplayName(){return "UNARY::TABLE "+Name();}

void SoTblFunc::clear_values(){X.clear();Y.clear();dY.clear();}

void SoTblFunc::add_item(double x, double y, double dy){
	int i=0;
	if(X.count()>0)i=Math_::insertIndex(X,x);
	X.insert(i,x);Y.insert(i,y);dY.insert(i,dy);
}
typedef double* pdouble;
double SoTblFunc::F(double x, SoDFReader *, DataEvent *){//ToDo: use better interpolation alhorythm
	if(X.count()==0)return 0;
	if(X.count()==1)return Y[0];
	if(x<=X[0])return Y[0];
	if(x>=X[X.count()-1])return Y[X.count()-1];
	return Math_::interpolateLinear(X,Y,x);
}

void SoTblFunc::Export(){
	out_1d(Name());
	for(int i=0; i<Count();i++){
			DataEvent *item=new DataEvent(4);
			item->Set_ADC(0,X[i]);
			item->Set_ADC(1,Y[i]);
			item->Set_ADC(2,0);
			item->Set_ADC(3,dY[i]);
			item->Freeze();
			out_dataitem(item);
	}
	out_end();
}

int SoTblFunc::Count(){return X.count();}

void SoTblFunc::GetItem(int n, double &x, double &y, double &dy){
	if((n>=0)&(n<Count())){
		x=X.at(n);y=Y.at(n);dy=dY.at(n);
	}else{
		x=0;y=0;dy=0;
	}
}

SoTblFuncTxt::SoTblFuncTxt(SortProject *father):SoTblFunc(father){
	AddType(3);m_path="";
	columns[0]=0;
	columns[1]=1;
	columns[2]=-1;
}
SoTblFuncTxt::SoTblFuncTxt(QDataStream &str, SortProject *father, bool old):SoTblFunc(str,father){
	AddType(3);
	SetPath(readstr(str));
	if(!old){
		str.readRawData((char*)&columns,sizeof(columns));
	}else{
		columns[0]=0;
		columns[1]=1;
		columns[2]=-1;
	}
}

void SoTblFuncTxt::Save(QDataStream &str){
	SoTblFunc::Save(str);
	writestr(m_path,str);
	str.writeRawData((char*)&columns,sizeof(columns));
}
SoTblFuncTxt::~SoTblFuncTxt(){}

QString SoTblFuncTxt::DisplayName(){return SoTblFunc::DisplayName()+"; FILE="+Path();}

QString SoTblFuncTxt::Path(){return m_path;}
void SoTblFuncTxt::SetPath(QString path){
	m_path=path;
	changed(this);
}

int SoTblFuncTxt::Column(uint ind){
	if(ind<3)
		return columns[ind];
	else
		return 0;
}

void SoTblFuncTxt::SetColumn(uint ind, int col){
	if(ind<3){
		columns[ind]=col;
		changed(this);
	}
}

void SoTblFuncTxt::recalc(){
	clear_values();
	QFile file(m_path);
	if ( file.open(QIODevice::ReadOnly) ) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			QStringList items1=line.replace(",",".").split('\t',QString::SkipEmptyParts,Qt::CaseInsensitive);
			QStringList items;
			for(int i=0; i<items1.count();i++)
				items.append(items1.at(i).split(' ',QString::SkipEmptyParts,Qt::CaseInsensitive));
			double x[3];
			for(int i=0;i<3;i++)x[i]=0;
			bool ok;
			for(int i=0; i<items.length();i++){
				int ind=-1;
				for(int j=0;j<3;j++)if(columns[j]==i)ind=j;
				if(ind>=0){
					QString num=items.at(i).toLower();
					if(num[0]=='.')num="0"+num;
					x[ind]=num.toDouble(&ok);
					if(!ok){
						x[ind]=0;
						error("invalid number format in column "+QString::number(i));
					}
				}
			}
			add_item(x[0],x[1],x[2]);
		}
		file.close();
	}
}

SP1Norm::SP1Norm(SoECSP1 *sp1):SoTblFunc(sp1->Owner()){
	AddType(2);m_sp1=sp1;m_func="";
	connect(m_sp1,SIGNAL(changed(SObject*)),this,SLOT(change()));
	connect(m_sp1,SIGNAL(deleting()),this,SLOT(remove()));
	change();
}
SP1Norm::SP1Norm(QDataStream &str, SortProject *father):SoTblFunc(str,father){
	AddType(2);m_sp1=NULL;
	int ind=-1;
	str.readRawData((char*)&ind,sizeof(int));
	SObject *so=Owner()->at(ind);
	if(so!=NULL){
		if(so->Is(SOT_ECSP1)){
			m_sp1=(SoECSP1*)so;
			connect(m_sp1,SIGNAL(deleting()),this,SLOT(remove()));
		}else
			error("Cannot find function for distribution");
	}else
		error("Cannot find function for distribution");
	m_func=readstr(str);
	change();
}
void SP1Norm::Save(QDataStream &str){
	SoTblFunc::Save(str);
	int ind=Owner()->Number(m_sp1);
	str.writeRawData((char*)&ind,sizeof(int));
	writestr(m_func,str);
}
SP1Norm::~SP1Norm(){}
QString SP1Norm::DisplayName(){return SoTblFunc::DisplayName()+" norm";}
SoECSP1 *SP1Norm::Spectrum(){return m_sp1;}
QString SP1Norm::NormFunc(){return m_func;}
void SP1Norm::SetNormFunc(QString name){
	m_func=name;
	change();
}
void SP1Norm::change(){recalc();changed(this);}
void SP1Norm::recalc(){
	clear_values();
	double K=0;
	Owner()->GetVar(m_func,&K,Spectrum()->Source(),NULL);
	int n=Spectrum()->Axis()->ChannelCount();
	for(int i=0;i<n;i++){
		int cnt=Spectrum()->ChCnt(i);
		double x=Spectrum()->Axis()->GetMagnitude(i);
		double y=cnt*K;
		double dy=sqrt(double(cnt))*K;
		add_item(x, y, dy);
	}
}

SObject *SP1Norm::DisplParrent(){return m_sp1;}

SP1Norm_CoefErr::SP1Norm_CoefErr(SoECSP1 *sp1):SP1Norm(sp1){
	AddType(1);m_func_err="";
}
SP1Norm_CoefErr::SP1Norm_CoefErr(QDataStream &str, SortProject *father):SP1Norm(str,father){
	AddType(1);
	m_func_err=readstr(str);
}
void SP1Norm_CoefErr::Save(QDataStream &str){
	SP1Norm::Save(str);
	writestr(m_func_err,str);
}
SP1Norm_CoefErr::~SP1Norm_CoefErr(){}
QString SP1Norm_CoefErr::NormFuncErr(){return m_func_err;}
void SP1Norm_CoefErr::SetNormFuncErr(QString name){
	m_func_err=name;change();
}
void SP1Norm_CoefErr::SetNormFunc(QString name, QString nameerr){
	m_func=name;m_func_err=nameerr;change();
}
void SP1Norm_CoefErr::recalc(){
	clear_values();
	double K=0;
	double dK=0;
	Owner()->GetVar(NormFunc(),&K,Spectrum()->Source(),NULL);
	Owner()->GetVar(m_func_err,&dK,Spectrum()->Source(),NULL);
	double K2=K*K; dK*=dK;
	int n=Spectrum()->Axis()->ChannelCount();
	for(int i=0;i<n;i++){
		int cnt=Spectrum()->ChCnt(i);
		double x=Spectrum()->Axis()->GetMagnitude(i);
		double y=cnt*K;
		double dy=sqrt(double(cnt)*K2+dK);
		add_item(x, y, dy);
	}
}




SoBinaryOperator::SoBinaryOperator(SortProject *proj):SObject(proj){AddType(3);}
SoBinaryOperator::SoBinaryOperator(QDataStream &str, SortProject *proj):SObject(str,proj){AddType(3);}
SoBinaryOperator::~SoBinaryOperator(){}
QString SoBinaryOperator::DisplayName(){return "BINARY(x,y) "+Name();}

SoTableOfTables::SoTableOfTables(SortProject *proj):SoBinaryOperator(proj){
	AddType(1);
}

SoTableOfTables::SoTableOfTables(QDataStream &str, SortProject *proj):SoBinaryOperator(str,proj){
	AddType(1);
	QString name=0;
	do{
		name=readstr(str);
		if(name!=""){
			double y=0;
			str.readRawData((char*)&y,sizeof(double));
			Add(y,name);
		}
	}while(name!="");
}

void SoTableOfTables::Save(QDataStream &str){
	SoBinaryOperator::Save(str);
	for(int i=0; i<Count();i++){
		writestr(GetUnaryName(i),str);
		double y=GetY(i);
		str.writeRawData((char*)&y,sizeof(double));
	}
	writestr("",str);
}

SoTableOfTables::~SoTableOfTables(){}

int SoTableOfTables::Count(){return m_yvalues.count();}
void SoTableOfTables::Remove(int index){
	if(index<0)return;
	if(index>=Count())return;
	m_yvalues.removeAt(index);
	m_unarynames.removeAt(index);
	changed(this);
}
double SoTableOfTables::GetY(int index){
	if(index<0)return 0;
	if(index>=Count())return 0;
	return m_yvalues.at(index);
}
QString SoTableOfTables::GetUnaryName(int index){
	if(index<0)return "";
	if(index>=Count())return "";
	return m_unarynames.at(index);
}
void SoTableOfTables::Add(double y, QString name){
	if(name=="")return;
	int i=0;
	if(m_yvalues.count()>0)
		i=Math_::insertIndex(m_yvalues,y);
	m_yvalues.insert(i,y);m_unarynames.insert(i,name);
	changed(this);
}
double SoTableOfTables::F(double x, double y, SoDFReader *dr, DataEvent *event){
	if(Count()==0)return 0;
	if(Count()==1)return Owner()->GetUnary(m_unarynames[0],x,dr,event);
	int i=0;
	if(m_yvalues.count()>0)
		i=Math_::insertIndex(m_yvalues,y);
	if(i==0)i++;
	double fi=Owner()->GetUnary(m_unarynames[i],x,dr,event);
	double fn=Owner()->GetUnary(m_unarynames[i-1],x,dr,event);
	double k=(fi-fn)/(m_yvalues[i]-m_yvalues[i-1]);
	return fn+k*(y-m_yvalues[i-1]);
}
