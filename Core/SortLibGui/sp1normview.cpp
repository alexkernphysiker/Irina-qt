#include "sp1normview.h"
#include "ui_sp1normview.h"

Sp1NormView::Sp1NormView(SP1Norm *sp1, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::Sp1NormView)
{
	ui->setupUi(this);
	m_sp1=sp1;
	refr();
	connect(m_sp1,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

Sp1NormView::~Sp1NormView()
{
	delete ui;
}
void Sp1NormView::refr(){
	ui->lineEdit->setText(m_sp1->NormFunc());
}
void Sp1NormView::on_pushButton_clicked()
{
	m_sp1->SetNormFunc(ui->lineEdit->text());
}
