// this file is distributed under
// GPL v 3.0 license
#include "gnuplotter.h"
#include <QString>
#include <QFileDialog>
#include <QApplication>
#include <QFile>


#include "plotterview.h"
#include "settingswindow.h"

QString getConfigPath(){
	QDir appDir = QDir(qApp->applicationDirPath());
	return appDir.absoluteFilePath("gnuplot.config");
}

QString Caption_plot(){
	return "Gnuplot output/2.4";
}

PlotParameters::PlotParameters(){
	xlabel="";
	ylabel="";
	logx=false;
	logy=false;
	xrange="";
	yrange="";
	xtics="";
	ytics="";
	setkey="";
}

Gnuplotter::Gnuplotter(){
	gnuplotpath="gnuplot";
	{
		QString p=getConfigPath();
		QFile file(p);
		file.open(QFile::ReadOnly);
		if(file.isOpen()){
			QTextStream str(&file);
			gnuplotpath="";
			while(!str.atEnd()){
				QString s="";
				str>>s;
				if(gnuplotpath!="")
					gnuplotpath=gnuplotpath+" "+s;
				else
					gnuplotpath=s;
			}
			file.close();
		}
	}
	create_png=false;
	create_ps=false;
	pngsize="640,480";
	font="Arial, 14";
	mode=0;
	title="";
	ax1=NULL;
	ax2=NULL;
}
Gnuplotter::~Gnuplotter(){
	{
		QFile file(getConfigPath());
		file.open(QFile::WriteOnly);
		if(file.isOpen()){
			QTextStream str(&file);
			str<<gnuplotpath;
			file.close();
		}
	}
	if(ax1!=NULL)
		delete ax1;
	if(ax2!=NULL)
		delete ax2;
}
void Gnuplotter::processerror(QProcess::ProcessError /*err*/){
   QString msg="\nDATA_OUTPUT: error occured while starting gnuplot";
   error(msg);
}

//Opens Gnuplotter for drawing 1-d plot
void Gnuplotter::sl_out_1d(QString msg){
	if(mode!=0)
		error("DATA_OUTPUT: data transfer error");
	mode=1;
	title=msg.replace("_"," ");
	txtfile=new QFile(QString(msg+".txt"));
	txtfile->open(QFile::WriteOnly);
	if(!txtfile->isOpen()){
		error("DATA_OUTPUT: cannot create file "+msg+".txt");
	}else{
		txtfilecreator=new QTextStream(txtfile);
	}
}
//Opens Gnuplotter for drawing 2-d plot
void Gnuplotter::sl_out_2d(QString msg){
	if(mode!=0)
		error("DATA_OUTPUT: data transfer error");
	mode=5;
	title=msg.replace("_"," ");
	txtfile=new QFile(QString(msg+".txt"));
	txtfile->open(QFile::WriteOnly);
	if(!txtfile->isOpen()){
		error("DATA_OUTPUT: cannot create file "+msg+".txt");
	}else{
		txtfilecreator=new QTextStream(txtfile);
	}
}
//Transfer data from irina-qt
void Gnuplotter::sl_out_dataitem(DataEvent* item){
	switch(mode){
	case 1:{
		if(txtfile->isOpen()){
			*txtfilecreator << (item->ADC(0)) <<"\t"<< (item->ADC(1)) <<"\t"<< (item->ADC(2)) <<"\t"<< (item->ADC(3)) <<"\n";
		}
		delete item;
	}	break;
	case 2:
		mode=3;
	case 3:{
		if(txtfile->isOpen()){
			*txtfilecreator << (item->ADC(0)) <<"\t";
		}
	}	break;
	case 4:{
		if(ax2!=NULL)delete ax2;
		ax2=item;
		mode=3;
	}	break;
	case 5:{
		if(ax1!=NULL)delete ax1;
		ax1=item;
		mode=4;
	}	break;
	default:
		error("DATA_OUTPUT: data transfer error");
		break;
	}
}
//data-end signal
void Gnuplotter::sl_out_end(){
	switch(mode){
	case 1:{
		if(txtfile->isOpen()){
			txtfile->close();
			delete txtfilecreator;
			delete txtfile;
			out_1d();
		}
		mode=0;
	}	break;
	case 2:{
		if(txtfile->isOpen()){
			txtfile->close();
			delete txtfilecreator;
			delete txtfile;
			out_2d();
		}
		mode=0;
	}break;
	case 3:{
		if(txtfile->isOpen()){
			*txtfilecreator << "\n";
		}
		mode=2;
	}break;
	default:
		error("DATA_OUTPUT: data transfer error");
		break;
	}
}


void Gnuplotter::out_1d(){
	QString path = title+".txt";
	if(m_data.count()==0){
		PlotterView *form=new PlotterView();
		form->setWindowTitle(Caption_plot());
		connect(this, SIGNAL(changed(QStringList&)),form,SLOT(TakeData(QStringList&)));
		connect(form,SIGNAL(info(bool,PlotParameters*)),this,SLOT(Closing(bool,PlotParameters*)));
		connect(form,SIGNAL(settings()),this,SLOT(GnuplotterSetup()));
		connect(this, SIGNAL(closeallrelated()),form,SLOT(close()));
		form->show();
	}
	m_data.append(path);
	m_data.append(title);
	changed(m_data);
}

