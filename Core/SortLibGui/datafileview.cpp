// this file is distributed under
// GPL v 3.0 license
#include "datafileview.h"
#include "ui_datafileview.h"

DataFileView::DataFileView(SoDFReader *obj, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::DataFileView)
{
	m_reader=obj;
	ui->setupUi(this);
	changed(NULL);
	busy(m_reader->Owner()->IsBusy());
	connect(m_reader,SIGNAL(changed(SObject*)),this,SLOT(changed(SObject*)));
	connect(m_reader->Owner(), SIGNAL(busy(bool)),this, SLOT(busy(bool)));
}
void DataFileView::changed(SObject */*so*/){
	ui->descr->setPlainText(m_reader->Description());
	ui->lineEdit->setText(m_reader->Name());
	QString msg="Variable List:";
	for(int i=0; i<m_reader->VarCnt();i++){
		msg+="\n"+QString::number(i)+") "+
				m_reader->VarName(i)+" = "+QString::number(m_reader->Variable(m_reader->VarName(i)));
	}
	ui->varlist->setText(msg);
	if(m_reader->VarCnt()>0)ui->numtodel->setMaximum(m_reader->VarCnt()-1);
}
DataFileView::~DataFileView()
{
	delete ui;
}

void DataFileView::on_addbut_clicked()
{
	m_reader->ApplyVariable(ui->newvarname->text(),ui->newvarval->value());
}

void DataFileView::on_rembut_clicked()
{
	m_reader->RemVar(ui->numtodel->value());
}

void DataFileView::on_evcnt_clicked()
{
	SortProject *proj=m_reader->Owner();
	proj->Add(new SoECounter(m_reader));
}

void DataFileView::on_lineEdit_textChanged(const QString &arg1)
{
	m_reader->Set_Name(arg1);
}

void DataFileView::on_pushButton_clicked()
{
	m_reader->Read();
}

void DataFileView::on_sp1_clicked()
{
	m_reader->Owner()->Add(new SoECSP1(m_reader));
}

void DataFileView::on_sp2_clicked()
{
	m_reader->Owner()->Add(new SoECSP2(m_reader));
}

void DataFileView::AddTab(QString name, QWidget *control){
	ui->tabWidget->addTab(control,name);
}
void DataFileView::busy(bool val){
	ui->pushButton->setEnabled(!val);
	ui->evcnt->setEnabled(!val);
	ui->sp1->setEnabled(!val);
	ui->sp2->setEnabled(!val);
}
