// this file is distributed under
// GPL v 3.0 license
#include <SortLib.h>

////////   simple event filter
SoEFilter::SoEFilter(SortProject *father):SObject(father){AddType(SOT_EFilter);m_nd=0;}
SoEFilter::SoEFilter(QDataStream &str, SortProject *father):SObject(str,father){
	AddType(SOT_EFilter);
	str.readRawData((char*)&m_nd, sizeof(int));
}
SoEFilter::~SoEFilter(){}
void SoEFilter::Save(QDataStream &str){
	SObject::Save(str);
	str.writeRawData((char*)&m_nd, sizeof(int));
}
bool SoEFilter::In(SoDFReader *, DataEvent *event){return event->ND()==ND();}
void SoEFilter::BeforeDelete(){
	SObject::BeforeDelete();
	if(Owner()!=NULL){
		int ind=0;//remove dependent objects
		while(ind<Owner()->Count()){
			SObject *so=Owner()->at(ind);
			if(so->Is(SOT_ECounter)){
				SoECounter *ec=(SoECounter*)so;
				ec->DelFilterAt(ec->FilterInd(this));
			}
			ind++;
		}
	}
}
QString SoEFilter::DisplayName(){return "FILTER "+Name()+" ND = "+QString::number(ND());}
SObject *SoEFWin::DisplParrent(){return Axis();}
int SoEFilter::ND(){return m_nd;}
void SoEFilter::SetND(int val){m_nd=val;changed(this);}



