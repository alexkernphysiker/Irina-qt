// this file is distributed under
// GPL v 3.0 license
#include "additional.h"
#include <QPainter>
#include "sp2open.h"
#include "ui_sp2open.h"
#include <SortLibGui.h>

SP2Open::SP2Open(SoECSP2 *sp2, SoSPEnergy *cal, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SP2Open)
{
	ui->setupUi(this);
	m_sp2=sp2;
	origx=ui->left->x();
	origy=ui->left->y();
	offsx=offsy=markerx=markery=0;
	setWindowTitle(m_sp2->DisplayName()+" ("+m_sp2->Source()->DisplayName()+")");
	refrlocs(NULL);
	busy(m_sp2->Owner()->IsBusy());
	connect(m_sp2->Owner(),SIGNAL(busy(bool)),this, SLOT(busy(bool)));
	connect(m_sp2->Owner(),SIGNAL(project_changed(SObject*)),this,SLOT(refrlocs(SObject*)));
	connect(m_sp2->Owner(),SIGNAL(object_changed(SObject*)),this,SLOT(refrlocs(SObject*)));
	connect(m_sp2,SIGNAL(destroyed()),this,SLOT(close()));
	connect(m_sp2,SIGNAL(changed(SObject*)),this,SLOT(refrlocs(SObject*)));
	connect(m_sp2,SIGNAL(changed(SObject*)),this,SLOT(update()));
	connect(m_sp2->Owner(),SIGNAL(gui_close()),this,SLOT(close()));
	points=QList<QPoint>();
	m_cal=cal;
	ui->calbox->setVisible(cal!=NULL);
	if(cal!=NULL){
		ui->e1->setValue(cal->Magn(0));
		ui->e2->setValue(cal->Magn(1));
		ui->e3->setValue(cal->Magn(2));
	}
}
void SP2Open::close(){
	m_sp2=NULL;
	QDialog::close();
}
void SP2Open::refrlocs(SObject *so){
	if(m_sp2==NULL)return;
	if(so!=NULL){
		if(!so->Is(SOT_EFLoc)){
			if(so!=m_sp2)return;
		}
	}
	ui->loctree->clear();
	for(int i=0; i<m_sp2->Owner()->Count();i++){
		SObject *so=m_sp2->Owner()->at(i);
		if(so->Is(SOT_EFLoc)){
			SoEFLoc *loc=(SoEFLoc*)so;
			if((loc->XAxis()==m_sp2->XAxis())&(loc->YAxis()==m_sp2->YAxis())){
				SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(loc,NULL,false);
				ui->loctree->addTopLevelItem(item);
			}
		}
	}
}
SP2Open::~SP2Open()
{
	delete ui;
}
SoEFLoc *SP2Open::SelectedLocus(){
	SoEFLoc *res=NULL;
	if(ui->loctree->selectedItems().count()>0){
		SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)ui->loctree->selectedItems()[0];
		res=(SoEFLoc*)item->myobj();
	}
	return res;
}

