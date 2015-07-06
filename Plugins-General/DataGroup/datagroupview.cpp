// this file is distributed under
// GPL v 3.0 license
#include "datagroupview.h"
#include "ui_datagroupview.h"
#include <Core/SortLib/sortlib_.h>
#include <SortLibGui.h>

DataGroupView::DataGroupView(DataGroup *group, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::DataGroupView)
{
	m_master=group;
	ui->setupUi(this);
	for(int i=0; i< RegisteredTypesCount();i++){
		SoTypeReg *factory=GetRegisteredType(i);
		if(factory->Category()==SoCatData)
			if(factory->Signature()!=SOT_DataGroup){
				ui->SourceTypes->addTopLevelItem(new TypeRegTreeWidgetItem(factory));
			}
	}
	connect(m_master,SIGNAL(changed(SObject*)),this, SLOT(changed(SObject*)));
	changed(m_master);
}
DataGroupView::~DataGroupView()
{
	delete ui;
}

void DataGroupView::changed(SObject *){
	QString str="";
	switch(m_master->GetType()){
	case DGSumm:
		str="Variables are summed";
		break;
	case DGAverage:
		str="Average value for variables is taken";
		break;
	}
	ui->DGType->setText(str);
	ui->ListOfSources->clear();
	for(int i=0; i<m_master->GetDFReaderCount();i++){
		ui->ListOfSources->addTopLevelItem(new SObjectTreeWidgetItem(m_master->GetDFReader(i),NULL,false));
	}
}


void DataGroupView::on_DGTypes_itemDoubleClicked(QTreeWidgetItem *item, int )
{
	switch(ui->DGTypes->indexOfTopLevelItem(item)){
	case 0:
		m_master->SetType(DGSumm);
		break;
	case 1:
		m_master->SetType(DGAverage);
	}
}

void DataGroupView::on_SourceTypes_itemDoubleClicked(QTreeWidgetItem *item, int )
{
	TypeRegTreeWidgetItem* myitem=(TypeRegTreeWidgetItem*)item;
	m_master->AddDFReader(myitem->TypeReg());
}

void DataGroupView::on_RemoveButton_clicked()
{
	if(ui->ListOfSources->selectedItems().count()==0)return;
	QTreeWidgetItem* item=ui->ListOfSources->selectedItems()[0];
	int index=ui->ListOfSources->indexOfTopLevelItem(item);
	m_master->RemDFReader(index);
}
