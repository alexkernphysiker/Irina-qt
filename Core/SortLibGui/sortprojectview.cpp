#include <QtGui>
#include <QFileDialog>
#include <QDir>
#include <SortLibGui.h>
#include <Core/SortLib/sortlib_.h>
#include "additional.h"
#include "datafileview.h"
#include "valuegb.h"
#include "ui_sortprojectview.h"
#include "ecounterview.h"
#include "filterview.h"
#include "formulaview.h"
#include "dependecyview.h"
#include "groupdfview.h"
#include "tblfuncview.h"
#include "txtview.h"
#include "sp1normview.h"
#include "sp1normerrview.h"
#include "tbltblview.h"

SObjectTreeWidgetItem::SObjectTreeWidgetItem(SObject *so, myTreeWidget *w, bool recursive):QTreeWidgetItem(){
	m_object=so;
	setText(0,m_object->DisplayName());
	if(recursive){
		SortProject *proj=m_object->Owner();
		for(int i=0; i<proj->Count(); i++){
			SObject *o=proj->at(i);
			if(m_object == o->DisplParrent()){
				SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(o,w);
				addChild(item);
			}
		}
	}
	if(w!=NULL)
		QObject::connect(so,SIGNAL(changed(SObject*)),w,SLOT(changed(SObject*)));
}
SObjectTreeWidgetItem::~SObjectTreeWidgetItem(){}
SObject *SObjectTreeWidgetItem::myobj(){return m_object;}

TypeRegTreeWidgetItem::TypeRegTreeWidgetItem(SoTypeReg *typereg):QTreeWidgetItem(){
	m_typereg=typereg;
	setText(0,typereg->Caption());
}
TypeRegTreeWidgetItem::~TypeRegTreeWidgetItem(){}
SoTypeReg* TypeRegTreeWidgetItem::TypeReg(){return m_typereg;}

myTreeWidget::myTreeWidget(QWidget* parent, SortProject *sp):QTreeWidget(parent){
		m_project=sp;m_view=NULL;
		QTreeWidgetItem* headerItem = new QTreeWidgetItem();
		headerItem->setText(0,m_project->Path());
		setHeaderItem(headerItem);
		connect(this , SIGNAL(itemSelectionChanged()),this,SLOT(ViewInfo()));
		connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(OpenItem(QTreeWidgetItem*,int)));
		connect(m_project,SIGNAL(project_changed(SObject*)),this,SLOT(Refresh(SObject*)));
		Refresh(NULL);
}
myTreeWidget::~myTreeWidget(){}
SObject *myTreeWidget::SelectedSObject(){
	if(selectedItems().count()>0){
		return ((SObjectTreeWidgetItem*)selectedItems().at(0))->myobj();
	}else return NULL;
}
void myTreeWidget::LookForSelection(SObjectTreeWidgetItem *itm, SObject *so){
	if(itm->myobj()==so){
		setCurrentItem(itm);
		scrollToItem(itm,PositionAtCenter);
	}else{
		for(int i=0;(selectedItems().count()==0)& (i<itm->childCount());i++){
			SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)itm->child(i);
			LookForSelection(item,so);
		}
	}
}
void myTreeWidget::Refresh(SObject* tosel){
		clear();
		for(int i=0; i<m_project->Count(); i++){
			SObject *so=m_project->at(i);
			if(so->DisplParrent()==NULL	){
				SObjectTreeWidgetItem *item=new SObjectTreeWidgetItem(so,this);
				addTopLevelItem(item);
			}
		}
		if(tosel!=NULL){
			for(int i=0;(selectedItems().count()==0)& (i<topLevelItemCount());i++){
				SObjectTreeWidgetItem *itm=(SObjectTreeWidgetItem*)topLevelItem(i);
				LookForSelection(itm,tosel);
			}
		}
		expandAll();
}

