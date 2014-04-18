#include <QPainter>
#include <QtGlobal>
#include <SortLibGui.h>
#include "sp1open.h"
#include "ui_sp1open.h"

SP1Open::SP1Open(SoECSP1 *sp1, QWidget *parent):
	QDialog(parent),
	ui(new Ui::SP1Open)
{
	ui->setupUi(this);
	m_sp1=sp1;
	m_offs=0;
	m_xscale=1;
	m_yscale_log=0;
	x_beg=ui->xsdec->x()+ui->xsdec->width()+5;
	m_markerpos=0;
	refr(NULL);
	busy(m_sp1->Owner()->IsBusy());
	setWindowTitle(m_sp1->DisplayName()+" ("+m_sp1->Source()->DisplayName()+")");
	connect(m_sp1->Owner(),SIGNAL(project_changed(SObject*)),this,SLOT(refr(SObject*)));
	connect(m_sp1->Owner(),SIGNAL(object_changed(SObject*)),this,SLOT(refr(SObject*)));
	connect(m_sp1,SIGNAL(changed(SObject*)),this,SLOT(refr(SObject*)));
	connect(m_sp1,SIGNAL(changed(SObject*)),this,SLOT(update()));
	connect(m_sp1,SIGNAL(destroyed()),this,SLOT(close()));
	connect(m_sp1->Owner(),SIGNAL(busy(bool)),this,SLOT(busy(bool)));
	connect(m_sp1->Owner(),SIGNAL(gui_close()),this,SLOT(close()));
}
void SP1Open::close(){
	m_sp1=NULL;
	QDialog::close();
}
SP1Open::~SP1Open()
{
	delete ui;
}

PeakTreeWidgetItem::PeakTreeWidgetItem(Peak *peak, QTreeWidget *w):QTreeWidgetItem(w){
	m_peak=peak;
	setText(0,m_peak->Name());
}
PeakTreeWidgetItem::~PeakTreeWidgetItem(){}
Peak *PeakTreeWidgetItem::myobj(){return m_peak;}

void SP1Open::refr(SObject* so){
	if(m_sp1==NULL)return;
	if(so!=NULL){
		if(!so->Is(SOT_EFWin)){
			if(so!=m_sp1)return;
		}
	}
	ui->wintree->clear();
	SortProject *proj=m_sp1->Owner();
	for(int i=0; i<proj->Count();i++){
		SObject *so=proj->at(i);
		if(so->Is(SOT_EFWin)){
			SoEFWin *win=(SoEFWin*)so;
			if(win->Axis()==m_sp1->Axis()){
				SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(win,NULL,false);
				ui->wintree->addTopLevelItem(item);
			}
		}
	}
	ui->peaktree->clear();
	for(int i=0; i<m_sp1->PeakCnt();i++){
		PeakTreeWidgetItem *item=new PeakTreeWidgetItem(m_sp1->getPeak(i));
		ui->peaktree->addTopLevelItem(item);
	}
}
SoEFWin *SP1Open::SelectedWindow(){
	if(ui->wintree->selectedItems().count()>0){
		SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)ui->wintree->selectedItems()[0];
		return (SoEFWin*)item->myobj();
	}else return NULL;
}
Peak *SP1Open::SelectedPeak(){
	if(ui->peaktree->selectedItems().count()>0){
		PeakTreeWidgetItem *item=(PeakTreeWidgetItem*)ui->peaktree->selectedItems()[0];
		return item->myobj();
	}else return NULL;
}

void SP1Open::on_mover_clicked()
{
	m_offs+=20;
	update();
}

void SP1Open::on_movel_clicked()
{
	if(m_offs>20)m_offs-=20;
	update();
}

void SP1Open::on_xsinc_clicked()
{
	if(m_xscale<32)m_xscale*=2;
	update();
}

void SP1Open::on_xsdec_clicked()
{
	if(m_xscale>1)m_xscale/=2;
	update();
}

void SP1Open::on_ysdec_clicked()
{
	if(m_yscale_log>-8)m_yscale_log--;
	update();
}

void SP1Open::on_ysinc_clicked()
{
	if(m_yscale_log<256)m_yscale_log++;
	update();
}

void SP1Open::on_wintree_itemSelectionChanged(){
	if(ui->wintree->selectedItems().count()>0)
		ui->peaktree->clearSelection();
	update();
}

int SP1Open::channelsperscreen(){
	return (width()-x_beg)/m_xscale;
}
int SP1Open::cntperscreen(){
	int res=height();
	if(m_yscale_log<0)
		for(int i=0; i>m_yscale_log; i--)
			res/=2;
	else
		for(int i=0; i<m_yscale_log; i++)
			res*=2;
	return res;
}
int SP1Open::y2cnt(int y){
	int res=height()-y;
	if(m_yscale_log<0)
		for(int i=0; i>m_yscale_log; i--)
			res/=2;
	else
		for(int i=0; i<m_yscale_log; i++)
			res*=2;
	return res;
}
int SP1Open::x2ch(int x){
	return (x-x_beg)/m_xscale;
}

void SP1Open::drawpnt(int ch, int cnt, QPainter &painter, QColor &color, QColor &color2){
	if(ch<0)return;
	if(ch>channelsperscreen())return;
	if(cnt>cntperscreen())cnt=cntperscreen();
	if(m_yscale_log<0)
		for(int i=0; i>m_yscale_log; i--)
			cnt*=2;
	else
		for(int i=0; i<m_yscale_log; i++)
			cnt/=2;

	painter.fillRect(x_beg+ch*m_xscale,height()-cnt,m_xscale,cnt,color);
	painter.fillRect(x_beg+ch*m_xscale,0,m_xscale,height()- cnt,color2);
}

