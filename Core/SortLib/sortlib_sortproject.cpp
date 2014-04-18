#include <SortLib.h>
#include <sortlib_.h>

#define CACHE_POSTFIX (QString(".")+sortlibversion()+".cached_results")

SObject *ReadObj(QDataStream &str,SortProject *father){
	uint type=0;
	str.readRawData((char*)&type,sizeof(uint));
	switch(type){
	case 0: return NULL;
	case SOT_ECounter:
		return new SoECounter(str,father);
	case SOT_Value:
		return new SoValue(str,father);
	case SOT_EFilter:
		return new SoEFilter(str,father);
	case SOT_Formula:
		return new SoFormula(str,father);
	case SOT_SPAxis:
		return new SoSPAxis(str,father);
	case SOT_ECSP1:
		return new SoECSP1(str,father);
	case SOT_EFWin:
		return new SoEFWin(str,father);
	case SOT_ECSP2:
		return new SoECSP2(str,father);
	case SOT_EFLoc:
		return new SoEFLoc(str,father);
	case SOT_Dependency:
		return new SoDependency(str,father);
	case SOT_DSewed:
		return new SoDSewed(str,father);
	case SOT_GroupDF:
		return new SoGroupDF(str,father);
	case SOT_SPEnergy:
		return new SoSPEnergy(str,father);
	case SOT_SoTblFuncTxt:
		return new SoTblFuncTxt(str,father);
	case SOT_SP1Norm:
		return new SP1Norm(str,father);
	case SOT_SP1Norm_CoefErr:
		return new SP1Norm_CoefErr(str,father);
	case SOT_TblTbl:
		return new SoTableOfTables(str,father);

	//registered by the user
	default:{
		SObject *res=NULL;
		for(int i=0; (i<RegisteredTypesCount())&&(res==NULL); i++){
			SoTypeReg* reg=GetRegisteredType(i);
			if(reg->Signature()==type){
				res=reg->readInstance(str,father);
			}
		}
		return res;
	}
	}
	return NULL;
}