void myTreeWidget::ViewInfo(){
	if(m_view!=NULL){
		delete m_view;
		m_view=NULL;
	}
	SObject *so= SelectedSObject();
	if(so!=NULL){
		m_view = (QWidget *)so->GetForm();
		if(m_view==NULL){
			if(so->Is(SOT_DFReader))
				m_view= new DataFileView((SoDFReader*)so);
			if(so->Is(SOT_ECounter))
				m_view=new ECounterView((SoECounter*)so);
			if(so->Is(SOT_Value))
				m_view= new valuegb((SoValue*)so);
			if(so->Is(SOT_EFilter))
				m_view=new FilterView((SoEFilter*)so);
			if(so->Is(SOT_Formula))
				m_view=new FormulaView((SoFormula*)so);
			if(so->Is(SOT_Dependency))
				m_view=new DependecyView((SoDependency*)so);
			if(so->Is(SOT_GroupDF))
				m_view=new GroupDFView((SoGroupDF*)so);
			if(so->Is(SOT_TblTbl))
				m_view=new TblTblView((SoTableOfTables*)so);
			if(so->Is(SOT_SoTblFunc)){
				m_view=new TblFuncView((SoTblFunc *)so);
				if(so->Is(SOT_SoTblFuncTxt))
					((TblFuncView * ) m_view)->AddTab("Text File",new txtview((SoTblFuncTxt*)so));
				if(so->Is(SOT_SP1Norm)){
					if(so->Is(SOT_SP1Norm_CoefErr))
						((TblFuncView * ) m_view)->AddTab("SP1 Norm (take coeff. error)",new SP1NormErrView((SP1Norm_CoefErr*)so));
					else
						((TblFuncView * ) m_view)->AddTab("SP1 Norm",new Sp1NormView((SP1Norm*)so));
				}
			}
		}
		if(m_view!=NULL)
			FlushInfoViewer(m_view);
	}
}
bool SObjectTreeWidgetItem::changed(SObject *so){
	bool res=false;
	if(m_object==so){
		res=true;
		setText(0,m_object->DisplayName());
	}
	for(int i=0; (!res)&(i<childCount());i++)
		res|=((SObjectTreeWidgetItem*)child(i))->changed(so);
	return res;
}
void myTreeWidget::changed(SObject *obj){
	//if(obj==NULL)return;
	bool done=false;
	for(int i=0; (!done)&(i<topLevelItemCount());i++){
		SObjectTreeWidgetItem *item=(SObjectTreeWidgetItem*)topLevelItem(i);
		done|=item->changed(obj);
	}
}
void myTreeWidget::OpenItem(QTreeWidgetItem* /*itm*/, int /*column*/){
		//myTreeWidgetItem *item=(myTreeWidgetItem*)itm;
}

SObject* AddTxt(SortProject *proj,SObject *){
	return new SoTblFuncTxt(proj);
}
SObject* AddSP1Norm(SortProject *,SObject *sel){
	if(sel!=NULL){
		if(sel->Is(SOT_ECSP1)){
			return new SP1Norm((SoECSP1*)sel);
		}else MsgBox("Please select an SP1");
	}else MsgBox("Please select an SP1");
	return NULL;
}
SObject* AddSP1Norm_WE(SortProject *,SObject *sel){
	if(sel!=NULL){
		if(sel->Is(SOT_ECSP1)){
			return new SP1Norm_CoefErr((SoECSP1*)sel);
		}else MsgBox("Please select an SP1");
	}else MsgBox("Please select an SP1");
	return NULL;
}

SObject* AddTblTbl(SortProject *proj, SObject *){
	return new SoTableOfTables(proj);
}

SortProjectView::SortProjectView(SortProject *project, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SortProjectView)
{
	ui->setupUi(this);
	m_project=project;m_busy=false;
	QDir dir=QDir::current();
	QString str=m_project->Path();
	ui->pathlabel->setText(dir.absoluteFilePath(str));
	top_=ui->pathlabel->y()+ui->pathlabel->height();
	height_=ui->statuslabel->y()-ui->pathlabel->y()-ui->pathlabel->height();
	m_tree=new myTreeWidget(ui->centralwidget,m_project);
	m_tree->setFixedHeight(height_);
	m_tree->setFixedWidth((width()/3));
	m_tree->move(0,top_);
	m_tree->Refresh(NULL);
	busy(m_project->IsBusy());
	connect(m_tree,SIGNAL(FlushInfoViewer(QWidget*)),this,SLOT(TakeInfoView(QWidget*)));
	connect(this,SIGNAL(closing()),m_project, SLOT(sl_gui_close()));
	connect(this,SIGNAL(OutputSetup()),m_project,SLOT(sl_out_config()));
	connect(m_project,SIGNAL(busy(bool)),this, SLOT(busy(bool)));
	connect(m_project,SIGNAL(statusmsg(QString)),this,SLOT(statusmsg(QString)));

	RegisterSoType(new SoTypeReg("","Table function from text file",SoCatOther,SOT_SoTblFuncTxt,NULL,&AddTxt));
	RegisterSoType(new SoTypeReg("","Table func. Normalize SP1",SoCatOther,SOT_SP1Norm,NULL,&AddSP1Norm));
	RegisterSoType(new SoTypeReg("","Table func. Normalize SP1 (take coeff. error)",SoCatOther,SOT_SP1Norm_CoefErr,NULL,&AddSP1Norm_WE));
	RegisterSoType(new SoTypeReg("","Binary: interpolation between several unaries",SoCatOther,SOT_TblTbl,NULL,&AddTblTbl));
	{
		int n=RegisteredTypesCount();
		for(int i=0; i<n; i++){
			SoTypeReg* reg=GetRegisteredType(i);
			Adder* adder=new Adder(reg,this);
			switch(reg->Category()){
			case SoCatData:{
				ui->menuDataFiles->addAction(reg->Caption(),adder,SLOT(Triger()));
			}break;
			case SoCatFilter:
				ui->menuND_filter->addAction(reg->Caption(),adder,SLOT(Triger()));
				break;
			case SoCatFormula:
				ui->menuEquations_and_magnitudes->addAction(reg->Caption(),adder,SLOT(Triger()));
				break;
			case SoCatOther:
				ui->menuOther->addAction(reg->Caption(),adder,SLOT(Triger()));
				break;
			}
		}
	}
}

