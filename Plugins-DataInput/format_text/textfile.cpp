// this file is distributed under
// GPL v 3.0 license
#include "textfile.h"
TxtData::TxtData(QString Path, SortProject *father):SoDFReader(Path,father){AddType( 2);Set_NADC(1);}
TxtData::TxtData(QDataStream &str, SortProject *father):SoDFReader(str,father){AddType(2);Set_NADC(1);}

DataEvent *TxtData::NextEvent(QDataStream &datastr){
	QString line="";
	bool finish=false;
	// This would be better to be done with using QTextStream
	// but SoDFReader (base data reading class) is designed
	// with using QDataStream for providing reading binary formats
	while((!finish)&&(!datastr.atEnd())){
		char c=0;
		datastr.readRawData(&c,1);
		line+=c;
		if((line.indexOf("\n")>=0)){
			line=line.replace("\n","");
			if(line.length()>0)
				finish=true;
		}
	}
	//split line into evaluable items
	QStringList items1=line.replace(",",".").split('\t',QString::SkipEmptyParts,Qt::CaseInsensitive);
	QStringList items;
	for(int i=0; i<items1.count();i++)
		items.append(items1.at(i).split(' ',QString::SkipEmptyParts,Qt::CaseInsensitive));
	char n=0;
	{
		int cnt=items.length();
		if(cnt<0)cnt=0;
		if(cnt>127)cnt=127;
		n=cnt;
	}
	double *x = new double[n];
	for(char i=0;i<n;i++)x[int(i)]=0;
	bool ok;
	for(int i=0; i<n;i++){
		QString num=items.at(i).toLower();
		if(num[0]=='.')num="0"+num;
		x[i]=num.toDouble(&ok);
		if(!ok){
			x[i]=INFINITY;
		}
	}
	if(eventsread()==0){//get column number
		Set_NADC(n);
	}else{//check if items count is equal to column number
		if(n!=NADC())
			error("Some event has items count different from general ("+QString::number(n)+")");
	}
	DataEvent *res=new DataEvent(NADC());
	for(char i=0; (i<n)&&(i<NADC()); i++){
		res->Set_ADC(i,x[int(i)]);
	}
	delete[] x;
	return res;
}

