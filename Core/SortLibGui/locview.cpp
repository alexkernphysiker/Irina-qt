#include <additional.h>
#include "locview.h"
#include "ui_locview.h"
#include <SortLibGui.h>

LocView::LocView(SoEFLoc *loc, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::LocView)
{
	ui->setupUi(this);
	m_loc=loc;
	for(int i=0; i<loc->Owner()->Number(loc);i++){
		SObject *so=loc->Owner()->at(i);
		if(so->Is(SOT_SPAxis))ui->AxTree->addTopLevelItem(new SObjectTreeWidgetItem(so,NULL,false));
	}
	refr();
	connect(m_loc,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

LocView::~LocView()
{
	delete ui;
}
SoSPAxis *LocView::SelectedAxis(){
	if(ui->AxTree->selectedItems().count()){
		SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)ui->AxTree->selectedItems()[0];
		SoSPAxis *ax=(SoSPAxis*)item->myobj();
		return ax;
	}
	return NULL;
}
void LocView::refr(){
	ui->xlabel->setText("not set");
	if(m_loc->XAxis()!=NULL)ui->xlabel->setText(m_loc->XAxis()->Name());
	ui->ylabel->setText("not set");
	if(m_loc->YAxis()!=NULL)ui->ylabel->setText(m_loc->YAxis()->Name());
}

void LocView::on_xbut_clicked()
{
	SoSPAxis *ax=SelectedAxis();
	if(ax!=NULL)m_loc->SetXAxis(ax);
	refr();
}

void LocView::on_ybut_clicked()
{
	SoSPAxis *ax=SelectedAxis();
	if(ax!=NULL)m_loc->SetYAxis(ax);
	refr();
}