void Gnuplotter::check_createfile(QTextStream &str){
	str << "set termoption font \""<<font<<"\"\n";
	str<<"replot\n";
	if(create_ps){
		QString filepath=QFileDialog::getSaveFileName(NULL,"Save plot to PS","*.ps","*.ps");
		str<<"set terminal postscript monochrome solid \n";
		str<<"set output '"+filepath+"'\n";
		str << "set termoption font \""<<font<<"\"\n";
		str<<"replot\n";
	}
	if(create_png){
		QString filepath=QFileDialog::getSaveFileName(NULL,"Save plot to PNG","*.png","*.png");
		str<<"set terminal png size "<<pngsize<<" enhanced font \""<<font<<"\"\n";
		str<<"set output '"+filepath+"'\n";
		str << "set termoption font \""<<font<<"\"\n";
		str<<"replot\n";
	}
}

void Gnuplotter::run(QString script){
	QProcess *gnuplot=new QProcess();
	QObject::connect(gnuplot,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processerror(QProcess::ProcessError)));
	QObject::connect(gnuplot,SIGNAL(finished(int)),gnuplot,SLOT(deleteLater()));
	gnuplot->start(gnuplotpath,QStringList()
			<<"-gray"<<"-persist"
			<<script);
}

void Gnuplotter::out_2d(){
	QString path = title+".txt";
	QString parameters= "u (($1*"+
			QString::number(ax1->ADC(1))+")+"+
			 QString::number(ax1->ADC(0))+
			 "):(($2*"+
			 QString::number(ax2->ADC(1))+")+"+
			 QString::number(ax2->ADC(0))+
			  "):"+
			 "3 matrix w image title '"+title+"'";
	QString preset="set xrange [ "+
			QString::number(ax1->ADC(0))+" : "+QString::number(ax1->ADC(2))+
			"]\n set yrange [ "+
			QString::number(ax2->ADC(0))+" : "+QString::number(ax2->ADC(2))+
			"]";
	QString script=".plotscript.gp";
	QFile file(script);
	file.open(QFile::WriteOnly);
	if(file.isOpen()){
		QTextStream str(&file);
		int k=96;
		str<<"set palette maxcolors "<<k<<"\n";
		str<<"set palette defined (" ;
		str<<"0 '#ffffff'";
		for(int i=1;i<k;i++)
			str<<", "<<i<<" '#"
			  << 60-i/2<<60-i/2<<60-i/2<<"'";
		str<<")\n";
		str<<preset<<"\n";
		str << "plot " <<"'"<<path+"' "<<parameters <<"\n";
		check_createfile(str);
		str<<"\npause 60000\n";
		file.close();
	}
	run(script);
}

void Gnuplotter::Closing(bool toplot, PlotParameters* params){
	if(m_data.count()==0)return;
	if(m_data.count()<2){
		m_data.clear();
		return;
	}
	if(toplot){
		QString script=".plotscript.gp";
		QFile file(script);
		file.open(QFile::WriteOnly);
		if(file.isOpen()){
			QTextStream str(&file);
			str << "set xlabel '"<<params->xlabel<<"'\n";
			str << "set ylabel '"<<params->ylabel<<"'\n";
			if(params->logx)
				str<<"set logscale x\n";
			if(params->logy)
				str<<"set logscale y\n";
			if(params->xrange!="")
				str<<"set xrange ["<<params->xrange<<"]\n";
			if(params->yrange!="")
				str<<"set yrange ["<<params->yrange<<"]\n";
			if(params->xtics!="")
				str<<"set xtics "<<params->xtics<<"\n";
			if(params->ytics!="")
				str<<"set ytics "<<params->ytics<<"\n";
			if(params->setkey!="")
				str<<"set key "<<params->setkey<<"\n";
			str << "set style line 1 lc rgb \"#000000\" lt 1 lw 1\n";
			str << "plot ";
			int linetype=0;
			int pointtype=4;
			for(int i=1; i<m_data.count();i+=2){
				QString title=m_data.at(i);
				str <<"\""+m_data.at(i-1)+"\"";
				int indexof=title.indexOf("#");
				if(indexof>=0){
					title=title.remove(indexof,title.length()-indexof);
				}
				title=title.remove("{yerrorbars}");
				indexof=title.indexOf("{line}");
				if(indexof>=0){
					title=title.remove("{line}");
					str <<" w l "<<"linetype "<<linetype;
					linetype++;
				}else{
					indexof=title.indexOf("{points}");
					if(indexof>=0){
						title=title.remove("{points}");
						str <<" pt "<<pointtype<<" ";
					}else{
						str <<" w xyerrorbars ls 1 pt "<<pointtype<<" ";
					}
					pointtype++;
				}
				if(title!=""){
					str<<"title \""+title+"\"";
				}else{
					str<<"notitle";
				}
				if(i<m_data.count()-2)
					str <<", \\";
				str <<"\n";
			}
			check_createfile(str);
			str<<"\npause 60000\n";
			file.close();
			m_data.clear();
		}
		run(script);
	}
	m_data.clear();
}

void Gnuplotter::sl_out_gui_close(){
	closeallrelated();
}

void Gnuplotter::sl_out_config(){
	SettingsWindow *form=new SettingsWindow(this);
	QObject::connect(this, SIGNAL(closeallrelated()),form,SLOT(close()));
	form->show();
}

QString Gnuplotter::About(){return Caption_plot();}
