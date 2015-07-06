// this file is distributed under
// GPL v 3.0 license
#include "ecalview.h"
#include "ui_ecalview.h"
#include "sp2open.h"
#include "additional.h"
#include <SortLibGui.h>

ECalView::ECalView(SoSPEnergy *spe, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::ECalView)
{
	ui->setupUi(this);
	m_spe=spe;
	SortProject *proj=m_spe->Owner();
	for(int i=0;i<proj->Number(m_spe);i++){
		SObject *so=proj->at(i);
		if(so->Is(SOT_SPAxis)){
			SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(so,NULL,false);
			ui->axtree->addTopLevelItem(item);
		}
	}
	refr();
}

ECalView::~ECalView()
{
	delete ui;
}

void ECalView::refr(){
	if(m_spe->XAxis()!=NULL)
		ui->xlabel->setText(m_spe->XAxis()->Name());
	else
		ui->xlabel->setText("not set");
	if(m_spe->YAxis()!=NULL)
		ui->ylabel->setText(m_spe->YAxis()->Name());
	else
		ui->ylabel->setText("not set");
	ui->treeWidget->clear();
	SortProject *proj=m_spe->Owner();
	for(int i=0; i<proj->Count();i++){
		SObject *so=proj->at(i);
		if(so->Is(SOT_ECSP2)){
			SoECSP2 *sp2=(SoECSP2*)so;
			if((sp2->XAxis()==m_spe->XAxis())&(sp2->YAxis()==m_spe->YAxis())){
				SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(sp2,NULL,false);
				ui->treeWidget->addTopLevelItem(item);
			}
		}
	}
}
SoSPAxis *ECalView::SelectedAxis(){
	if(ui->axtree->selectedItems().count()>0){
		SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)ui->axtree->selectedItems()[0];
		return (SoSPAxis*)item->myobj();
	}else return NULL;
}

void ECalView::on_xbut_clicked()
{
	SoSPAxis *ax=SelectedAxis();
	if(ax!=NULL)m_spe->SetXAxis(ax);
	refr();
}

void ECalView::on_ybut_clicked()
{
	SoSPAxis *ax=SelectedAxis();
	if(ax!=NULL)m_spe->SetYAxis(ax);
	refr();
}

void ECalView::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	SoECSP2 *sp2=(SoECSP2*)itm->myobj();
	SP2Open *form=new SP2Open(sp2,m_spe,this);
	form->show();
}