void SP1Open::paintEvent(QPaintEvent *){
	QPainter painter(this);
	QColor colorsp;
	colorsp.setRgba(qRgba(0,0,255,255));
	QColor colorw;
	colorw.setRgba(qRgba(0,128,128,255));
	QColor colormarker;
	colormarker.setRgba(qRgba(0,0,0,255));
	QColor colormarker2;
	colormarker2.setRgba(qRgba(0,255,0,255));
	QColor colortr;
	colortr.setRgba(qRgba(0,0,0,0));
	SoEFWin *win=SelectedWindow();
	for(int ch=0; ch<channelsperscreen();ch++){
		int cnt=m_sp1->ChCnt(ch+m_offs);
		if((m_markerpos-(int)m_offs)==ch){
			drawpnt(ch,cnt,painter,colormarker,colormarker);
		}
		if(win!=NULL){
			if(win->In(ch+m_offs))
				drawpnt(ch,cnt,painter,colorw,colortr);
			else
				drawpnt(ch,cnt,painter,colorsp,colortr);
		}else{
			if(SelectedPeak()!=NULL){
				if(((ch+(int)m_offs)==SelectedPeak()->M1())|((ch+(int)m_offs)==SelectedPeak()->M2()))
					drawpnt(ch,cnt,painter,colormarker2,colormarker2);
				else
					drawpnt(ch,cnt,painter,colorsp,colortr);
			}else
				drawpnt(ch,cnt,painter,colorsp,colortr);
		}
	}
}

void SP1Open::updmarkermsg(){
	ui->leglabel->setText("Ch="+QString::number(m_markerpos)+
						  "(Value="+QString::number(m_sp1->Axis()->GetMagnitude(m_markerpos))+
													")\nCount = "+QString::number(m_sp1->ChCnt(m_markerpos)));
}
void SP1Open::on_llmb_clicked()
{
	if(m_markerpos>=20)m_markerpos-=20;
	if(!ui->radioButton_3->isChecked()){
		if(SelectedWindow()!=NULL){
		}
	}
	update();updmarkermsg();
}

void SP1Open::on_lmb_clicked()
{
	if(m_markerpos>0)m_markerpos--;
	if(!ui->radioButton_3->isChecked()){
		if(SelectedWindow()!=NULL){
			if(ui->rrb->isChecked())
				SelectedWindow()->SetIn(m_markerpos,false);
			if(ui->lrb->isChecked())
				SelectedWindow()->SetIn(m_markerpos,true);
		}
	}
	update();updmarkermsg();
}

void SP1Open::on_rmb_clicked()
{
	if(m_markerpos<65535)m_markerpos++;
	if(!ui->radioButton_3->isChecked()){
		if(SelectedWindow()!=NULL){
			if(ui->rrb->isChecked())
				SelectedWindow()->SetIn(m_markerpos,false);
			if(ui->lrb->isChecked())
				SelectedWindow()->SetIn(m_markerpos,true);
		}
	}
	update();updmarkermsg();
}

void SP1Open::on_rrmb_clicked()
{
	if(m_markerpos<=65515)m_markerpos+=20;
	update();updmarkermsg();

}
void SP1Open::keyPressEvent(QKeyEvent *ke){
	if(ke->key()==Qt::Key_Left){
		if(ke->modifiers()==Qt::NoButton)
			on_lmb_clicked();
		else
			on_llmb_clicked();
	}else if(ke->key()==Qt::Key_Right){
		if(ke->modifiers()==Qt::NoButton)
			on_rmb_clicked();
		else
			on_rrmb_clicked();
	}else if(ke->key()==Qt::Key_Insert){
		ui->lrb->setChecked(true);
	}else if(ke->key()==Qt::Key_Delete){
		ui->rrb->setChecked(true);
	}else if(ke->key()==Qt::Key_Home){
		ui->radioButton_3->setChecked(true);
	}else if(ke->key()==Qt::Key_PageUp){
		if(SelectedPeak()!=NULL)SelectedPeak()->SetM1(m_markerpos);
		update();
	}else if(ke->key()==Qt::Key_PageDown){
		if(SelectedPeak()!=NULL)SelectedPeak()->SetM2(m_markerpos);
		update();
	}
	else{
		QWidget::keyPressEvent(ke);
	}
}

void SP1Open::on_toolButton_clicked()
{//Add peak
	if(ui->peakname->text()!="")
		m_sp1->AddPeak(ui->peakname->text());
	//refr();
}

void SP1Open::on_toolButton_2_clicked()
{//delete peak
	if(SelectedPeak()!=NULL)
		m_sp1->RemPeak(SelectedPeak());
	//refr();
}

void SP1Open::on_peaktree_itemSelectionChanged()
{
	if(SelectedPeak()!=NULL)
		ui->wintree->clearSelection();
	update();
}

void SP1Open::on_toolButton_4_clicked()
{
	m_sp1->Owner()->Add(new SoEFWin(m_sp1->Axis()));
}

void SP1Open::on_toolButton_3_clicked()
{
	if(SelectedWindow()!=NULL){
		SortProject *proj=m_sp1->Owner();
		proj->Remove(proj->Number(SelectedWindow()));
	}
}

void SP1Open::on_pushButton_clicked()
{
	m_sp1->Source()->Read();
}

void SP1Open::busy(bool val){
	ui->pushButton->setEnabled(!val);
	ui->toolButton_3->setEnabled(!val);
	ui->toolButton_4->setEnabled(!val);
}
