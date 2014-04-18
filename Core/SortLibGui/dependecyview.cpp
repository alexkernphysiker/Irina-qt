#include "dependecyview.h"
#include "ui_dependecyview.h"
#include "additional.h"
#include <SortLibGui.h>

DependecyView::DependecyView(SoDependency *dep, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::DependecyView)
{
	ui->setupUi(this);
	m_dependency=dep;
	ui->nameedit->setText(m_dependency->Name());
	ui->argedit->setText(m_dependency->Argument());
	ui->valedit->setText(m_dependency->Value());
	ui->xerror->setText(m_dependency->XError());
	ui->erredit->setText(m_dependency->YError());
	ui->sewbox->setVisible(m_dependency->Is(SOT_DSewed));
	if(m_dependency->Is(SOT_DSewed)){
		ui->sewname->setText(((SoDSewed *)m_dependency)->Sewed());
	}
	if(m_dependency->Is(SOT_DSewed)){
		((SoDSewed*)m_dependency)->Sewed();
	}
	SortProject *proj=m_dependency->Owner();
	for(int i=0; i<proj->Number(m_dependency);i++){
		SObject *so=proj->at(i);
		if(so->Is(SOT_DFReader)){
			SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(so,NULL,false);
			ui->avtree->addTopLevelItem(item);
		}
	}
	refr();
}
void DependecyView::refr(){
	ui->usedtree->clear();
	for(int i=0; i<m_dependency->DFCnt();i++){
		SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(m_dependency->GetDF(i),NULL,false);
		ui->usedtree->addTopLevelItem(item);
	}
}
DependecyView::~DependecyView()
{
	delete ui;
}

void DependecyView::on_nameedit_textChanged(const QString &arg1)
{
	m_dependency->Set_Name(arg1);
}

void DependecyView::on_argedit_textChanged(const QString &arg1)
{
	m_dependency->SetArgument(arg1);
}

void DependecyView::on_valedit_textChanged(const QString &arg1)
{
	m_dependency->SetValue(arg1);
}

void DependecyView::on_erredit_textChanged(const QString &arg1)
{
	m_dependency->SetYError(arg1);
}

void DependecyView::on_avtree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	m_dependency->AddDF((SoDFReader*)itm->myobj());
	refr();
}

void DependecyView::on_usedtree_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)item;
	m_dependency->RemDF((SoDFReader*)itm->myobj());
	refr();
}

void DependecyView::on_pushButton_clicked()
{
	m_dependency->Export();
}

void DependecyView::on_sewname_textChanged(const QString &arg1)
{
	if(m_dependency->Is(SOT_DSewed)){
		((SoDSewed*)m_dependency)->SetSewed(arg1);
	}
}

void DependecyView::on_xerror_textChanged(const QString &arg1)
{
	m_dependency->SetXError(arg1);
}
