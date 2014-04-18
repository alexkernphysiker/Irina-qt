#include <SortLib.h>
////////////// simple event counter
SoECounter::SoECounter(SoDFReader *source):SObject(source->Owner()){
	AddType( SOT_ECounter);m_count=0;m_filt_cnt=0;m_filters=NULL;
	m_reader=source;
	Connect_to();
}
SoECounter::SoECounter(QDataStream &str, SortProject *father):SObject(str,father){
	AddType( SOT_ECounter);m_filt_cnt=0;
	int readernumber=0;m_count=0;m_filters=NULL;
	str.readRawData((char*)&readernumber, sizeof(int));
	SObject *obj=Owner()->at(readernumber);
	if(obj->Is(SOT_DFReader)){
		m_reader=(SoDFReader*)obj;
		Connect_to();
	}
	bool finnished=false;
	while((!finnished)&(!str.atEnd())){
		int filid=-1;
		str.readRawData((char*)&filid,sizeof(int));
		if(filid<0)
			finnished=true;
		else{
			SObject *so=Owner()->at(filid);
			if(so!=NULL){
				if(so->Is(SOT_EFilter))
					AddFilter((SoEFilter*)so);
			}
		}
	}
}
void SoECounter::Save(QDataStream &str){
	SObject::Save(str);
	int readernumber=Owner()->Number(m_reader);
	str.writeRawData((char*)&readernumber,sizeof(int));
	int filid=-1;
	for(uint i=0; i<m_filt_cnt; i++){
		filid=Owner()->Number(m_filters[i]);
		str.writeRawData((char*)&filid,sizeof(int));
	}
	filid=-1;
	str.writeRawData((char*)&filid,sizeof(int));
}
SoECounter::~SoECounter(){
	if(m_filters!=NULL)delete m_filters;
}
int SoECounter::FilterCnt(){
	return m_filt_cnt;
}
SoEFilter *SoECounter::Filter(int N){
	if(m_filters==NULL)return NULL;
	if(N<0)return NULL;
	if(N>=(int)m_filt_cnt)return NULL;
	return m_filters[N];
}
void SoECounter::AddFilter(SoEFilter *filter){
	if(filter==NULL)return;
	if(FilterInd(filter)>=0)return;
	SoEFilter **old=m_filters;
	m_filters=new SoEFilter*[m_filt_cnt+1];
	for(uint i=0; i<m_filt_cnt; i++)
		m_filters[i]=old[i];
	m_filt_cnt++;
	m_filters[m_filt_cnt-1]=filter;
	if(old!=NULL)delete old;
	changed(this);
}
int SoECounter::FilterInd(SoEFilter *filter){
	int res=-1;
	for(int i=0; i<(int)m_filt_cnt; i++)
		if(Filter(i)==filter)
			res=i;
	return res;
}
void SoECounter::DelFilterAt(int N){
	if(m_filters==NULL)return;
	if(N<0)return;
	if(N>=(int)m_filt_cnt)return;
	if(m_filt_cnt==1){
		delete m_filters;
		m_filters=NULL;
		m_filt_cnt=0;
		return;
	}
	SoEFilter **old=m_filters;
	m_filters=new SoEFilter*[m_filt_cnt-1];
	for(int i=0; i<N; i++)
		m_filters[i]=old[i];
	for(uint i=N+1; i<m_filt_cnt;i++)
		m_filters[i-1]=old[i];
	m_filt_cnt--;
	delete old;
	changed(this);
}
void SoECounter::Connect_to(){
	connect(m_reader,SIGNAL(ReadingStarted()),this,SLOT(ResetData()));
	connect(m_reader,SIGNAL(DataEventGot(DataEvent*)),this,SLOT(DataEventAccepted(DataEvent*)));
	connect(m_reader,SIGNAL(DataReadingFinnished()),	this,SLOT(AllDataAccepted()));
	connect(m_reader,SIGNAL(statusmsg(QString)),this,SLOT(status(QString)));
}
SoDFReader *SoECounter::Source(){return m_reader;}
int SoECounter::EventsCount(){return m_count;}
void SoECounter::ResetData(){
	m_count=0;DEInit();
}
void SoECounter::DataEventAccepted(DataEvent *event){
	if(event!=NULL)
	if(DEAccepted(event)){
		m_count++;
	}
}
void SoECounter::status(QString){
	changed(this);
	Refreshed(this);
}
void SoECounter::AllDataAccepted(){
	DEFinal();
	double val=m_count;
	if(Name()!=""){
		m_reader->ApplyVariable("$"+Name(),val);
		m_reader->ApplyVariable("$"+Name()+" ERROR",sqrt(val));
	}
	changed(this);
	Refreshed(this);
}
void SoECounter::DEInit(){}
void SoECounter::DEFinal(){}
bool SoECounter::DEAccepted(DataEvent *event){
	bool res=true;
	for(uint i=0; i< m_filt_cnt; i++)
		res&=(m_filters[i]->In(m_reader,event));
	return res;
}
QString SoECounter::DisplayName(){
	QString msg= "COUNTER "+Name();
	return msg;
}
SObject *SoECounter::DisplParrent(){return Source();}
void SoECounter::LoadCache(QDataStream &str){
	str.readRawData((char*)&m_count,sizeof(m_count));
	AllDataAccepted();
}
void SoECounter::SaveCache(QDataStream &str){
	str.writeRawData((char*)&m_count,sizeof(m_count));
}

