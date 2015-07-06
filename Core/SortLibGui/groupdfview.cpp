// this file is distributed under
// GPL v 3.0 license
#include "groupdfview.h"
#include "ui_groupdfview.h"
#include <additional.h>
#include <SortLibGui.h>

GroupDFView::GroupDFView(SoGroupDF *group, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::GroupDFView)
{
	ui->setupUi(this);
	m_group=group;
	ui->nameedit->setText(m_group->Name());
	ui->typelabel->setText(m_group->FormulaType());
	ui->expredit->setText(m_group->Expression());
	SortProject *proj=m_group->Owner();
	for(int i=0; i<proj->Number(m_group);i++){
		SObject *so=proj->at(i);
		if(so->Is(SOT_DFReader)){
			SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(so,NULL,false);
			ui->avtree->addTopLevelItem(item);
		}
	}
	refr();
}

GroupDFView::~GroupDFView()
{
	delete ui;
}
void GroupDFView::refr(){
	ui->usedtree->clear();
	for(int i=0; i<m_group->DFCnt();i++){
		SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(m_group->GetDF(i),NULL,false);
		ui->usedtree->addTopLevelItem(item);
	}
}
void GroupDFView::on_nameedit_textChanged(const QString &arg1)
{
	m_group->Set_Name(arg1);
}

void GroupDFView::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	if(item->text(0)=="SUM")m_group->SetSumm();
	if(item->text(0)=="SUM_OF_SQUARES")m_group->SetSummSq();
	if(item->text(0)=="AVERAGE")m_group->SetAverage();
	if(item->text(0)=="VARIANCE")m_group->SetDispersion();
	ui->typelabel->setText(m_group->FormulaType());
}

void GroupDFView::on_avtree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	m_group->AddDF((SoDFReader*)itm->myobj());
	refr();
}

void GroupDFView::on_usedtree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	m_group->RemDF((SoDFReader*)itm->myobj());
	refr();
}

void GroupDFView::on_expredit_textChanged(const QString &arg1)
{
	m_group->SetExpression(arg1);
}
