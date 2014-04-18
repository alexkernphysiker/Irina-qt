#include "outputroot.h"
#include <TGraphErrors.h>
#include <TGraph2D.h>
#include <TQtWidget.h>

#define _about_msg_ "Root data output/0.3"

RootOutput::RootOutput()
{
	mode=0;
}
RootOutput::~RootOutput(){

}

QString RootOutput::About(){return _about_msg_;}

void RootOutput::sl_out_1d(QString msg){
	switch(mode){
	case 0:
		data.clear();
		title=msg.replace("_"," ");
		mode=1;
		break;
	default:
		error("RootDataOut: data transfer error");
	}
}

void RootOutput::sl_out_2d(QString msg){
	switch(mode){
	case 0:
		data.clear();
		title=msg.replace("_"," ");
		mode=5;
		break;
	default:
		error("RootDataOut: data transfer error");
	}
}

void RootOutput::sl_out_dataitem(DataEvent *item){
	switch(mode){
	case 0:
		error("RootDataOut: data transfer error");
		break;
	case 1:
		data.append(item);
		break;
	case 2:
		mode=3;
	case 3:
		data.append(item);
		break;
	case 4:
		Y=item;
		mode=3;
		break;
	case 5:
		X=item;
		mode=4;
		break;
	default:
		error("RootDataOut: data transfer error");
	}
}

void RootOutput::sl_out_end(){
	switch(mode){
	case 1:{
			uint N=data.count();
			double *x = new double [N];
			double *dx = new double [N];
			double *y = new double [N];
			double *dy= new double [N];
			for(uint i=0; i<N; i++){
				DataEvent * item=data.at(i);
				x[i]=item->ADC(0);
				y[i]=item->ADC(1);
				dx[i]=item->ADC(2);
				dy[i]=item->ADC(3);
				delete item;
			}
			data.clear();
			// Create any other Qt-widget here
			TQtWidget *MyWidget= new TQtWidget(0,_about_msg_);
			MyWidget->resize(800,600);
			MyWidget->show();
			MyWidget->GetCanvas()->cd();
			TGraphErrors *mygraph = new TGraphErrors(N,x,y,dx,dy);
			delete[] x;
			delete[] dx;
			delete[] y;
			delete[] dy;
			mygraph->SetName(title.toStdString().c_str());
			mygraph->SetTitle(title.toStdString().c_str());
			mygraph->SetMarkerStyle(20);
			mygraph->Draw("AP");
			MyWidget->GetCanvas()->Update();
		}
		mode=0;
		break;
	case 2:{
		double X_init=X->ADC(0);
		double X_step=X->ADC(1);
		double Y_init=Y->ADC(0);
		double Y_step=Y->ADC(1);
		uint nx=uint((X->ADC(2)-X->ADC(0))/X->ADC(1));
		uint ny=uint((Y->ADC(2)-Y->ADC(0))/Y->ADC(1));
		uint n=nx*ny;
		double *xx=new double[n];
		double *yy=new double[n];
		double *zz=new double[n];
		delete X;
		delete Y;
		double x=X_init;
		double y=Y_init;
		uint N=data.count();
		uint nonzerocnt=0;
		{
			uint index=0;
			for(uint i=0; (i<N)&&(index<n); i++){
				DataEvent *item=data.at(i);
				if(item!=nullptr){
					xx[index]=x;yy[index]=y;
					zz[index]=item->ADC(0);
					if(zz[index]>0)nonzerocnt++;
					delete item;
					x+=X_step;
					index++;
				}else{
					x=X_init;
					y+=Y_step;
				}
			}
		}
		//ToDo: provide saving full sp2 data into file

		double *XX=new double[nonzerocnt];
		double *YY=new double[nonzerocnt];
		double *ZZ=new double[nonzerocnt];
		{
			uint index=0;
			for(uint i=0; (i<n)&&(index<nonzerocnt);i++){
				if(zz[i]>0){
					XX[index]=xx[i];
					YY[index]=yy[i];
					ZZ[index]=zz[i];
					index++;
				}
			}
		}
		delete[] xx;
		delete[] yy;
		delete[] zz;

		TQtWidget *MyWidget= new TQtWidget(0,_about_msg_);
		MyWidget->resize(800,600);
		MyWidget->show();
		MyWidget->GetCanvas()->cd();
		TGraph2D *mygraph = new TGraph2D(nonzerocnt, XX, YY, ZZ);
		delete[] XX;
		delete[] YY;
		delete[] ZZ;
		mygraph->SetName(title.toStdString().c_str());
		mygraph->SetTitle(title.toStdString().c_str());
		mygraph->Draw("pcol");
		MyWidget->GetCanvas()->Update();
		mode=0;
	}break;
	case 3:{
		data.append(nullptr);
		mode=2;
	}break;
	default:
		error("Plotter: data transfer error");
	}
}

void RootOutput::sl_out_config(){}

void RootOutput::sl_out_gui_close(){}
