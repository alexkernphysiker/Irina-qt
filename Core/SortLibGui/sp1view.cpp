// this file is distributed under
// GPL v 3.0 license
#include "sp1view.h"
#include "ui_sp1view.h"
#include <sp1open.h>
#include <additional.h>
#include <SortLibGui.h>


SP1View::SP1View(SoECSP1 *sp1, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::SP1View)
{
	ui->setupUi(this);
	m_sp1=sp1;
	busy(m_sp1->Owner()->IsBusy());
	connect(m_sp1->Owner(),SIGNAL(busy(bool)),this,SLOT(busy(bool)));
	for(int i=0; i<sp1->Owner()->Number(sp1);i++){
		SObject *so=sp1->Owner()->at(i);
		if(so->Is(SOT_SPAxis))ui->AxisTree->addTopLevelItem(new SObjectTreeWidgetItem(so,NULL,false));
	}
	refr();
}

SP1View::~SP1View()
{
	delete ui;
}
void SP1View::refr(){
	QString msg="";
	if(m_sp1->Axis()!=NULL){
		msg="Axis: "+m_sp1->Axis()->Name()+
				"\nMinimum value = "+QString::number( m_sp1->Axis()->Min())+
				"\nMaximum value = "+QString::number( m_sp1->Axis()->Max())+
				"\nChannel width = "+QString::number(m_sp1->Axis()->ChannelW())+
				"\nChannel count = "+QString::number(m_sp1->Axis()->ChannelCount());
	}else{
		msg="This spectrum is not configured yet.\nPlease select axis configuration for it\ndouble clicking item at the list.";
	}
	ui->label->setText(msg);
}

void SP1View::on_AxisTree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	SoSPAxis *axis=(SoSPAxis*)itm->myobj();
	m_sp1->SetAxis(axis);
	refr();
}

void SP1View::on_refrbutt_clicked()
{
	m_sp1->Source()->Read();
}

void SP1View::busy(bool val){
	ui->refrbutt->setEnabled(!val);
	ui->axis_button->setEnabled(!val);
}

void SP1View::on_axis_button_clicked()
{
	SortProject *proj=m_sp1->Owner();
	proj->Insert(new SoSPAxis(proj),proj->Number(m_sp1));
}

void SP1View::on_open_but_clicked()
{
	if(m_sp1->Axis()==NULL)return;
	SP1Open *form=new SP1Open(m_sp1,NULL);
	form->show();
}

void SP1View::on_DataOutButton_clicked()
{
	m_sp1->Export();
}
