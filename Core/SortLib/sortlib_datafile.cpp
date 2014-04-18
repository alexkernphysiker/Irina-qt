#include <SortLib.h>

#include <QFile>
#include <QList>
#include <QDataStream>
#include <QMutexLocker>
#include <QDateTime>


/////Data Event and Data Constant implementation
DataEvent::DataEvent(int N){
	m_valid=true;
	m_freezed=false;
	m_nadc=N;
	m_nd=0;
	if(m_nadc>0)
		m_adc=new double[m_nadc];
	else
		m_adc=NULL;
	for(int i=0; i<m_nadc; i++)m_adc[i]=0;
}
DataEvent::~DataEvent(){
	if(m_adc!=NULL)delete [] m_adc;
}
void DataEvent::Freeze(){
	QMutexLocker locker(&m_mutex);
	m_freezed=true;
}
bool DataEvent::Is_Valid(){
	QMutexLocker locker(&m_mutex);
	return m_valid;
}
void DataEvent::Set_Invalid(){
	QMutexLocker locker(&m_mutex);
	if(!m_freezed)
		m_valid=false;
}
double DataEvent::ADC(int n){
	QMutexLocker locker(&m_mutex);
	int N=n;
	if((N>=0)&(N<m_nadc)){
		return m_adc[N];
	}else{
		return 0;
	}
}
void DataEvent::Set_ADC(int n, double val){
	QMutexLocker locker(&m_mutex);
	if(!m_freezed){
		if((n>=0)&(n<m_nadc)){
			m_adc[n]=val;
		}
	}
}
void DataEvent::Set_ND(int val){
	QMutexLocker locker(&m_mutex);
	if(!m_freezed){
		m_nd=val;
	}
}
int DataEvent::ND(){
	QMutexLocker locker(&m_mutex);
	return m_nd;
}
bool DataEvent::GetVar(QString name, double *val){
	bool res=false;
	if(!Is_Valid())	return true;///!!!
	if(name=="")return false;
	if(name.compare("#nd",Qt::CaseInsensitive)==0){
		val[0]=ND();
		res=true;
	}
	for(int i=0; i<m_nadc; i++)
		if(name.compare("#"+QString::number(i),Qt::CaseInsensitive)==0){
			val[0]=ADC(i);
			res=true;
		}
	return res;
}


///second level base classes
SoDFReader::LocalVariable::LocalVariable(QString name, double value){m_name=name;m_value=value;}
QString SoDFReader::LocalVariable::Name(){return m_name;}
double SoDFReader::LocalVariable::Value(){return m_value;}


/// base datafile reader class
SoDFReader::SoDFReader(QString Path, SortProject *father):SObject(father){
	m_path=Path;AddType(SOT_DFReader);m_descr="";m_nadc=0;
	m_constants=QList<LocalVariable>();
	m_variables=QList<LocalVariable>();
	connect(this, SIGNAL(statusmsg(QString)),Owner(),SLOT(setstatusmsg(QString)));
}
SoDFReader::SoDFReader(QDataStream &str, SortProject *father):SObject(str,father){
	m_path="";AddType(SOT_DFReader);m_descr="";m_nadc=0;
	m_constants= QList<LocalVariable>();
	m_variables= QList<LocalVariable>();
	connect(this, SIGNAL(statusmsg(QString)),Owner(),SLOT(setstatusmsg(QString)));
	m_path=readstr(str);
	bool finished=false;
	while((!str.atEnd())&(!finished)){
		QString vn=readstr(str);
		if(vn.compare("",Qt::CaseInsensitive)!=0){
			double val=0;
			str.readRawData((char*)&val,sizeof(double));
			ApplyVariable(vn,val);
		}else finished=true;
	}
}
QString SoDFReader::DisplayName(){
	QString res="DATAFILE ";
	if(State()==DF_Error)res+="(!!!) ";
	if(State()==DF_Warning)res+="(!) ";
	return res+Name()+" = "+m_path;
}
void SoDFReader::BeforeDelete(){
	SObject::BeforeDelete();
	if(Owner()!=NULL){
		int ind=0;//remove dependent objects
		while(ind<Owner()->Count()){
			SObject *so=Owner()->at(ind);
			if(so->Is(SOT_ECounter)){
				SoECounter *cnt=(SoECounter*)so;
				if(cnt->Source()==this)
					Owner()->Remove(ind);
				else
					ind++;
			}else{
				if(so->Is(SOT_GroupDF)){
					((SoGroupDF *)so)->RemDF(this);
				}
				ind++;
			}
		}
	}
}
SoDFReader::~SoDFReader(){
	m_constants.clear();m_variables.clear();
}
QString SoDFReader::Path(){return m_path;}
QString SoDFReader::Description(){return m_descr;}
void SoDFReader::Save(QDataStream &str){
	SObject::Save(str);
	writestr(m_path,str);
	for(int i=0; i<m_variables.size();i++){
		LocalVariable dc=m_variables.at(i);
		writestr(dc.Name(),str);
		double val=dc.Value();
		str.writeRawData((char*)&val,sizeof(double));
	}
	writestr("",str);
}
QString SoDFReader::eventsfromfile="EventsCount";
QString SoDFReader::brokeneventsfromfile="BrokenEventsCount";
ReaderAsync *SoDFReader::_getReaderAsync(QString m_path, SoDFReader *father){
	return new ReaderAsync(m_path,father);
}