///////////// sp1 window
SoSPAxis *SoEFWin::Axis(){return m_axis;}
SoEFWin::~SoEFWin(){
	if(m_data!=NULL)delete [] m_data;
}
void SoEFWin::AxisChanged(){
	if(m_data!=NULL)delete [] m_data;
	m_data=NULL;
	if(m_axis==NULL)return;
	m_data=new bool[m_axis->ChannelCount()];
	for(int i=0; i<m_axis->ChannelCount(); i++)
		m_data[i]=false;
	changed(this);
}
bool SoEFWin::In(int ch){
	if(m_data==NULL)return false;
	if(m_axis==NULL)return false;
	bool res=false;
	if((ch>=0)&(ch<m_axis->ChannelCount()))
		res=m_data[ch];
	return res;
}
void SoEFWin::SetIn(int ch, bool val){
	if(m_data==NULL)return;
	if(m_axis==NULL)return;
	if((ch>=0)&(ch<m_axis->ChannelCount()))
		m_data[ch]=val;
	//signal about changes is not send, because it will be too frequent
}
bool SoEFWin::In(SoDFReader *, DataEvent *event){
	if(m_axis==NULL)return false;
	int ch=m_axis->getChannel(NULL,event);
	return In(ch);
}
QString SoEFWin::DisplayName(){
	return "FILTER::SP1WIN "+Name();
}
SoEFWin::SoEFWin(SoSPAxis *axis):SoEFilter(axis->Owner()){
	AddType(1);m_data=NULL;	m_axis=axis;
	connect(axis,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
}
SoEFWin::SoEFWin(QDataStream &str, SortProject *father):SoEFilter(str,father){
	AddType(1);m_axis=NULL;m_data=NULL;
	int an=-1;
	str.readRawData((char*)&an,sizeof(int));
	SObject *so=Owner()->at(an);
	if((so!=NULL)&(so->Is(SOT_SPAxis)))
		m_axis=(SoSPAxis*)so;
	connect(m_axis,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
	int ch=-1;
	do{
		str.readRawData((char*)&ch,sizeof(int));
		if(ch>=0)SetIn(ch,true);
	}while(ch>=0);
}
void SoEFWin::Save(QDataStream &str){
	SoEFilter::Save(str);
	int ch=-1;
	if(m_axis!=NULL)ch=Owner()->Number(m_axis);
	str.writeRawData((char*)&ch,sizeof(int));
	if(m_axis!=NULL)
		for(int i=0; i<m_axis->ChannelCount(); i++)
			if(In(i))
				str.writeRawData((char*)&i,sizeof(int));
	ch=-1;
	str.writeRawData((char*)&ch,sizeof(int));
}




//////////// sp2 locus
SoEFLoc::SoEFLoc(SortProject *father):SoEFilter(father){
	AddType(2);m_data=NULL;m_xax=NULL;m_yax=NULL;
}
SoEFLoc::SoEFLoc(QDataStream &str, SortProject *father):SoEFilter(str,father){
	AddType(2);m_data=NULL;m_xax=NULL;m_yax=NULL;
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
		if((x>=0)&(y>=0))
			SetIn(x,y,true);
	}while((x>=0)&(y>=0));
}
void SoEFLoc::Save(QDataStream &str){
	SoEFilter::Save(str);
	int an=-1;
	if(XAxis()!=NULL)an=Owner()->Number(XAxis());
	str.writeRawData((char*)&an,sizeof(int));
	an=-1;
	if(YAxis()!=NULL)an=Owner()->Number(YAxis());
	str.writeRawData((char*)&an,sizeof(int));
	if((XAxis()!=NULL)&(YAxis()!=NULL))
		for(int y=0; y<YAxis()->ChannelCount();y++)
			for(int x=0; x<XAxis()->ChannelCount();x++)
				if(In(x,y)){
					str.writeRawData((char*)&x,sizeof(int));
					str.writeRawData((char*)&y,sizeof(int));
				}
	int fin=-1;
	str.writeRawData((char*)&fin,sizeof(int));
	str.writeRawData((char*)&fin,sizeof(int));
}
SoEFLoc::~SoEFLoc(){if(m_data!=NULL)delete [] m_data;}
QString SoEFLoc::DisplayName(){return "FILTER::SP2LOC "+Name();}
bool SoEFLoc::In(SoDFReader *, DataEvent *event){
	if((XAxis()==NULL)|(YAxis()==NULL))return false;
	int x=XAxis()->getChannel(NULL,event);
	int y=YAxis()->getChannel(NULL,event);
	return In(x,y);
}
bool SoEFLoc::In(int x, int y){
	if((XAxis()==NULL)|(YAxis()==NULL))return false;
	if((x<0)|(y<0))return false;
	if(x>=XAxis()->ChannelCount())return false;
	if(y>=YAxis()->ChannelCount())return false;
	int ind=y*XAxis()->ChannelCount()+x;
	return m_data[ind];
}
void SoEFLoc::SetIn(int x, int y, bool val){
	if((XAxis()==NULL)|(YAxis()==NULL))return;
	if((x<0)|(y<0))return;
	if(x>=XAxis()->ChannelCount())return;
	if(y>=YAxis()->ChannelCount())return;
	int ind=y*XAxis()->ChannelCount()+x;
	m_data[ind]=val;
}
void SoEFLoc::AxisChanged(){
	if(m_data!=NULL)delete [] m_data;
	m_data=NULL;
	if(m_xax==NULL)return;
	if(m_yax==NULL)return;
	int cnt=m_xax->ChannelCount()*m_yax->ChannelCount();
	m_data=new bool[cnt];
	for(int i=0; i<cnt; i++)
		m_data[i]=false;
	changed(this);
}
SoSPAxis *SoEFLoc::XAxis(){return m_xax;}
SoSPAxis *SoEFLoc::YAxis(){return m_yax;}
void SoEFLoc::SetXAxis(SoSPAxis *ax){
	if(XAxis()!=NULL)
		disconnect(XAxis(),SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	m_xax=ax;
	connect(ax,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
}
void SoEFLoc::SetYAxis(SoSPAxis *ax){
	if(YAxis()!=NULL)
		disconnect(YAxis(),SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	m_yax=ax;
	connect(ax,SIGNAL(AxisChanged()),this,SLOT(AxisChanged()));
	AxisChanged();
}
