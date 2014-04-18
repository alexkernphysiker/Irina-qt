#include "KINR.h"
#include <QMutexLocker>

QString headereventsmsg="Header_events";
SoD00V::SoD00V(QString Path, SortProject *father):SoDFReader(Path,father){AddType( 1);AddType( 1);}
SoD00V::SoD00V(QDataStream &str, SortProject *father):SoDFReader(str,father){AddType( 1);AddType( 1);}
void SoD00V::ReadHeader(QDataStream &datastr){
	QMutexLocker locker(&readmutex);
	char tmp[4];
	for(int i=0;i<252;i++){
		char c=0;
		datastr.readRawData(&c,1);
		Set_description(Description()+ QChar(c));
	}
	datastr.readRawData(tmp,2);
	datastr.readRawData(tmp,1);
	Set_NADC( tmp[0]);
	datastr.readRawData(tmp,1);
	char nmon=tmp[0]*4;
	if(nmon>63){
		nmon=63;
		Set_Warning();
		error("Data header contains wrong data about monitor detectors count.");
	}
	for(int i=0; i<63; i++){
		uint cnt=0;
		datastr.readRawData((char*)&cnt,4);
		double val=cnt;
		if(i<nmon)
			ApplyConstant("M#"+QString::number(i),val);
	}
	m_events_header=0;
	datastr.readRawData((char*)&m_events_header,4);
}
DataEvent *SoD00V::NextEvent(QDataStream &datastr){
	QMutexLocker locker(&readmutex);
	DataEvent *res=new DataEvent(NADC());
	for(char i=0; i<NADC(); i++){
		if(!datastr.atEnd()){
			int val=0;
			datastr.readRawData((char*)&val,2);
			res->Set_ADC(i,val);
		}else res->Set_Invalid();
	}
	if(!datastr.atEnd()){
		int val=0;
		datastr.readRawData((char*)&val,2);
		res->Set_ND(val);
	}else res->Set_Invalid();
	return res;
}
void SoD00V::FinalDataCheck(){
	QMutexLocker locker(&readmutex);
	SoDFReader::FinalDataCheck();
	double evhead=m_events_header;
	ApplyConstant(headereventsmsg,evhead);
	if(evhead!=eventsread()){
		Set_Warning();
	}
}

QString RADIX50=" ABCDEFGHIJKLMNOPQRSTUVWXYZ$.*0123456789";
SoD00Z::SoD00Z(QString Path, SortProject *father):SoDFReader(Path,father){AddType( 1);AddType( 2);}
SoD00Z::SoD00Z(QDataStream &str, SortProject *father):SoDFReader(str,father){AddType( 1);AddType( 2);}
void SoD00Z::ReadHeader(QDataStream &datastr){
	QMutexLocker locker(&readmutex);
	char *buf=new char[512];
	datastr.readRawData(buf,512);
	int formatid=((int)(buf[10])+256*((int)buf[11]));
	if(formatid!=2){
		error("Wrond datafile format descriptor");
		Set_Error();
	}
	recordwidth=(((int)buf[8])+256*((int)buf[9]));
	blocklength=(((int)buf[14])+256*((int)buf[15]));
	recsperblock=blocklength/recordwidth;
	blocktag=blocklength%recordwidth;
	numblocks=(((int)buf[18])+256*((int)buf[19]));
	lastblocklength=(((int)buf[20])+256*((int)buf[21]));
	count=recsperblock*numblocks-(recsperblock-lastblocklength);
	Set_NADC((char)(recordwidth-1));
	char cntcnt=buf[134];
	for(int i=0;i<cntcnt;i++){
		uint v=((uint)buf[136+i*4])+
			((uint)buf[136+i*4+1])*256+
			((uint)buf[136+i*4+2])*65536+
			((uint)buf[136+i*4+3])*256*65536;
		double val=v;
		ApplyConstant("M#"+QString::number(i),val);
	}
	int textlen=buf[232];
	QString text="";
	for(int i=0; (i< (textlen+1)/2) && (i<10); i++){
		int data=
			((int)buf[234+i*2])+
			((int)buf[234+i*2])*256;
		int basa=RADIX50.length();
		text+=RADIX50[(data%(basa*basa*basa))/(basa*basa)];
		text+=RADIX50[(data%(basa*basa))/basa];
		text+=RADIX50[data%basa];
	}
	Set_description(Description()+ text);
	cnt=0;
	blockcnt=0;
	delete [] buf;
}

DataEvent *SoD00Z::NextEvent(QDataStream &datastr){
	QMutexLocker locker(&readmutex);
	DataEvent *res=new DataEvent(NADC());
	for(char i=0; i<NADC(); i++){
		if(!datastr.atEnd()){
			int val=0;
			datastr.readRawData((char*)&val,2);
			res->Set_ADC(i,val);
		}else res->Set_Invalid();
	}
	if(!datastr.atEnd()){
		int val=0;
		datastr.readRawData((char*)&val,2);
		res->Set_ND(val);
	}else res->Set_Invalid();
	cnt++;
	if(blockcnt<(numblocks-1)){
		if((cnt%recsperblock)==0){
			if(blocktag>0){
				char *buf=new char[blocktag];
				datastr.readRawData(buf,blocktag);
				delete [] buf;
			}
			blockcnt++;
		}
	}else{//this block is the last one
		if(cnt>=count){
			char c=0;
			while(!datastr.atEnd())
				datastr.readRawData(&c,1);
		}
	}
	return res;
}
void SoD00Z::FinalDataCheck(){
	QMutexLocker locker(&readmutex);
	SoDFReader::FinalDataCheck();
	double evhead=count;
	ApplyConstant(headereventsmsg,evhead);
	if(evhead!=eventsread()){
		Set_Warning();
	}
}
