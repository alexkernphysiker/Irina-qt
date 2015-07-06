// this file is distributed under
// GPL v 3.0 license
#include <additional.h>
#include "ecounterview.h"
#include "ui_ecounterview.h"
#include <sp1view.h>
#include <sp2view.h>
#include <SortLibGui.h>

ECounterView::ECounterView(SoECounter *cnt, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::ECounterView)
{
	ui->setupUi(this);
	m_counter=cnt;
	ui->nameedit->setText(m_counter->Name());
	refrAv();
	refrUsed();
	if(m_counter->Is(SOT_ECSP1)){
		QWidget *view=new SP1View((SoECSP1*)m_counter,this);
		view->move(0,ui->AvTree->y()+ui->AvTree->height());
		view->setVisible(true);
	}
	if(m_counter->Is(SOT_ECSP2)){
		QWidget *view=new SP2View((SoECSP2*)m_counter,this);
		view->move(0,ui->AvTree->y()+ui->AvTree->height());
		view->setVisible(true);
	}
}

ECounterView::~ECounterView()
{
	delete ui;
}
void ECounterView::on_nameedit_textChanged(const QString &arg1)
{
	m_counter->Set_Name(arg1);
}

void ECounterView::refrAv(){
	ui->AvTree->clear();
	for(int i=0; i<m_counter->Owner()->Number(m_counter);i++){
		SObject *so=m_counter->Owner()->at(i);
		SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(so,NULL,false);
		if(so->Is(SOT_EFilter))ui->AvTree->addTopLevelItem(item);
	}
}
void ECounterView::refrUsed(){
	ui->UsedTree->clear();
	for(int i=0; i<m_counter->FilterCnt();i++)
		ui->UsedTree->addTopLevelItem(new SObjectTreeWidgetItem(m_counter->Filter(i),NULL,false));
}

void ECounterView::on_AvTree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	m_counter->AddFilter((SoEFilter*)itm->myobj());
	refrUsed();
}

void ECounterView::on_UsedTree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	m_counter->DelFilterAt(m_counter->FilterInd((SoEFilter*)itm->myobj()));
	refrUsed();
}