//////////////// sp1
SoECSP1::SoECSP1(SoDFReader *reader):SoECounter(reader){
	AddType(1);m_axis=NULL;m_data=NULL;m_peaks=QList<Peak*>();
}
SoECSP1::SoECSP1(QDataStream &str, SortProject *father):SoECounter(str,father){
	AddType(1);m_axis=NULL;m_data=NULL;m_peaks=QList<Peak*>();
	int an=-1;
	str.readRawData((char*)&an,sizeof(int));
	SObject *so=Owner()->at(an);
	if(so!=NULL)if(so->Is(SOT_SPAxis))
		SetAxis((SoSPAxis*)so);
	char ch=0;
	do{
		str.readRawData(&ch,sizeof(char));
		if(ch!=0)
			m_peaks.append(new Peak(str,this));
	}while(ch!=0);
}
void SoECSP1::Save(QDataStream &str){
	SoECounter::Save(str);
	int an=-1;
	if(m_axis!=NULL)an=Owner()->Number(m_axis);
	str.writeRawData((char*)&an,sizeof(int));
	for(int i=0; i<m_peaks.count();i++){
		m_peaks.at(i)->Save(str);
	}
	char ch=0;
	str.writeRawData(&ch,sizeof(char));
}
void SoECSP1::LoadCache(QDataStream &str){
	if(m_axis!=NULL){
		int n=m_axis->ChannelCount();
		str.readRawData((char*)m_data, n*sizeof(uint));
	}
	SoECounter::LoadCache(str);
}
void SoECSP1::SaveCache(QDataStream &str){
	if(m_axis!=NULL){
		int n=m_axis->ChannelCount();
		str.writeRawData((char*)m_data, n*sizeof(uint));
	}
	SoECounter::SaveCache(str);
}
SoECSP1::~SoECSP1(){
	if(m_data!=NULL)delete [] m_data;
	while(m_peaks.count()>0){
		Peak *peak=m_peaks.at(0);
		m_peaks.removeAt(0);
		delete peak;
	}
}
QString SoECSP1::DisplayName(){return "SP1 "+Name();}
SoSPAxis *SoECSP1::Axis(){return m_axis;}
void SoECSP1::SetAxis(SoSPAxis *axis){
	if(m_axis!=NULL)
		disconnect(m_axis,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	m_axis=axis;
	if(m_axis!=NULL)
		connect(m_axis,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
}
void SoECSP1::AxisChanged(){
	if(m_data!=NULL)delete [] m_data;
	m_data=NULL;
	if(m_axis==NULL)return;
	m_data=new uint[m_axis->ChannelCount()];
	changed(this);
}
int SoECSP1::ChCnt(int channel){
	if(channel<0)return 0;
	if(channel>=m_axis->ChannelCount())return 0;
	return m_data[channel];
}
void SoECSP1::DEInit(){
	SoECounter::DEInit();
	if(m_axis!=NULL){
		for(int i=0; i<m_axis->ChannelCount();i++)
			m_data[i]=0;
	}
}
bool SoECSP1::DEAccepted(DataEvent *event){
	if(SoECounter::DEAccepted(event)){
		if(m_axis!=NULL){
			int chn=m_axis->getChannel(Source(),event);
			if((chn>=0)&(chn<m_axis->ChannelCount()))
				m_data[chn]++;
			return true;
		}else return false;
	}else return false;
}
void SoECSP1::DEFinal(){
	SoECounter::DEFinal();
	for(int i=0; i<m_peaks.count();i++){
		Peak *peak=m_peaks.at(i);
		if(peak->Name()!=""){
			Source()->ApplyVariable("$"+peak->Name(),double(peak->S()));
			Source()->ApplyVariable("$"+peak->Name()+" ERROR",(double(peak->dS())));
		}
	}
}
int SoECSP1::PeakCnt(){
	return m_peaks.count();
}
void SoECSP1::AddPeak(QString name){
	m_peaks.append(new Peak(name,this));
	changed(this);
}
Peak *SoECSP1::getPeak(int N){
	if(N<0)return NULL;
	if(N>=m_peaks.count())return NULL;
	return m_peaks.at(N);
}
void SoECSP1::RemPeak(Peak *peak){
	m_peaks.removeOne(peak);
	changed(this);
}
void SoECSP1::Export(){
	if(Axis()!=NULL){
		out_1d(QString(Source()->Path()+Source()->Name()+"-SP1"+Name()));
		for(int i=0; i<Axis()->ChannelCount();i++){
			double cnt=ChCnt(i);
			double err=sqrt(cnt);
			DataEvent* item=new DataEvent(4);
			item->Set_ADC(0,Axis()->GetMagnitude(i));
			item->Set_ADC(1,cnt);
			item->Set_ADC(2,Axis()->ChannelW());
			item->Set_ADC(3,err);
			item->Freeze();
			out_dataitem(item);
		}
		out_end();
	}else error("Cannot export spectrum without configured axis");
}

//////////////////////// sp1 peak
Peak::Peak(QString name, SoECSP1 *sp1){
	m_sp1=sp1;m1=0;m2=0;m_name=name;
}
Peak::Peak(QDataStream &str, SoECSP1 *sp1){
	m_sp1=sp1;
	str.readRawData((char*)&m1,sizeof(int));
	str.readRawData((char*)&m2,sizeof(int));
	m_name=readstr(str);
}
void Peak::Save(QDataStream &str){
	char c=1;
	str.writeRawData(&c,sizeof(char));
	str.writeRawData((char*)&m1,sizeof(int));
	str.writeRawData((char*)&m2,sizeof(int));
	writestr(m_name,str);
}
Peak::~Peak(){}
int Peak::M1(){return m1;}
int Peak::M2(){return m2;}
void Peak::SetM1(int m){
	if(m<0)return;
	if(m>65535)return;
	m1=m;
	if(m2<m1)m2=m;
}
void Peak::SetM2(int m){
	if(m<0)return;
	if(m>65535)return;
	m2=m;
	if(m2<m1)m1=m;
}
double Peak::S(){
	double res=0;
	for(int i=m1; i<=m2;i++){
		res=res+ m_sp1->ChCnt(i);
	}
	double foot=(m2-m1+1);
	foot*=(m_sp1->ChCnt(m1)+m_sp1->ChCnt(m2));
	foot/=2;
	return res-foot;
}
double Peak::dS(){
	double res=0;
	for(int i=m1; i<=m2;i++){
		res=res+ m_sp1->ChCnt(i);
	}
	double foot=(m2-m1+1);
	foot*=(m_sp1->ChCnt(m1)+m_sp1->ChCnt(m2));
	foot/=2;
	return sqrt(res)+sqrt(foot);
}
QString Peak::Name(){return m_name;}
void Peak::SetName(QString name){
	m_name=name;
}




///////////////   sp2
SoECSP2::SoECSP2(SoDFReader *reader):SoECounter(reader){
	AddType(2);m_xax=NULL;m_yax=NULL;m_data=NULL;
}
SoECSP2::SoECSP2(QDataStream &str, SortProject *father):SoECounter(str,father){
	AddType(2);m_xax=NULL;m_yax=NULL;m_data=NULL;
	int an=-1;
	str.readRawData((char*)&an,sizeof(int));
	if(an>=0)SetXAxis((SoSPAxis*)Owner()->at(an));
	an=-1;
	str.readRawData((char*)&an,sizeof(int));
	if(an>=0)SetYAxis((SoSPAxis*)Owner()->at(an));
}
void SoECSP2::Save(QDataStream &str){
	SoECounter::Save(str);
	int an=-1;
	if(XAxis()!=NULL)an=Owner()->Number(XAxis());
	str.writeRawData((char*)&an,sizeof(int));
	an=-1;
	if(YAxis()!=NULL)an=Owner()->Number(YAxis());
	str.writeRawData((char*)&an,sizeof(int));
}
void SoECSP2::LoadCache(QDataStream &str){
	if((m_xax!=NULL)&&(m_yax!=NULL)){
		int n=m_xax->ChannelCount()*m_yax->ChannelCount();
		str.readRawData((char*)m_data, n*sizeof(uint));
	}
	SoECounter::LoadCache(str);
}
void SoECSP2::SaveCache(QDataStream &str){
	if((m_xax!=NULL)&&(m_yax!=NULL)){
		int n=m_xax->ChannelCount()*m_yax->ChannelCount();
		str.writeRawData((char*)m_data, n*sizeof(uint));
	}
	SoECounter::SaveCache(str);
}
SoECSP2::~SoECSP2(){if(m_data!=NULL)delete [] m_data;}
QString SoECSP2::DisplayName(){return "SP2 "+Name();}

void SoECSP2::AxisChanged(){
	if(m_data!=NULL)delete [] m_data;
	m_data=NULL;
	if(m_xax==NULL)return;
	if(m_yax==NULL)return;
	int size=m_xax->ChannelCount()*m_yax->ChannelCount();
	m_data=new uint[size];
	for(int i=0; i<size; i++)
		m_data[i]=0;
	changed(this);
}
SoSPAxis *SoECSP2::XAxis(){return m_xax;}
SoSPAxis *SoECSP2::YAxis(){return m_yax;}
void SoECSP2::SetXAxis(SoSPAxis *ax){
	if(XAxis()!=NULL)
		disconnect(XAxis(),SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	m_xax=ax;
	connect(ax,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
}
void SoECSP2::SetYAxis(SoSPAxis *ax){
	if(YAxis()!=NULL)
		disconnect(YAxis(),SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	m_yax=ax;
	connect(ax,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
}
int SoECSP2::Cnt(int xch, int ych){
	if((m_xax==NULL)|(m_yax==NULL))return 0;
	if((xch<0)|(ych<0))return 0;
	if(xch>=XAxis()->ChannelCount())return 0;
	if(ych>=YAxis()->ChannelCount())return 0;
	int ind=ych*XAxis()->ChannelCount()+xch;
	return m_data[ind];
}
void SoECSP2::AddCnt(int xch, int ych){
	if((m_xax==NULL)|(m_yax==NULL))return;
	if((xch<0)|(ych<0))return;
	if(xch>=XAxis()->ChannelCount())return;
	if(ych>=YAxis()->ChannelCount())return;
	int ind=ych*XAxis()->ChannelCount()+xch;
	m_data[ind]++;
}
void SoECSP2::DEInit(){
	SoECounter::DEInit();
	if((m_xax==NULL)|(m_yax==NULL))return;
	int size=m_xax->ChannelCount()*m_yax->ChannelCount();
	for(int i=0; i<size; i++)
		m_data[i]=0;
}
bool SoECSP2::DEAccepted(DataEvent *event){
	if((m_xax==NULL)|(m_yax==NULL))return false;
	if(SoECounter::DEAccepted(event)){
		int x=XAxis()->getChannel(Source(),event);
		int y=YAxis()->getChannel(Source(),event);
		if((x>0)&(x<XAxis()->ChannelCount()-1)&(y>0)&(y<YAxis()->ChannelCount()-1))
			AddCnt(x,y);
		return true;
	}else return false;
}
void SoECSP2::DEFinal(){
	SoECounter::DEFinal();
}
void SoECSP2::Export(){
	if((XAxis()!=NULL)&(YAxis()!=NULL)){
		out_2d(QString(Source()->Path()+Source()->Name()+"-SP2"+Name()));
		{
			int y_cnt=YAxis()->ChannelCount();
			int x_cnt=XAxis()->ChannelCount();
			{
				DataEvent *item=new DataEvent(3);
				item->Set_ADC(0,XAxis()->Min());
				item->Set_ADC(1,XAxis()->ChannelW());
				item->Set_ADC(2,XAxis()->Max());
				item->Freeze();
				out_dataitem(item);
			}
			{
				DataEvent *item=new DataEvent(3);
				item->Set_ADC(0,YAxis()->Min());
				item->Set_ADC(1,YAxis()->ChannelW());
				item->Set_ADC(2,YAxis()->Max());
				item->Freeze();
				out_dataitem(item);
			}
			for(int y=0; y<y_cnt;y++){
				for(int x=0;x<x_cnt;x++){
					DataEvent *item=new DataEvent(1);
					item->Set_ADC(0,Cnt(x,y));
					item->Freeze();
					out_dataitem(item);
				}
				out_end();
			}
		}
		out_end();
	}else error("Cannot export 2D spectrum without configured axes");
}