void SP2Open::paintEvent(QPaintEvent *ev){
		QPainter painter(this);
		SoEFLoc *loc=SelectedLocus();
		int sc=ui->scale->value();
		int szx=(width()-origx)/sc;
		int szy=origy/sc;
		for(int y=0; y<szy;y++)
			for(int x=0; x<szx; x++){
				int cnt=m_sp2->Cnt(x+offsx,y+offsy);
				QRect *rect=new QRect(origx+x*sc,origy-(y*sc)-sc,sc,sc);
				if(rect->intersects(ev->rect())){
					if(cnt>ui->threshold->value()){
						QColor color;
						int b=64+cnt*ui->brightness->value();
						if(b>255)b=255;
						int cv=ui->colorslider->value();
						color.setRgba(qRgba(cv,cv,cv,b));
						painter.fillRect(*rect, color);
					}
					if(loc!=NULL)
						if(loc->In(x+offsx,y+offsy)){
							QColor color;
							color.setRgba(qRgba(0,255-ui->LocCol->value(),ui->LocCol->value(),96));
							painter.fillRect(*rect, color);
						}
				}
				delete rect;
			}
		for(int i=1; i<points.count();i++){
			int x1=points[i-1].x()-offsx;
			int y1=points[i-1].y()-offsy;
			int x2=points[i].x()-offsx;
			int y2=points[i].y()-offsy;
			x1=origx+x1*sc+sc/2;
			x2=origx+x2*sc+sc/2;
			y1=origy-y1*sc-sc/2;
			y2=origy-y2*sc-sc/2;
			//QColor color;
			//color.setRgba(qRgba(0,0,0,255));
			painter.drawLine(x1,y1,x2,y2);
		}
		for(int i=1; i<points.count();i++){
			int x=points[i].x()-offsx;
			int y=points[i].y()-offsy;
			QRect *rect=new QRect(origx+x*sc,origy-(y*sc)-sc,sc,sc);
			QColor color;
			color.setRgba(qRgba(0,0,0,255));
			painter.fillRect(*rect, color);
			delete rect;
		}
		{
			int x=markerx-offsx;
			int y=markery-offsy;
			int msz=ui->markersize->value();
			QRect *rect=new QRect(origx+x*sc,origy-(y*sc)-sc,sc,sc);
			QColor color;
			color.setRgba(qRgba(255-ui->MarkerCol->value(),ui->MarkerCol->value(),0,255));
			painter.fillRect(*rect, color);
			delete rect;
			rect=new QRect(origx+x*sc-sc*msz,origy-(y*sc)-sc-sc*msz,sc*(1+2*msz),sc*(1+2*msz));
			color.setRgba(qRgba(255-ui->MarkerCol->value(),ui->MarkerCol->value(),0,128));
			painter.fillRect(*rect, color);
			delete rect;
		}
}
void SP2Open::checkpeaks(){
	if(ui->peak1->isChecked()){
		QPoint p=QPoint(markerx,markery);
		m_cal->SetMarker(0,p);
	}
	if(ui->peak2->isChecked()){
		QPoint p=QPoint(markerx,markery);
		m_cal->SetMarker(1,p);
	}
	if(ui->peak3->isChecked()){
		QPoint p=QPoint(markerx,markery);
		m_cal->SetMarker(2,p);
	}
}
void SP2Open::keyPressEvent(QKeyEvent *ke){
	SoEFLoc *loc=SelectedLocus();
	bool val=ui->locadd->isChecked();
	int x=markerx-offsx;
	int y=markery-offsy;
	int msz=ui->markersize->value();
	int sc=ui->scale->value();
	QRect *oldrect=new QRect(origx+x*sc-sc*msz,origy-(y*sc)-sc-sc*msz,sc*(1+2*msz),sc*(1+2*msz));
	if((ke->key()==Qt::Key_A)|(ke->key()==Qt::Key_Q)|(ke->key()==Qt::Key_Z)){
		if(ke->modifiers()==Qt::NoButton){
			if(markerx>0)markerx--;
		}else{
			if(markerx>15)markerx-=16;
		}
	}
	if((ke->key()==Qt::Key_D)|(ke->key()==Qt::Key_E)|(ke->key()==Qt::Key_C)){
		if(ke->modifiers()==Qt::NoButton){
			if(markerx<65536)markerx++;
		}else{
			if(markerx<65520)markerx+=16;
		}
	}
	if((ke->key()==Qt::Key_S)|(ke->key()==Qt::Key_Z)|(ke->key()==Qt::Key_C)){
		if(ke->modifiers()==Qt::NoButton){
			if(markery>0)markery--;
		}else{
			if(markery>15)markery-=16;
		}
	}
	if((ke->key()==Qt::Key_W)|(ke->key()==Qt::Key_Q)|(ke->key()==Qt::Key_E)){
		if(ke->modifiers()==Qt::NoButton){
			if(markery<65536)markery++;
		}else{
			if(markery<65520)markery+=16;
		}
	}
	if(ke->key()==Qt::Key_K)ui->locadd->setChecked(true);
	if(ke->key()==Qt::Key_L)ui->locdel->setChecked(true);
	if((ke->key()==Qt::Key_X)&(loc!=NULL)){
		points.append(QPoint(markerx,markery));
		update();
	}
	if((ke->key()==Qt::Key_Escape)){
		points.clear();
		update();
	}
	if((ke->key()==Qt::Key_V)&(loc!=NULL)){
		points.append(QPoint(markerx,markery));
		points.append(points[0]);
		int xmin=65535;
		int xmax=0;
		for(int i=0; i<points.count();i++){
			if(xmin>points[i].x())xmin=points[i].x();
			if(xmax<points[i].x())xmax=points[i].x();
		}
		if(xmax>xmin){
			int ymin[xmax-xmin+1];
			int ymax[xmax-xmin+1];
			for(int i=0;i<=(xmax-xmin);i++){
				ymin[i]=65535;
				ymax[i]=0;
			}
			for(int i=1;i<points.count();i++){
				int x1=points[i-1].x();
				int y1=points[i-1].y();
				int x2=points[i].x();
				int y2=points[i].y();
				if(x1<x2){
					for(int x=x1; x<=x2;x++){
						double dxf=(x2-x1);
						double dxp=(x-x1);
						double dyf=(y2-y1);
						int y=y1+int(dxp*dyf/dxf);
						if(ymin[x-xmin]>y)ymin[x-xmin]=y;
						if(ymax[x-xmin]<y)ymax[x-xmin]=y;
					}
				}else if(x1>x2){
					for(int x=x1; x>=x2;x--){
						double dxf=(x2-x1);
						double dxp=(x-x1);
						double dyf=(y2-y1);
						int y=y1+int(dxp*dyf/dxf);
						if(ymin[x-xmin]>y)ymin[x-xmin]=y;
						if(ymax[x-xmin]<y)ymax[x-xmin]=y;
					}
				}else{
					if(ymin[x1-xmin]>y1)ymin[x1-xmin]=y1;
					if(ymax[x1-xmin]<y1)ymax[x1-xmin]=y1;
					if(ymin[x1-xmin]>y2)ymin[x1-xmin]=y2;
					if(ymax[x1-xmin]<y2)ymax[x1-xmin]=y2;
				}
			}
			for(int x=xmin; x<=xmax;x++){
				int y1=ymin[x-xmin];
				int y2=ymax[x-xmin];
				for(int y=y1; y<=y2;y++)
					loc->SetIn(x,y,val);
			}
			points.clear();
		}
		update();
	}

	x=markerx-offsx;
	y=markery-offsy;
	QRect *rect=new QRect(origx+x*sc-sc*msz,origy-(y*sc)-sc-sc*msz,sc*(1+2*msz),sc*(1+2*msz));
	update(*oldrect);
	update(*rect);
	delete rect;
	delete oldrect;
	ui->splabel->setText("X="+QString::number(markerx)+
						 " Y="+QString::number(markery)+" Count="+QString::number(m_sp2->Cnt(markerx,markery)));
	checkpeaks();
}

