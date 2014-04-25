#include <SortLib.h>
#include <expression/expression.h>


/////////// global variable
SoValue::SoValue(SortProject *father):SObject(father){
	AddType(SOT_Value);m_value=0;
}
SoValue::SoValue(QDataStream &str, SortProject *father):SObject(str,father){
	AddType(SOT_Value);
	str.readRawData((char*)&m_value, sizeof(double));
}
void SoValue::SetValue(double val){m_value=val;changed(this);}
SoValue::~SoValue(){}
void SoValue::Save(QDataStream &str){
	SObject::Save(str);
	str.writeRawData((char*)&m_value,sizeof(double));
}
double SoValue::Value(){return m_value;}
QString SoValue::DisplayName(){return "VAR "+Name()+" = "+QString::number(m_value);}




//////////////// function that depends on datafile and/or data event
SoFormula::SoFormula(SortProject *father):SObject(father){
	AddType(SOT_Formula);m_expr=NULL;m_useexpr=true;
}
SoFormula::SoFormula(QDataStream &str, SortProject *father):SObject(str,father){
	AddType(SOT_Formula);m_expr=NULL;m_useexpr=true;
	Set_Expression( Math_::readexpr(str));
}
void SoFormula::Save(QDataStream &str){
	SObject::Save(str);
	if(m_expr!=NULL)
		m_expr->Save(str);
	else{
		char c=0;
		str.writeRawData(&c,sizeof(char));
	}
}
SoFormula::~SoFormula(){	if(m_expr!=NULL)delete m_expr;}
void SoFormula::SetValueGetParams(SoDFReader *fr, DataEvent *event){
	vg_fr=fr;vg_event=event;
}
void SoFormula::GetValueForExpr(double &retval, char type, QString name, double x1, double x2){
	retval=0;
	switch(type){
	case MATH__EXPR_VARIABLE:
		if(!Owner()->GetVar(name,&retval,vg_fr,vg_event)){
			error("CAnnot find variable or function '"+name+"'");
		}
		break;
	case MATH__EXPR_UNARY:
		retval=Owner()->GetUnary(name,x1,vg_fr,vg_event);
		break;
	case MATH__EXPR_BINARY:
		retval=Owner()->GetBinary(name,x1,x2,vg_fr,vg_event);
		break;
	}
}
double SoFormula::Value(SoDFReader *fr, DataEvent *event){
	if(m_expr==NULL)return 0;
	SetValueGetParams(fr,event);
	return m_expr->Value();
}
Math_::Expr *SoFormula::Expression(){
	return m_expr;
}
void SoFormula::Set_Expression(Math_::Expr *expr){
	if(m_expr!=NULL)delete m_expr;
	m_expr=expr;
	if(m_expr!=NULL){
		connect(m_expr,SIGNAL(changed(Math_::Expr*)),this,SLOT(acceptchanged(Math_::Expr*)));
		connect(m_expr,SIGNAL(error(QString)),this,SLOT(catcherror(QString)));
		connect(m_expr,SIGNAL(GetValue(double&,char,QString,double,double)),this,SLOT(GetValueForExpr(double&,char,QString,double,double)));
	}
	changed(this);changedexpr(m_expr);
}
void SoFormula::Del_Expression(){Set_Expression(NULL);}
void SoFormula::catcherror(QString msg){
	error("Error in expression "+msg);
}
void SoFormula::acceptchanged(Math_::Expr *sender){changed(this);changedexpr(sender);}
QString SoFormula::DisplayName(){return "FUNC "+Name();}