QString readstr(QDataStream &str){
	QString m_name="";
	bool finished=false;
	while((!str.atEnd())&(!finished)){
		char c=0;
		str.readRawData(&c,1);
		if(c!=0)m_name= m_name.append(c);
		else finished=true;
	}
	return m_name;
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

///////// Project class
SortProject::SortProject(QString path, bool replace){
	m_path=path;m_data=NULL;m_count=0;m_chcnt=0;
	m_sendsignals=true;isbusy=false;blockerrors=false;
	m_dataoutmsg="";
	if(replace){
		QFile file(m_path);
		file.open(QFile::WriteOnly);
		if(file.isOpen())file.close();
	}
	lastfindtype=0;
	lastfindname="";
	lastfindobj=NULL;
}
void SortProject::Load(){
	lastfindtype=0;
	lastfindname="";
	lastfindobj=NULL;
	m_sendsignals=false;
	blockerrors=true;
	Clear();lst.clear();
	{
		QFile file(m_path);
		file.open(QIODevice::ReadOnly);
		if(file.isOpen()){
			QDataStream str(&file);
			bool err=false;
			while((!str.atEnd()) & (!err)){
				SObject *so=ReadObj(str,this);
				if(NULL!=so){
					Add(so);
				}else{
					err=true;
					error("Cannot load an SObject from project file.\nThis file has either another version or wrong format.\nMaybe some necessary plugin is missing.");
				}
			}
			file.close();
		}else{
			error("Project file not found");
			file.open(QFile::WriteOnly);
			if(file.isOpen()){
				file.close();
				error("New file created");
			}
		}
	}{
		QFile file(m_path+CACHE_POSTFIX);
		file.open(QIODevice::ReadOnly);
		if(file.isOpen()){
			QDataStream str(&file);
			bool err=false;
			while((!str.atEnd()) & (!err)){
				int index=-1;
				str.readRawData((char*)&index,sizeof(int));
				SObject* so=at(index);
				if(NULL!=so){
					so->LoadCache(str);
				}else{
					err=true;
				}
			}
			file.close();
			if(err){error("Cache loading error");}
		}
	}
	m_sendsignals=true;
	blockerrors=false;
	pr_changed(NULL);
}
void SortProject::Refresh(){
	if(lst.count()>0){
		error("Project is allready being refreshed.");
		return;
	}
	for(int i=0; i<Count();i++){
		SObject *so=at(i);
		if(so->Is(SOT_DFReader)){
			SoDFReader *dr=(SoDFReader*)so;
			lst.append(dr);
		}
	}
	for(int i=1;i<lst.count();i++){
		connect(lst.at(i-1),SIGNAL(DataReadingFinnished()),lst.at(i),SLOT(Read()));
	}
	if(lst.count()>0){
		connect(lst.at(lst.count()-1),SIGNAL(DataReadingFinnished()),this,SLOT(refr()));
		//busy(true);
		lst.at(0)->Read();
	}
}
void SortProject::refr(){
	for(int i=1;i<lst.count();i++){
		disconnect(lst.at(i-1),SIGNAL(DataReadingFinnished()),lst.at(i),SLOT(Read()));
	}
	if(lst.count()>0){
		disconnect(lst.at(lst.count()-1),SIGNAL(DataReadingFinnished()),this,SLOT(refr()));
	}
	lst.clear();
	//busy(false);
	refreshed();
}
SortProject::~SortProject(){
	m_sendsignals=false; Clear();
}
QString SortProject::Path(){return m_path;}
void SortProject::Save(){
	if(m_path!=""){
		{
			QFile file(m_path);
			file.open(QFile::WriteOnly|QFile::Truncate);
			if(file.isOpen()){
				QDataStream str(&file);
				for(int i=0; i<m_count; i++){
					m_data[i]->Save(str);
				}
				file.close();
			}
		}
		{
			QFile file(m_path+CACHE_POSTFIX);
			file.open(QFile::WriteOnly|QFile::Truncate);
			if(file.isOpen()){
				QDataStream str(&file);
				for(int i=0; i<m_count; i++){
					str.writeRawData((char*)&i,sizeof(int));
					at(i)->SaveCache(str);
				}
				file.close();
			}
		}
	}
}
int SortProject::Count(){return m_count;}
SObject *SortProject::at(int N){
	SObject *res=NULL;
	if((N>=0)&(N<m_count)){
		res=m_data[N];
	}
	return res;
}
int SortProject::Number(SObject *obj){
	int res=-1;
	for(int i=0; i<m_count; i++){
		if(m_data[i]==obj)res=i;
	}
	return res;
}
void SortProject::Add(SObject *obj){
	if(obj==NULL)return;
	SObject **olddata=m_data;
	m_data=new SObject*[m_count+1];
	if(olddata!=NULL){
	for(int i=0; i<m_count; i++)
		m_data[i]=olddata[i];
	}
	m_count++;
	m_data[m_count-1]=obj;
	if(olddata!=NULL)delete olddata;
	connect(obj,SIGNAL(changed(SObject*)),this, SLOT(obj_changed(SObject*)));
	connect(obj,SIGNAL(error(QString)),this,SLOT(catcherror(QString)));
	connect(obj,SIGNAL(busy(bool)),this,SLOT(setbusy(bool)));
	connect(obj,SIGNAL(out_1d(QString)), this, SLOT(sl_out_1d(QString)));
	connect(obj,SIGNAL(out_2d(QString)),this,SLOT(sl_out_2d(QString)));
	connect(obj,SIGNAL(out_dataitem(DataEvent*)),this,SLOT(sl_out_dataitem(DataEvent*)));
	connect(obj,SIGNAL(out_end()),this,SLOT(sl_out_end()));
	pr_changed(obj);
}
void SortProject::Remove(int N){
	//when removing is impossible
	if(NULL==m_data)return;
	if(N<0)return;
	if(N>=m_count) return;
	//dispose object being deleted correctly
	int newN=N;//objects, depended of this one remove themselves, so we should find out new index
	{
		SObject *so=m_data[N];
		if(NULL!=so){
			so->BeforeDelete();
			newN=Number(so);
			delete so;
			m_data[newN]=NULL;
		}
	}
	//delete this element from data array
	if(1==m_count){
		delete [] m_data;
		m_data=NULL;
		m_count=0;
		return;
	}
	SObject **olddata=m_data;
	m_data=new SObject*[m_count-1];
	for(int i=0; i<newN; i++)
			m_data[i]=olddata[i];
	for(int i=newN+1; i<m_count; i++)
			m_data[i-1]=olddata[i];
	m_count--;
	delete [] olddata;
	pr_changed(NULL);
}
void SortProject::Remove(SObject *so){
	int index=-1;
	for(int i=0; (i<Count())&(index<0);i++){
		if(at(i)==so)index=i;
	}
	if(index>=0)Remove(index);
}
void SortProject::Insert(SObject *obj,int N){
	if(obj==NULL)return;
	if((N<0)|(N>=m_count))
		Add(obj);
	else{
		SObject **olddata=m_data;
		m_data=new SObject*[m_count+1];
		for(int i=0; i<m_count; i++){
			if(i<N)
				m_data[i]=olddata[i];
			else
				m_data[i+1]=olddata[i];
		}
		m_count++;
		m_data[N]=obj;
		if(olddata!=NULL)delete [] olddata;
	}
	connect(obj,SIGNAL(changed(SObject*)),this, SLOT(obj_changed(SObject*)));
	connect(obj,SIGNAL(error(QString)),this,SLOT(catcherror(QString)));
	connect(obj,SIGNAL(plot(QString,SObject*)),this,SLOT(objplots(QString,SObject*)));
	pr_changed(obj);
}
void SortProject::Clear(){
	m_sendsignals=false;
	while(Count()>0)
		Remove(0);
	m_sendsignals=true;
	pr_changed(NULL);
}
SObject *SortProject::Find(uint type, QString name, Qt::CaseSensitivity sence, bool just){
	if(name=="")return NULL;
	if(name.compare(lastfindname,sence)==0)
		if(type==lastfindtype)
			return lastfindobj;
	SObject *res=NULL;
	int n=Count();
	for(int i=0; (res==NULL)&(i<n); i++){
		SObject *so=at(i);
		bool isit=false;
		if(just)isit=so->IsJust(type);else isit=so->Is(type);
		if(isit){
			if(so->Name().compare(name,sence)==0){
				res=so;
			}
		}
	}
	lastfindtype=type;
	lastfindname=name;
	lastfindobj=res;
	return res;
}

bool SortProject::GetVar(QString name, double *val, SoDFReader *dr, DataEvent *event){
	bool res=false;
	//Check if data is valid
	if(name=="")return false;
	//Look for variables in datafile and event if they are given as parameters
	if(dr!=NULL){
		res|= dr->GetVar(name,val);
		if(res)variable_getting(name,val,dr,event);
	}
	if(event!=NULL){
		res|=event->GetVar(name,val);
		if(res)variable_getting(name,val,dr,event);
	}
	if(res)return res;
	// otherwise look for such value in the project
	for(int i=0; i<Count();i++){
		SObject *so=at(i);
		if(so->Name().compare( name, Qt::CaseInsensitive)==0){
			if(so->Is(SOT_Value)){
				val[0]=((SoValue*)so)->Value();
				variable_getting(name,val,dr,event);
				res=true;
			}
			if(so->Is(SOT_Formula)){
				val[0]=((SoFormula*)so)->Value(dr,event);
				res=true;
			}
			if(so->Is(SOT_GroupDF)){
				val[0]=((SoGroupDF*)so)->Value();
				res=true;
			}
		}
	}
	return res;
}

double SortProject::GetUnary(QString name, double x, SoDFReader *dr, DataEvent *event){
	// built in operators
	if(name.compare("sign",Qt::CaseInsensitive)==0){
		if(x>0)return 1.0;
		if(x<0)return -1.0;
		return 0;
	}
	if(name.compare("abs",Qt::CaseInsensitive)==0){
		if(x>0)return x;
		if(x<0)return -x;
		return 0;
	}
	if((name.compare("-",Qt::CaseInsensitive)==0)|(name.compare("minus",Qt::CaseInsensitive)==0))
		return -x;
	if(name.compare("sqrt",Qt::CaseInsensitive)==0){
		if(x>=0)return sqrt(x);
		else{
			catcherror("Arithmetic error: attempt to calculate negative value's square root");
			return 0;
		}
	}
	if(name.compare("sqr",Qt::CaseInsensitive)==0)
		return x*x;
	if(name.compare("exp",Qt::CaseInsensitive)==0)
		return exp(x);
	if(name.compare("ln",Qt::CaseInsensitive)==0)
		return log(x);
	if(name.compare("sin",Qt::CaseInsensitive)==0)
		return sin(x);
	if(name.compare("cos",Qt::CaseInsensitive)==0)
		return cos(x);
	if(name.compare("sind",Qt::CaseInsensitive)==0)
		return sin(x*3.1415926/180);
	if(name.compare("cosd",Qt::CaseInsensitive)==0)
		return cos(x*3.1415926/180);
	// User defined operators
	SoUnaryOperator *opr=(SoUnaryOperator*)Find(SOT_Unary, name,Qt::CaseInsensitive);
	if(NULL!=opr)
		return opr->F(x,dr,event);
	// Unknown operator handling
	catcherror("Arithmetic error: unknown unary operator '"+name+"'");
	return 0;
}
double SortProject::GetBinary(QString name, double x, double y, SoDFReader *dr, DataEvent *event){
	//built in operators
	if((name.compare("+",Qt::CaseInsensitive)==0)|(name.compare("add",Qt::CaseInsensitive)==0))
		return x+y;
	if((name.compare("-",Qt::CaseInsensitive)==0)|(name.compare("sub",Qt::CaseInsensitive)==0))
		return x-y;
	if((name.compare("*",Qt::CaseInsensitive)==0)|(name.compare("mul",Qt::CaseInsensitive)==0))
		return x*y;
	if((name.compare("/",Qt::CaseInsensitive)==0)|(name.compare("div",Qt::CaseInsensitive)==0)){
		if(0.0!=y)return x/y; else {
			catcherror("Arithmetic error: division by zero");
			return 0;
		}
	}
	if((name.compare("pow",Qt::CaseInsensitive)==0)|(name.compare("^",Qt::CaseInsensitive)==0))
		return pow(x,y);
	//User defined operators
	SoBinaryOperator *opr=(SoBinaryOperator*)Find(SOT_Binary,name,Qt::CaseInsensitive);
	if(NULL!=opr)
		return opr->F(x,y,dr,event);
	//Unknown operator handling
	catcherror("Arithmetic error: unknown binary operator '"+name+"'");
	return 0;
}
void SortProject::setbusy(bool val){busy(val);isbusy=val;}
void SortProject::setstatusmsg(QString msg){statusmsg(msg);}
bool SortProject::IsBusy(){return isbusy;}
void SortProject::catcherror(QString msg){
	if(blockerrors)return;
	QDateTime time=QDateTime::currentDateTime();
	QString timestr=time.toString();
	error("["+timestr+"] "+msg);
}
void SortProject::pr_changed(SObject *so){
	lastfindtype=0;
	lastfindname="";
	lastfindobj=NULL;
	if(m_sendsignals)
		project_changed(so);
}
void SortProject::obj_changed(SObject *so){
	lastfindtype=0;
	lastfindname="";
	lastfindobj=NULL;
	if(m_sendsignals)
		object_changed(so);
}

void SortProject::SetDataOutput(IDataOutput *out){
	connect(this, SIGNAL(out_1d(QString)),out,SLOT(sl_out_1d(QString)));
	connect(this,SIGNAL(out_2d(QString)),out,SLOT(sl_out_2d(QString)));
	connect(this,SIGNAL(out_dataitem(DataEvent*)),out,SLOT(sl_out_dataitem(DataEvent*)));
	connect(this,SIGNAL(out_end()),out,SLOT(sl_out_end()));
	connect(this,SIGNAL(out_config()),out,SLOT(sl_out_config()));
	connect(this,SIGNAL(gui_close()),out,SLOT(sl_out_gui_close()));
	connect(this,SIGNAL(destroyed()),out,SLOT(deleteLater()));
	connect(out,SIGNAL(error(QString)),this, SLOT(catcherror(QString)));
	m_dataoutmsg=out->About();
}

QString SortProject::DataOutput(){return m_dataoutmsg;}

void SortProject::sl_out_1d(QString obj){
	out_1d(obj);
}
void SortProject::sl_out_2d(QString obj){
	out_2d(obj);
}
void SortProject::sl_out_dataitem(DataEvent *item){
	out_dataitem(item);
}
void SortProject::sl_out_end(){
	out_end();
}
void SortProject::sl_out_config(){
	out_config();
}

void SortProject::sl_gui_close(){
	gui_close();
}