void SoDFReader::Read(){
	if(Owner()->IsBusy())return;
	Set_OK();ReadingStarted();m_descr="";
	m_constants.clear();
	ReaderAsync *reader=_getReaderAsync(m_path,this);
	connect(reader,SIGNAL(FileOpened(QDataStream*)),this,SLOT(onFileOpened(QDataStream*))
			,Qt::DirectConnection);
	if(CanReadAsync()){
	connect(reader,SIGNAL(DataEventGot(DataEvent*)),this,SLOT(GetDataEvent(DataEvent*)),
			Qt::QueuedConnection);
	connect(reader,SIGNAL(DataReadingFinnished()),this,SLOT(ReportFinished()),
			Qt::QueuedConnection);
	connect(reader,SIGNAL(statusmsg(QString)),this,SLOT(setstatusmsg(QString)),
			Qt::QueuedConnection);
	}else{
		connect(reader,SIGNAL(DataEventGot(DataEvent*)),this,SLOT(GetDataEvent(DataEvent*)),
				Qt::DirectConnection);
		connect(reader,SIGNAL(DataReadingFinnished()),this,SLOT(ReportFinished()),
				Qt::DirectConnection);
	}
	reader->Open();
	if(!reader->Error()){
		busy(true);
		reader->Start();
	}else{
		Set_Error();
		error(reader->ErrMsg());
		delete reader;
		ReportFinished();
	}
}

double SoDFReader::eventsread(){return m_n_events;}
double SoDFReader::eventsbroken(){return m_br_events;}
bool SoDFReader::CanReadAsync(){return true;}
void SoDFReader::DataEventIsGot(DataEvent */*Event*/){}

void SoDFReader::onFileOpened(QDataStream *str){
	m_n_events=0;
	m_br_events=0;
	ReadHeader(*str);
	if(m_nadc==0){
		Set_Warning();
		error("NADC==0");
	}
}
void SoDFReader::GetDataEvent(DataEvent *event){
	{
		QMutexLocker locker(&m_mutex);
		DataEventIsGot(event);
	}
	if(event->Is_Valid()){
		m_n_events+=1.0;
		event->Freeze();
		QMutexLocker locker(&m_mutex);
		DataEventGot(event);
	}else {
		Set_Warning();
		m_br_events+=1.0;
	}
	delete event;
}
void SoDFReader::ReportFinished(){
	busy(false);
	{
		QMutexLocker locker(&m_mutex);
		m_constants.append(LocalVariable(eventsfromfile,m_n_events));
		m_constants.append(LocalVariable(brokeneventsfromfile,m_br_events));
		FinalDataCheck();
		DataReadingFinnished();
	}
	if(m_constants.size()>0){
		m_descr+="\nVariables from data file:\n";
		for(int i=0; i<m_constants.size(); i++){
			LocalVariable dc=m_constants.at(i);
			m_descr+=dc.Name()+" = "+QString::number(dc.Value())+"\n";
		}
	}
	changed(this);
}

void SoDFReader::reg_event(bool ok){
	if(ok){
		m_n_events++;
	}else{
		m_br_events++;
	}
}
void SoDFReader::clear_descr(){m_descr="";}
void SoDFReader::clear_constants(){m_constants.clear();}
DFState SoDFReader::State(){return m_state;}
void SoDFReader::Set_OK(){m_state=DF_OK;}
void SoDFReader::Set_Warning(){m_state=DF_Warning;}
void SoDFReader::Set_Error(){m_state=DF_Error;}
void SoDFReader::setstatusmsg(QString msg){statusmsg(msg);}
int SoDFReader::NADC(){return m_nadc;}
void SoDFReader::clear_ev_cnt(){
	m_n_events=0;m_br_events=0;
}
double SoDFReader::Constant(QString name){
	double res=0;
	for(int i=0; i<m_constants.size(); i++){
		LocalVariable dc=m_constants.at(i);
		if(dc.Name().compare( name, Qt::CaseInsensitive)==0)
			res=dc.Value();
	}
	return res;
}
double SoDFReader::Variable(QString name){
	double res=0;
	for(int i=0; i<m_variables.size(); i++){
		LocalVariable dc=m_variables.at(i);
		if(dc.Name().compare( name, Qt::CaseInsensitive)==0)
			res=dc.Value();
	}
	return res;
}
bool SoDFReader::GetVar(QString name, double *val){
	bool res=false;
	if(name=="")return false;
	for(int i=0; i<m_constants.size(); i++){
		LocalVariable dc=m_constants.at(i);
		if(dc.Name().replace(" ","_").compare( name.replace(" ","_"), Qt::CaseInsensitive)==0){
			val[0]=dc.Value();
			res=true;
		}
	}
	for(int i=0; i<m_variables.size(); i++){
		LocalVariable dc=m_variables.at(i);
		if(dc.Name().compare( name, Qt::CaseInsensitive)==0){
			val[0]=dc.Value();
			res=true;
		}
	}
	return res;
}
void SoDFReader::ApplyVariable(QString name_, double val){
	if(name_.compare("",Qt::CaseSensitive)==0)return;
	QString name=name_.replace(" ","_");
	bool ToDo=true;
	for(int i=0; i<m_variables.size(); i++){
		LocalVariable dc=m_variables.at(i);
		if(dc.Name().compare( name, Qt::CaseInsensitive)==0){
			m_variables.removeAt(i);
		}
	}
	if(ToDo)
		m_variables.append(LocalVariable(name,val));
	changed(this);
}
int SoDFReader::VarCnt(){
	return m_variables.count();
}
QString SoDFReader::VarName(int N){
	if(N<0)return "";
	if(N>=m_variables.size())return "";
	LocalVariable dc=m_variables.at(N);
	return dc.Name();
}
void SoDFReader::RemVar(int N){
	if(N<0)return;
	if(N>=m_variables.size())return;
	m_variables.removeAt(N);
	changed(this);
}

