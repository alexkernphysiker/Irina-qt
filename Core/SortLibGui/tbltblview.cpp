#include "tbltblview.h"
#include "ui_tbltblview.h"

TblTblView::TblTblView(SoTableOfTables *tbl, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::TblTblView)
{
	m_tbl=tbl;
	ui->setupUi(this);
	refr();
	connect(m_tbl,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

TblTblView::~TblTblView()
{
	delete ui;
}

void TblTblView::refr(){
	ui->name->setText(m_tbl->Name());
	QString msg="";
	for(int i=0; i<m_tbl->Count();i++)
		msg+=QString::number(i)+") y = "+QString::number(m_tbl->GetY(i))+" => F(x,y) = "+m_tbl->GetUnaryName(i)+" (x)\n";
	ui->list->setPlainText(msg);
	if(m_tbl->Count()>0)
		ui->remat->setMaximum(m_tbl->Count()-1);
	else
		ui->remat->setMaximum(0);
}

void TblTblView::on_name_textChanged(const QString &arg1)
{
	m_tbl->Set_Name(arg1);
}

void TblTblView::on_add_clicked()
{
	m_tbl->Add(ui->y->value(),ui->unary->text());
}

void TblTblView::on_remove_clicked()
{
	m_tbl->Remove(ui->remat->value());
}