//////////// spectrum axis
SoSPAxis::SoSPAxis(SortProject *father):SoFormula(father){
	AddType(1);m_min=0;m_max=1023;m_channelw=1;
}
SoSPAxis::SoSPAxis(QDataStream &str, SortProject *father):SoFormula(str,father){
	AddType(1);
	str.readRawData((char*)&m_min,sizeof(double));
	str.readRawData((char*)&m_max,sizeof(double));
	str.readRawData((char*)&m_channelw,sizeof(double));
}
void SoSPAxis::Save(QDataStream &str){
	SoFormula::Save(str);
	str.writeRawData((char*)&m_min,sizeof(double));
	str.writeRawData((char*)&m_max,sizeof(double));
	str.writeRawData((char*)&m_channelw,sizeof(double));
}
SoSPAxis::~SoSPAxis(){}
int SoSPAxis::getChannel(SoDFReader *fr, DataEvent *event){
	double val=Value(fr,event);
	val=(val-m_min)/m_channelw;
	return int(val);
}
double SoSPAxis::GetMagnitude(int ch){
	return (double(ch)*ChannelW())+m_min;
}
int SoSPAxis::ChannelCount(){return int((m_max-m_min)/m_channelw)+1;}
double SoSPAxis::ChannelW(){return m_channelw;}
void SoSPAxis::SetChannelW(double val){
	if(val>0){
		if(val<(m_max-m_min))
			m_channelw=val;
		else m_channelw=(m_max-m_min);
	}
	changed(this);
	AxisChanged();
}
double SoSPAxis::Min(){return m_min;}
void SoSPAxis::SetMin(double val){
	if(val<m_max)
		m_min=val;
	changed(this);
	AxisChanged();
}
double SoSPAxis::Max(){return m_max;}
void SoSPAxis::SetMax(double val){
	if(val>m_min)
		m_max=val;
	changed(this);
	AxisChanged();
}
QString SoSPAxis::DisplayName(){return "FUNC::AXIS "+Name();}
void SoSPAxis::SetMinMaxChW(double min, double max, double chw){
	m_min=min;
	if(max>m_min)
		m_max=max;
	if(chw>0){
		if(chw<(m_max-m_min))
			m_channelw=chw;
		else m_channelw=(m_max-m_min);
	}
	changed(this);
	AxisChanged();
}
void SoSPAxis::BeforeDelete(){
	SObject::BeforeDelete();
	for(int i=0; i<Owner()->Count();i++){
		SObject *so=Owner()->at(i);
		if(so->Is(SOT_EFWin)){
			SoEFWin *win=(SoEFWin*)so;
			if(win->Axis()==this){
				Owner()->Remove(Owner()->Number( win));
				i--;
			}
		}else{
			if(so->Is(SOT_ECSP1)){
				SoECSP1 *sp1=(SoECSP1*)so;
				if(sp1->Axis()==this){
					Owner()->Remove(Owner()->Number(so));
					i--;
				}
			}else{
				if(so->Is(SOT_ECSP2)){
					SoECSP2 *sp2=(SoECSP2*)so;
					if((sp2->XAxis()==this)|(sp2->YAxis()==this)){
						Owner()->Remove(Owner()->Number(so));
						i--;
					}
				}else{
					if(so->Is(SOT_EFLoc)){
						SoEFLoc *loc=(SoEFLoc*)so;
						if((loc->XAxis()==this)|(loc->YAxis()==this)){
							Owner()->Remove(Owner()->Number(so));
							i--;
						}
					}
				}
			}
		}
	}
}