SortProjectView::~SortProjectView()
{
	delete ui;
	UnregisterSoType(SOT_SoTblFuncTxt);
	UnregisterSoType(SOT_SP1Norm);
	UnregisterSoType(SOT_SP1Norm_CoefErr);
	UnregisterSoType(SOT_TblTbl);
}

Adder::Adder(SoTypeReg *reg, SortProjectView *parent){
	m_reg=reg;
	m_parent=parent;
}
Adder::~Adder(){}

void Adder::Triger(){
	SObject *item=m_reg->createInstance(m_parent->Project(),m_parent->SelectedObj());
	if(item!=NULL)
		m_parent->Project()->Add(item);
}

SObject *SortProjectView::SelectedObj(){return m_tree->SelectedSObject();}
SortProject *SortProjectView::Project(){return m_project;}

void SortProjectView::on_actionSave_activated()
{
		m_project->Save();
}

void SortProjectView::on_actionSave_triggered()
{

}

void SortProjectView::TakeInfoView(QWidget *widg){
	widg->setParent(ui->centralwidget);
	widg->setFixedHeight(height_);
	widg->setFixedWidth(2*(width()/3));
	widg->move((width()/3),top_);
	widg->setVisible(true);
}

void SortProjectView::on_actionRecalculate_activated()
{
	m_project->Refresh();
}

void SortProjectView::on_actionAdd_activated()
{//add filter
	m_project->Insert(new SoEFilter(m_project),0);
}

void SortProjectView::on_actionAdd_constant_activated()
{
	m_project->Insert(new SoValue(m_project),0);
}

void SortProjectView::on_actionAdd_2_activated()
{//addformula
	m_project->Add(new SoFormula(m_project));
}


void SortProjectView::on_actionAdd_spectrum_axis_activated()
{
	m_project->Add(new SoSPAxis(m_project));
}

void SortProjectView::on_actionRemove_selected_element_activated()
{
	SObject *so=m_tree->SelectedSObject();
	if(so!=NULL){
		m_project->Remove(m_project->Number(so));
	}
}

void SortProjectView::on_actionClear_project_activated()
{
	m_project->Clear();
}

void SortProjectView::on_actionWindow_activated()
{
	SObject *so=m_tree->SelectedSObject();
	if(so!=NULL)
		if(so->Is(SOT_SPAxis)){
			m_project->Insert(new SoEFWin((SoSPAxis*)so),m_project->Number(so)+1);
			return;
		}
	MsgBox("Please select an SPAXIS.");
}

void SortProjectView::on_actionLocus_activated()
{
	m_project->Add(new SoEFLoc(m_project));
}

void MsgBox(QString txt){
	QMessageBox *msg=new QMessageBox();
	msg->setText(txt);
	msg->setWindowTitle(aboutsortlibcaption());
	msg->exec();
}
bool AskYesOrNo(QString str){
	QMessageBox *msg=new QMessageBox();
	msg->setWindowTitle(aboutsortlibcaption());
	msg->setText(str);
	msg->setStandardButtons(QMessageBox::Yes|QMessageBox::No);
	return QMessageBox::Yes==msg->exec();
}

void SortProjectView::on_actionX_Y_deltaY_activated()
{
	m_project->Add(new SoDependency(m_project));
}

void SortProjectView::on_actionAdd_DataFile_s_function_activated()
{
	m_project->Add(new SoGroupDF(m_project));
}

void SortProjectView::on_actionScatter_with_appendix_activated()
{
	m_project->Add(new SoDSewed(m_project));
}

void SortProjectView::closeEvent(QCloseEvent *event){
	if(m_busy){
		event->ignore();
		MsgBox("Cannot close application because data analysis is running.");
		return;
	}
	closing();
	if(AskYesOrNo("Do You want to save this project?"))
		m_project->Save();
}

void SortProjectView::on_actionDisplay_About_message_triggered()
{
	MsgBox(QString("Application for analysis of data obtained in multiparameter")+
		   " nuclear experiments and experiments with coincidents\n"+aboutsortlibmsg()+"\nData output extension:\n   "+m_project->DataOutput());
}

void SortProjectView::on_actionEnergy_calibration_at_SP2_activated()
{
	m_project->Add(new SoSPEnergy(m_project));
}

void SortProjectView::busy(bool val){
	if(m_busy==val)return;
	if(!val)ui->statuslabel->setText("");
	m_busy=val;
	this->setEnabled(!val);
	update();
}

void SortProjectView::statusmsg(QString msg){
	ui->statuslabel->setText(msg);
	update();
}

void SortProjectView::on_actionOutput_settings_activated(){
	OutputSetup();
}