QStringList SoDFReader::ConstantNames(){
	QStringList lst;
	for(int i=0; i< m_constants.count(); i++){
		lst<<m_constants[i].Name();
	}
	return lst;
}

void SoDFReader::ApplyConstant(QString name_, double val){
	QString name=name_.replace(" ","_");
	for(int i=0; i< m_constants.count();i++){
		if(m_constants[i].Name().compare(name,Qt::CaseInsensitive)==0){
			m_constants.removeAt(i);
			i--;
		}
	}
	m_constants.append(LocalVariable(name,val));changed(this);
}
void SoDFReader::LoadCache(QDataStream &str){
	m_descr=readstr(str);
	int n=0;
	str.readRawData((char*)&n, sizeof(int));
	for(int i=0; i< n;i++){
		double val=0;
		str.readRawData((char*)&val, sizeof(double));
		QString name=readstr(str);
		ApplyConstant(name,val);
	}
}
void SoDFReader::SaveCache(QDataStream &str){
	writestr(m_descr,str);
	int n=m_constants.count();
	str.writeRawData((char*)&n, sizeof(int));
	for(int i=0; i< n;i++){
		double val=m_constants[i].Value();
		str.writeRawData((char*)&val, sizeof(double));
		writestr(m_constants[i].Name(),str);
	}
}
void SoDFReader::ReadHeader(QDataStream &/*datastr*/){}///is to read file headers and fill constants from there and set NADC
void SoDFReader::Set_NADC(int val){m_nadc=val;}
void SoDFReader::FinalDataCheck(){m_descr+="\nNumber of ADC: "+QString::number(NADC());}
void SoDFReader::Set_description(QString descr){m_descr=descr;changed(this);}


/// class for data reading in second thread
bool ReaderAsync::Error(){return !valid;}
QString ReaderAsync::ErrMsg(){return msg;}
ReaderAsync::ReaderAsync(SoDFReader *father){
	datastr=NULL;
	file=NULL;
	valid=true;
	msg="";
	master=father;
}
ReaderAsync::ReaderAsync(QString m_path, SoDFReader *father){
	datastr=NULL;
	file=NULL;
	master=father;
	valid=false;
	msg="";
	file=new QFile(m_path);
}
ReaderAsync::~ReaderAsync(){
	if(file!=NULL)
		file->close();
	if(datastr!=NULL)
		delete datastr;
	if(file!=NULL)
		delete file;
}
void ReaderAsync::Open(){
		file->open(QIODevice::ReadOnly);
		if(file->isOpen()){
			valid=true;
			datastr=new QDataStream(file);
			FileOpened(datastr);
		}else {
			msg="Could not open file";
		}
}

void ReaderAsync::Start(){
	if(master->CanReadAsync()){
		QThread *thr=new QThread();
		moveToThread(thr);
		connect(thr,SIGNAL(started()),this,SLOT(process()));
		connect(this,SIGNAL(finnished()),thr,SLOT(quit()));
		connect(this,SIGNAL(finnished()),this,SLOT(deleteLater()));
		connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
		thr->start();
	}else{
		process();
	}
}
void ReaderAsync::process(){
	statusmsg(master->DisplayName()+" in process");
	QTime timer;
	timer.start();
	long i=0;
	while(!datastr->atEnd()){
		DataEvent *event=master->NextEvent(*datastr);
		DataEventGot(event);
		i++;
		if(master->CanReadAsync()){
			if(timer.elapsed()>1000){
				statusmsg(master->DisplayName()+": "+
						  QString::number(i)+" events");
				timer.start();
			}
		}
	}
	DataReadingFinnished();
	statusmsg(master->DisplayName()+" finished");
	finnished();
}

SoDFReader *ReaderAsync::Owner(){return master;}
QDataStream *ReaderAsync::Stream(){return datastr;}