////////// spectrum axis with dE-E calibration
SoSPEnergy::SoSPEnergy(SortProject *father):SoSPAxis(father){
	AddType(1);m_x=NULL;m_y=NULL;m_points=QList<QPoint>();
	SetMarker(0,QPoint(0,0));
	SetMarker(1,QPoint(0,0));
	SetMarker(2,QPoint(0,0));
	buildexpr();
}
SoSPEnergy::SoSPEnergy(QDataStream &str, SortProject *father):SoSPAxis(str,father){
	AddType(1);m_x=NULL;m_y=NULL;m_points=QList<QPoint>();
	int an=-1;
	str.readRawData((char*)&an,sizeof(int));
	if(an>=0)SetXAxis((SoSPAxis*)Owner()->at(an));
	an=-1;
	str.readRawData((char*)&an,sizeof(int));
	if(an>=0)SetYAxis((SoSPAxis*)Owner()->at(an));
	int x=-1;
	int y=-1;
	do{
		str.readRawData((char*)&x,sizeof(int));
		str.readRawData((char*)&y,sizeof(int));
		double d=0;
		str.readRawData((char*)&d,sizeof(double));
		if((x>=0)&(y>=0)){
			SetMarker(-1,QPoint(x,y));
			SetMagn(MarkerCnt()-1,d);
		}
	}while((x>=0)&(y>=0));
	buildexpr();
}
void SoSPEnergy::Save(QDataStream &str){
	SoSPAxis::Save(str);
	int an=-1;
	if(XAxis()!=NULL)an=Owner()->Number(XAxis());
	str.writeRawData((char*)&an,sizeof(int));
	an=-1;
	if(YAxis()!=NULL)an=Owner()->Number(YAxis());
	str.writeRawData((char*)&an,sizeof(int));
	if((XAxis()!=NULL)&(YAxis()!=NULL))
		for(int i=0; i<MarkerCnt();i++){
			QPoint p=Marker(i);
			int x=p.x();
			int y=p.y();
			double d=Magn(i);
			str.writeRawData((char*)&x,sizeof(int));
			str.writeRawData((char*)&y,sizeof(int));
			str.writeRawData((char*)&d,sizeof(double));
		}
	int fin=-1;
	double d=0;
	str.writeRawData((char*)&fin,sizeof(int));
	str.writeRawData((char*)&fin,sizeof(int));
	str.writeRawData((char*)&d,sizeof(double));
}
SoSPEnergy::~SoSPEnergy(){}
QString SoSPEnergy::DisplayName(){return "FUNC::AXIS::CALIBRATION "+Name();}
SoSPAxis *SoSPEnergy::XAxis(){return m_x;}
SoSPAxis *SoSPEnergy::YAxis(){return m_y;}
void SoSPEnergy::SetXAxis(SoSPAxis *ax){m_x=ax;buildexpr();changed(this);}
void SoSPEnergy::SetYAxis(SoSPAxis *ax){m_y=ax;buildexpr();changed(this);}
int SoSPEnergy::MarkerCnt(){return m_points.count();}
QPoint SoSPEnergy::Marker(int N){
	if((N<0)|(N>=m_points.count()))return QPoint(0,0);
	return m_points[N];
}
void SoSPEnergy::SetMarker(int N, QPoint p){
	if((N<0)|(N>=m_points.count())){
		if(m_points.count()<3){
			m_points.append(p);
			m_magn.append(0);
			buildexpr();
		}
	}else
		m_points.replace(N,p);
	buildexpr();
}
double SoSPEnergy::Magn(int N){
	if((N<0)|(N>=m_points.count()))return 0;
	return m_magn.at(N);
}
void SoSPEnergy::SetMagn(int N, double val){
	if((N<0)|(N>=m_points.count()))return;
	m_magn.replace(N,val);
	buildexpr();
}
void SoSPEnergy::buildexpr(){
	Set_Expression(NULL);
	if(MarkerCnt()<3)return;
	if(XAxis()==NULL)return;
	if(YAxis()==NULL)return;
	double A[9];
	double B[3];
	for(int i=0; i<3;i++){
		B[i]=m_magn[i];
		A[i*3]=1;
		QPoint p=m_points[i];
		A[i*3+1]=p.x();
		A[i*3+2]=p.y();
	}
	for(int i=0;i<3;i++){
		{
			double k=A[i*3+i];
			if(k==0){
				return;
			}
			for(int j=i;j<3;j++)A[i*3+j]/=k;
			B[i]/=k;
		}
		for(int i2=i+1; i2<3;i2++){
			double k=A[i2*3+i];//A[i*3+i];
			for(int j=i; j<3;j++){
				A[i2*3+j]-=k*A[i*3+j];
			}
			B[i2]-=k*B[i];
		}
	}
	for(int i=2; i>=0;i--){
		{
			double k=A[i*3+i];
			if(k==0){
				return;
			}
		}
		for(int i2=i-1; i2>=0;i2--){
			double k=A[i2*3+i];
			for(int j=i; j<3;j++){
				A[i2*3+j]-=k*A[i*3+j];
			}
			B[i2]-=k*B[i];
		}
	}
	Math_::Number *k0=new Math_::Number();
	Math_::Number *k1=new Math_::Number();
	Math_::Number *k2=new Math_::Number();
	k0->SetValue(B[0]);
	k1->SetValue(B[1]);
	k2->SetValue(B[2]);
	Math_::Variable *x=new Math_::Variable();
	Math_::Variable *y=new Math_::Variable();
	x->SetName(XAxis()->Name());
	y->SetName(YAxis()->Name());
	Math_::Binary *k1x=new Math_::Binary();
	k1x->SetName("*");k1x->SetOperand(k1);k1x->SetOperand2(x);
	Math_::Binary *k2y=new Math_::Binary();
	k2y->SetName("*");k2y->SetOperand(k2);k2y->SetOperand2(y);
	Math_::Binary *k1k2=new Math_::Binary();
	k1k2->SetName("+");k1k2->SetOperand(k1x);k1k2->SetOperand2(k2y);
	Math_::Binary *res=new Math_::Binary();
	res->SetName("+");res->SetOperand(k0);res->SetOperand2(k1k2);
	Set_Expression(res);
	changedexpr(res);
	changed(this);
}