void SP2Open::on_up_clicked()
{
	if(offsy<65536)offsy+=32;
	update();
}

void SP2Open::on_down_clicked()
{
	if(offsy>0)offsy-=32;
	update();
}

void SP2Open::on_left_clicked()
{
	if(offsx>0)offsx-=32;
	update();
}

void SP2Open::on_right_clicked()
{
	if(offsx<65536)offsx+=32;
	update();
}

void SP2Open::on_scale_valueChanged(int /*value*/){update();}

void SP2Open::on_brightness_valueChanged(int /*value*/){update();}

void SP2Open::on_threshold_valueChanged(int /*value*/){update();}

void SP2Open::on_markersize_valueChanged(int /*value*/){update();}



void SP2Open::on_loctree_itemSelectionChanged()
{
	update();
}

void SP2Open::on_e1_valueChanged(double arg1)
{
	m_cal->SetMagn(0,arg1);
}

void SP2Open::on_e2_valueChanged(double arg1)
{
	m_cal->SetMagn(1,arg1);
}

void SP2Open::on_e3_valueChanged(double arg1)
{
	m_cal->SetMagn(2,arg1);
}

void SP2Open::on_peak3_toggled(bool checked)
{
	if(checked){
		QPoint p=m_cal->Marker(2);
		markerx=p.x();
		markery=p.y();
		update();
	}
}

void SP2Open::on_peak2_toggled(bool checked)
{
	if(checked){
		QPoint p=m_cal->Marker(1);
		markerx=p.x();
		markery=p.y();
		update();
	}
}

void SP2Open::on_peak1_toggled(bool checked)
{
	if(checked){
		QPoint p=m_cal->Marker(0);
		markerx=p.x();
		markery=p.y();
		update();
	}
}

void SP2Open::on_toolButton_clicked()
{
	SoEFLoc *loc=new SoEFLoc(m_sp2->Owner());
	loc->SetXAxis(m_sp2->XAxis());
	loc->SetYAxis(m_sp2->YAxis());
	m_sp2->Owner()->Add(loc);
}

void SP2Open::on_toolButton_2_clicked()
{
	SoEFLoc *loc=SelectedLocus();
	if(loc!=NULL)m_sp2->Owner()->Remove(m_sp2->Owner()->Number(loc));
}

void SP2Open::on_pushButton_clicked()
{
	m_sp2->Source()->Read();
}

void SP2Open::on_colorslider_valueChanged(int /*value*/){	update();}

void SP2Open::on_LocCol_valueChanged(int /*value*/){update();}

void SP2Open::on_MarkerCol_valueChanged(int /*value*/){update();}

void SP2Open::busy(bool val){
	ui->pushButton->setEnabled(!val);
	ui->toolButton->setEnabled(!val);
	ui->toolButton_2->setEnabled(!val);
}
