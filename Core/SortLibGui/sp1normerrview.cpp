#include "sp1normerrview.h"
#include "ui_sp1normerrview.h"

SP1NormErrView::SP1NormErrView(SP1Norm_CoefErr *sp1, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::SP1NormErrView)
{
	ui->setupUi(this);
	m_sp1=sp1;
	refr();
	connect(m_sp1,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

SP1NormErrView::~SP1NormErrView()
{
	delete ui;
}

void SP1NormErrView::refr(){
	ui->valname->setText(m_sp1->NormFunc());
	ui->errname->setText(m_sp1->NormFuncErr());
}

void SP1NormErrView::on_Apply_clicked()
{
	m_sp1->SetNormFunc(ui->valname->text(),ui->errname->text());
}