///////////// constant function
SoGroupDF::SoGroupDF(SortProject *father):SObject(father){
	AddType(SOT_GroupDF);m_expr="";m_functype='s';m_analyse=QList<SoDFReader*>();
}
SoGroupDF::SoGroupDF(QDataStream &str, SortProject *father):SObject(str,father){
	AddType(SOT_GroupDF);m_expr="";m_functype='s';m_analyse=QList<SoDFReader*>();
	m_expr=readstr(str);
	str.readRawData(&m_functype,sizeof(char));
	int dn=-1;
	do{
		str.readRawData((char*)&dn,sizeof(int));
		if(dn>=0){
			AddDF((SoDFReader*)Owner()->at(dn));
		}
	}while(dn>=0);
}
void SoGroupDF::Save(QDataStream &str){
	SObject::Save(str);
	writestr(m_expr,str);
	str.writeRawData(&m_functype,sizeof(char));
	for(int i=0; i<DFCnt();i++){
		int dn=Owner()->Number(GetDF(i));
		str.writeRawData((char*)&dn,sizeof(int));
	}{
		int dn=-1;
		str.writeRawData((char*)&dn,sizeof(int));
	}
}
SoGroupDF::~SoGroupDF(){}
QString SoGroupDF::DisplayName(){return "FUNC::GROUP::"+FormulaType()+" "+Name();}
QString SoGroupDF::Expression(){return m_expr;}
void SoGroupDF::SetExpression(QString str){m_expr=str;changed(this);}
int SoGroupDF::DFCnt(){return m_analyse.count();}
SoDFReader *SoGroupDF::GetDF(int N){
	if((N<0)|(N>=m_analyse.count()))return NULL;
	return m_analyse.at(N);
}
void SoGroupDF::AddDF(SoDFReader *source){
	if(!m_analyse.contains(source))
		m_analyse.append(source);
	changed(this);
}
void SoGroupDF::RemDF(SoDFReader *source){
	if(m_analyse.contains(source))
		m_analyse.removeOne(source);
	changed(this);
}
QString SoGroupDF::FormulaType(){
	switch(m_functype){
	case 's':
		return "SUM";
	case 'q':
		return "SUM_OF_SQUARES";
	case 'a':
		return "AVERAGE";
	case 'd':
		return "VARIANCE";
	default:
		return "not set";
	}
}
void SoGroupDF::SetSumm(){m_functype='s';changed(this);}
void SoGroupDF::SetSummSq(){m_functype='q';changed(this);}
void SoGroupDF::SetAverage(){m_functype='a';changed(this);}
void SoGroupDF::SetDispersion(){m_functype='d';changed(this);}
double SoGroupDF::Value(){
	double res=0;
	switch(m_functype){
	case 's':
		for(int i=0;i<DFCnt();i++){
			double var=0;
			Owner()->GetVar(m_expr,&var,GetDF(i),NULL);
			res+=var;
		}
		break;
	case 'q':
		for(int i=0;i<DFCnt();i++){
			double var=0;
			Owner()->GetVar(m_expr,&var,GetDF(i),NULL);
			res+=var*var;
		}
		break;
	case 'a':
		for(int i=0;i<DFCnt();i++){
			double var=0;
			Owner()->GetVar(m_expr,&var,GetDF(i),NULL);
			res+=var;
		}
		if(DFCnt()>0)res/=DFCnt();
		break;
	case 'd':{
		double av=0;
		for(int i=0;i<DFCnt();i++){
			double var=0;
			Owner()->GetVar(m_expr,&var,GetDF(i),NULL);
			av+=var;
		}
		if(DFCnt()>0)av/=DFCnt();
		for(int i=0;i<DFCnt();i++){
			double var=0;
			Owner()->GetVar(m_expr,&var,GetDF(i),NULL);
			double dif=av - var;
			res+=dif*dif;
		}
		res=sqrt(res);
	}break;
	default:
		break;
	}
	return res;
}
