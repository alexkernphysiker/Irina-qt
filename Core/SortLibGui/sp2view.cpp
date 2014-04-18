#include "additional.h"
#include "sp2view.h"
#include "ui_sp2view.h"
#include "sp2open.h"
#include <SortLibGui.h>

SP2View::SP2View(SoECSP2 *sp2, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::SP2View)
{
	ui->setupUi(this);
	m_sp2=sp2;
	busy(m_sp2->Owner()->IsBusy());
	connect(m_sp2->Owner(),SIGNAL(busy(bool)),this, SLOT(busy(bool)));
	for(int i=0; i<sp2->Owner()->Number(sp2);i++){
		SObject *so=sp2->Owner()->at(i);
		if(so->Is(SOT_SPAxis))ui->AxTree->addTopLevelItem(new SObjectTreeWidgetItem(so,NULL,false));
	}
	refr();
}

SP2View::~SP2View()
{
	delete ui;
}

void SP2View::refr(){
	ui->xlabel->setText("not set");
	if(m_sp2->XAxis()!=NULL)ui->xlabel->setText(m_sp2->XAxis()->Name());
	ui->ylabel->setText("not set");
	if(m_sp2->YAxis()!=NULL)ui->ylabel->setText(m_sp2->YAxis()->Name());
	ui->splabel->setText("Please configure the SP2");
	if((m_sp2->XAxis()!=NULL)&(m_sp2->YAxis()!=NULL))
		ui->splabel->setText(QString::number(m_sp2->XAxis()->ChannelCount())+"*"
							 +QString::number(m_sp2->YAxis()->ChannelCount())+" channels");
}
SoSPAxis *SP2View::SelectedAxis(){
	if(ui->AxTree->selectedItems().count()){
		SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)ui->AxTree->selectedItems()[0];
		SoSPAxis *ax=(SoSPAxis*)item->myobj();
		return ax;
	}
	return NULL;
}
void SP2View::on_makeaxis_clicked()
{
	SortProject *proj=m_sp2->Owner();
	proj->Insert(new SoSPAxis(proj),proj->Number(m_sp2));
}

void SP2View::on_setx_clicked()
{
	SoSPAxis *ax=SelectedAxis();
	if(ax!=NULL)m_sp2->SetXAxis(ax);
	refr();
}

void SP2View::on_sety_clicked()
{
	SoSPAxis *ax=SelectedAxis();
	if(ax!=NULL)m_sp2->SetYAxis(ax);
	refr();
}

void SP2View::on_refrbut_clicked()
{
	m_sp2->Source()->Read();
}


void SP2View::on_openbut_clicked()
{
	if(m_sp2->XAxis()==NULL)return;
	if(m_sp2->YAxis()==NULL)return;
	SP2Open *form=new SP2Open(m_sp2,NULL,NULL);
	form->show();
}

void SP2View::on_exportbut_clicked()
{
	m_sp2->Export();
}


void SP2View::busy(bool val){
	ui->refrbut->setEnabled(!val);
	ui->makeaxis->setEnabled(!val);
}
